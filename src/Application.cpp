#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Freemove.h"
#include "systems/Listener.h"

#include <glm/gtc/random.hpp>

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	meshManager("../data/models.zip"),
	shaderManager("../data/shaders.zip"),
	textureManager("../data/textures.zip"),
	configManager("../data/config.zip"),
	sceneManagerUserData{ &meshManager, &textureManager },
	sceneManager("../data/scenes.zip", &sceneManagerUserData),
	shader(shaderManager.Get("default.shader")),
	laserShader(shaderManager.Get("laser.shader")),
	audioPlayer("../data/audio.zip")
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
	Systems::UpdateListener();
}

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Systems::Render(shader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f);
	Systems::RenderLaser(laserShader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f, laservao, textureManager.Get("laser.rgba")->GetTexture());
}
