#include "System.h"

System::System(glm::uvec2 screenSize, const std::string& title, int, char*[]):
	event{0}
{
	this->screenSize = screenSize;
	this->running = true;

	if (SDL_Init(SDL_INIT_VIDEO) > 0)
	{
		throw std::runtime_error("SDL_Init failed");
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	//Create window
	window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenSize.x, screenSize.y, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	if (window == nullptr)
	{
		throw std::runtime_error("SDL_CreateWindow failed");
	}

	//Create OpenGL context
	context = SDL_GL_CreateContext(window);
	if (context == nullptr)
	{
		throw std::runtime_error("SDL_GL_CreateContext failed");
	}

	//Initialize GLEW
	glewExperimental = GL_TRUE;
	GLenum glewError = glewInit();
	if (glewError != GLEW_OK)
	{
		throw std::runtime_error("glewInit failed");
	}

	//Use Vsync
	if (SDL_GL_SetSwapInterval(1) < 0)
	{
		std::cerr << "Warning: Unable to set VSync" << std::endl;
	}
	//Enable depth test
	glEnable(GL_DEPTH_TEST);


	//Initialize OpenAL
	alDevice = alcOpenDevice(NULL);
	if (!alDevice)
	{
		throw std::runtime_error("alcOpenDevice failed");
	}
	//alGetError();
	alContext = alcCreateContext(alDevice, NULL);
	if (!alcMakeContextCurrent(alContext))
	{
		throw std::runtime_error("alcMakeContextCurrent failed");
	}

	if (alGetError() != AL_NO_ERROR)
	{
		throw std::runtime_error("OpenAL failed");
	}
}

System::~System()
{
	alDevice = alcGetContextsDevice(alContext);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(alContext);
	alcCloseDevice(alDevice);

	SDL_DestroyWindow(window);
	SDL_Quit();
}

void System::Run()
{
	float dt = 1 / 60.0f;
	float currentTime = SDL_GetTicks() / 1000.0f;

	while (running)
	{
		//Handle SDL events
		while (SDL_PollEvent(&event) != 0)
		{
			HandleEvent(event);
			switch (event.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			default:
				break;
			}
		}
		//Update
		float newTime = SDL_GetTicks() / 1000.0f;
		float frameTime = newTime - currentTime;
		currentTime = newTime;
		
		while (frameTime > 0.0f)
		{
			float deltaTime = std::min(frameTime, dt);
			Update(deltaTime);
			frameTime -= deltaTime;
		}
		
		//Render
		Render();
		SDL_GL_SwapWindow(window);
	}
}

glm::uvec2 System::GetScreenSize() const
{
	return screenSize;
}
