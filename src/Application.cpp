#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Freemove.h"

#include <glm/gtc/random.hpp>

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	meshManager("../data/models/"),
	shaderManager("../data/shaders/"),
	textureManager("../data/textures/"),
	configManager("../data/config/"),
	sceneManagerUserData{&meshManager, &textureManager},
	sceneManager("../data/scenes/", &sceneManagerUserData),
	audioManager("../data/audio/"),
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
	sceneManager.Get("floor.txt");


	


	

	
	alGenSources((ALuint)1, &source);
	// check for errors

	alSourcef(source, AL_PITCH, 1);
	alSourcef(source, AL_GAIN, 1);
	alSource3f(source, AL_POSITION, 0, 0, 0);
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	alSourcei(source, AL_LOOPING, true);
	alSourcei(source, AL_BUFFER, audioManager.Get("beep.ogg")->GetBuffer());

}


Application::~Application()
{
	camera.purge();
	light.purge();

	// cleanup context
	alDeleteSources(1, &source);
}

void Application::HandleEvent(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEBUTTONDOWN:
	{
		std::cout << "Mouse button down" << std::endl;
		

		int state;
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		if (state == AL_PLAYING)
		{
			alSourcePause(source);
		}
		else
		{
			alSourcePlay(source);
		}

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

	
	alListenerfv(AL_POSITION, glm::value_ptr(kult::get<Component::Position>(camera).pos));
	alListenerfv(AL_VELOCITY, glm::value_ptr(kult::get<Component::Physics>(camera).velocity));

	glm::vec3 dir = kult::get<Component::Position>(camera).rot * glm::vec3(1, 0, 0);
	ALfloat listenerOri[] = { dir.x, dir.y, dir.z, 0.0f, 0.0f, 1.0f };

	alListenerfv(AL_ORIENTATION, listenerOri);
}

void Application::Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Systems::Render(shader, camera, GetScreenSize(), 59.0f, 0.1f, 1000.0f);
}
