#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Draglook.h"
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
	audioPlayer("../data/audio.zip")
{
	//SDL_SetRelativeMouseMode(SDL_TRUE);

	glClearColor(0.0f, 0.6f, 0.0f, 1.0f);

	kult::add<Component::Position>(camera) = {
		glm::vec3(0, 0, 2)
	};
	//kult::add<Component::Freemove>(camera) = {
	//	configManager.Get("fSpeed")->GetFloat(),
	//	{
	//		configManager.Get("kForward")->GetScancode(),
	//		configManager.Get("kBackward")->GetScancode(),
	//		configManager.Get("kLeft")->GetScancode(),
	//		configManager.Get("kRight")->GetScancode(),
	//		configManager.Get("kFaster")->GetScancode()
	//	}
	//};
	kult::add<Component::Draglook>(camera) = {
		configManager.Get("fSensitivity")->GetFloat(),
		glm::vec3(0, 0, 0),
		5.0f,
		SDL_BUTTON_LMASK
	};
	kult::add<Component::Physics>(camera);
	kult::add<Component::Listener>(camera);

	kult::add<Component::Position>(light) = {
		glm::vec3(0, 0, 1),
		glm::quat(glm::vec3(0, 0, 0)),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::Light>(light) = {
		glm::vec3(1, 1, 1)
	};
	sceneManager.Get("tictactoe.txt");
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
	case SDL_MOUSEMOTION:
	{
		glm::vec2 m(event.motion.x, event.motion.y);

		if (event.motion.state & SDL_BUTTON_LMASK && m.length() > 1 && !audioPlayer.IsPlaying("cameraClick"))
		{
			audioPlayer.Play("cameraClick", "camera.ogg", { 0, 0, 0 }, { 0, 0, 0 });
		}
		break;
	}
	default:
		break;
	}
	Systems::HandleFreelookEvent(event);
	Systems::HandleDraglookEvent(event);
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
}
