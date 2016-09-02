#pragma once

#include <iostream>
#include <stdexcept>
#include <algorithm>

#include <GL/glew.h>
#include <glm/glm.hpp>

#ifdef _WIN32
	#include <SDL.h>
#else
	#include <SDL2/SDL.h>
#endif // _WIN32

class System
{
private:
	glm::uvec2 screenSize;
	bool running;

	SDL_Window* window;
	SDL_GLContext context;

	SDL_Event event;
	
public:
	System(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);
	virtual ~System();

	void Run();
	glm::uvec2 GetScreenSize() const;

	virtual void HandleEvent(SDL_Event& event) = 0;
	virtual void Update(float deltaTime) = 0;
	virtual void Render() = 0;
};

