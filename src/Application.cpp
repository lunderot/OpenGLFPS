#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/AnimatedRender.h"
#include "systems/Freelook.h"
#include "systems/Freemove.h"
#include "systems/Listener.h"

#include <glm/gtc/random.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/common.hpp>

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	meshManager("../data/models.zip"),
	animatedMeshManager("../data/animatedmodels.zip"),
	shaderManager("../data/shaders.zip"),
	textureManager("../data/textures.zip"),
	configManager("../data/config.zip"),
	sceneManagerUserData{ &meshManager, &textureManager },
	sceneManager("../data/scenes.zip", &sceneManagerUserData),
    animationManager("../data/animations.zip"),
	audioPlayer("../data/audio.zip"),
	shader(shaderManager.Get("default.shader")),
	animatedShader(shaderManager.Get("animated.shader")),
	laserShader(shaderManager.Get("laser.shader"))
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	kult::add<Component::Position>(camera) = {
		glm::vec3(0, 0, 2)
	};
	kult::add<Component::Freelook>(camera) = {
		configManager.Get("fSensitivity")->GetFloat()
	};
	kult::add<Component::Physics>(camera);
	kult::add<Component::Freemove>(camera) = {
		configManager.Get("fSpeed")->GetFloat()
	};
	kult::add<Component::Listener>(camera);

	kult::add<Component::Position>(cube) = {
		glm::vec3(0, 0, 0),
		glm::quat(glm::vec3(0, 0, 0)),
		glm::vec3(1, .1, .1)
	};
	kult::add<Component::Render>(cube) = {
		meshManager.Get("cube.obj"),
		textureManager.Get("wood.rgba"),
		true
	};
	kult::add<Component::Light>(cube) = {
		glm::vec3(0, 1, 0)
	};

	kult::add<Component::Position>(light) = {
		glm::vec3(1, 0, 1),
		glm::quat(glm::vec3(0, 0, 0)),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::Light>(light) = {
		glm::vec3(1, 1, 1)
	};
	sceneManager.Get("scene.txt");
	sceneManager.Get("floor.txt");

	kult::add<Component::Position>(animation) = {
		{3, 0, 1},
		glm::quat(glm::vec3(0, 0, 0)),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::AnimatedRender>(animation) = {
		animatedMeshManager.Get("quad.obj"),
		textureManager.Get("missing.rgba"),
		animationManager.Get("move.txt")
	};
	
	//Generate a vertex buffer with 2 points for lasers
	glGenBuffers(1, &laservbo);

	float points[] = {
		0.0f,  0.0f, 0.0f, 
		1.0f, 0.0f, 0.0f
	};

	glBindBuffer(GL_ARRAY_BUFFER, laservbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

	// Create VAO
	glGenVertexArrays(1, &laservao);
	glBindVertexArray(laservao);

	// Specify layout of point data
	GLint posAttrib = 0;
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 0, 0);

	kult::entity laser;
	kult::add<Component::Position>(laser) = {
		glm::vec3(8, 0, 2)
	};

	kult::add<Component::Particle>(laser) = {
		Component::ParticleData::Type::LASER
	};
	kult::entity laser2;
	kult::add<Component::Position>(laser2) = {
		glm::vec3(8, 3, 2)
	};

	kult::add<Component::Particle>(laser2) = {
		Component::ParticleData::Type::LASER
	};
}

Application::~Application()
{
	camera.purge();
	cube.purge();
	light.purge();
}

void Application::HandleEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	{
		std::cout << "Mouse button down" << std::endl;
		audioPlayer.Play("bep", "beep.ogg", { 0, 0, 5 });
		kult::get<Component::Position>(light).pos = kult::get<Component::Position>(camera).pos;
		animationManager.Reload("move.txt");
		break;
	}
	default:
		break;
	}
	Systems::HandleFreelookEvent(event);
}

void Application::Update(float deltaTime)
{
	Systems::UpdateFreemove();
	Systems::Physics(deltaTime);

	glm::f32 g = 0.55191502449;
	glm::vec2 f(10, 14);
	glm::vec3 points[] =
	{
		{ 0 * f.x, 1 * f.y, 3 },
		{ g * f.x, 1 * f.y, 3 },
		{ 1 * f.x, g * f.y, 3 },
		{ 1 * f.x, 0 * f.y, 3 },
		{ 1 * f.x, 0 * f.y, 3 },
		{ 1 * f.x, -g * f.y, 3 },
		{ g * f.x, -1 * f.y, 3 },
		{ 0 * f.x, -1 * f.y, 3 },
		{ 0 * f.x, -1 * f.y, 3 },
		{ -g * f.x, -1 * f.y, 3 },
		{ -1 * f.x, -g * f.y, 3 },
		{ -1 * f.x, 0 * f.y, 3 },
		{ -1 * f.x, 0 * f.y, 3 },
		{ -1 * f.x, g * f.y, 3 },
		{ -g * f.x, 1 * f.y, 3 },
		{ 0 * f.x, 1 * f.y, 3 },
	};
	glm::f32 ticks = SDL_GetTicks() % 10000 / 10000.0f;
	glm::vec3 pos0 = GetBezierPoint<16>(points, ticks);
	glm::vec3 pos1 = GetBezierPoint<16>(points, ticks+0.00001f);

	auto af = pos1 - pos0;
	kult::get<Component::Position>(cube).pos = pos0;
	kult::get<Component::Position>(cube).rot = glm::rotation(glm::vec3(1, 0, 0), normalize(af));
	Systems::UpdateListener();
	dt = deltaTime;
}


void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Systems::Render(shader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f);
	Systems::AnimatedRender(animatedShader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f, dt);
	Systems::RenderLaser(laserShader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f, laservao, textureManager.Get("laser.rgba")->GetTexture());
}
