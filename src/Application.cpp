#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Freemove.h"

#include <glm/gtc/random.hpp>

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	meshManager("data/models/"),
	shaderManager("data/shaders/"),
	textureManager("data/textures/"),
	configManager("data/config/"),
	sceneManagerUserData{&meshManager, &textureManager},
	sceneManager("data/scenes/", &sceneManagerUserData),
	shader(shaderManager.Get("default.shader"))
{
	SDL_SetRelativeMouseMode(SDL_TRUE);
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	kult::add<Component::Position>(camera) = {
		glm::vec3(0, 0, 2)
	};
	kult::add<Component::Freelook>(camera) = {
		configManager.Get("camera/fSensitivity")->GetFloat()
	};
	kult::add<Component::Physics>(camera);
	kult::add<Component::Freemove>(camera) = {
		configManager.Get("camera/fSpeed")->GetFloat()
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
	sceneManager.Get("scene2.txt");
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
}

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Systems::Render(shader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f);
}
