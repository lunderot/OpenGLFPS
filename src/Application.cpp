#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Freemove.h"

#include <glm/gtc/random.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/common.hpp>

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	meshManager("data/models/"),
	shaderManager("data/shaders/"),
	textureManager("data/textures/"),
	shader(shaderManager.GetAsset("default"))
{
	kult::add<Component::Position>(camera) = {
		glm::vec3(0, 0, 2)
	};
	kult::add<Component::Freelook>(camera) = {
		0.002f
	};
	kult::add<Component::Physics>(camera);
	kult::add<Component::Freemove>(camera) = {
		5.0f
	};

	kult::add<Component::Position>(cube) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(1, .1, .1)
	};
	kult::add<Component::Render>(cube) = {
		meshManager.GetAsset("cube.obj"),
		textureManager.GetAsset("wood.raw"),
		true
	};

	kult::add<Component::Position>(floor) = {
		glm::vec3(0, 0, 0),
		glm::quat(),
		glm::vec3(30, 30, 1)
	};
	kult::add<Component::Render>(floor) = {
		meshManager.GetAsset("quad.obj"),
		textureManager.GetAsset("wood.raw"),
		true
	};

	kult::add<Component::Position>(tree) = {
		glm::vec3(10, 0, 0),
		glm::quat(),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::Render>(tree) = {
		meshManager.GetAsset("tree.obj"),
		textureManager.GetAsset("missing.raw"),
		false
	};
	kult::add<Component::Physics>(tree) = {
		glm::vec3(0, 0, 0),
		glm::zero<glm::vec3>(),
		glm::vec3(0, 0, 1)
	};

	SDL_SetRelativeMouseMode(SDL_TRUE);
}


Application::~Application()
{
	camera.purge();
	cube.purge();
	tree.purge();
	floor.purge();
}

void Application::HandleEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	{
		std::cout << "Mouse button down" << std::endl;
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
	glm::f32 ticks = SDL_GetTicks() % 100000 / 100000.0f;
	glm::vec3 pos0 = GetBezierPoint<16>(points, ticks);
	glm::vec3 pos1 = GetBezierPoint<16>(points, ticks+0.00001f);

	auto af = pos1 - pos0;
	kult::get<Component::Position>(cube).pos = pos0;
	kult::get<Component::Position>(cube).rot = glm::rotation(glm::vec3(1, 0, 0), normalize(af));
}


void Application::Render()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Systems::Render(shader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f);
}