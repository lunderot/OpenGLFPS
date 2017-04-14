#include <sstream>

#include "Application.h"

int main(int argc, char* argv[])
{
	try
	{
		Application app(glm::uvec2(1280, 720), "OpenGLFPS", argc, argv);
		app.Run();
	}
	catch (std::exception e)
	{
		std::stringstream ss;
		ss << e.what() << std::endl
			<< "SDL error: " << SDL_GetError() << std::endl
			<< "OpenAL error: " << alGetError();
			;
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", ss.str().c_str(), nullptr);
		return 1;
	}
	return 0;
}
