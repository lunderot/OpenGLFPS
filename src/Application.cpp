#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Draglook.h"
#include "systems/Freemove.h"
#include "systems/Listener.h"

Application::Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]):
	System(screenSize, title, argc, argv),
	near(0.1f),
	far(1000.0f),
	fov(59.9f),
	meshManager("../data/models.zip"),
	shaderManager("../data/shaders.zip"),
	textureManager("../data/textures.zip"),
	configManager("../data/config.zip"),
	sceneManagerUserData{ &meshManager, &textureManager },
	sceneManager("../data/scenes.zip", &sceneManagerUserData),
	shader(shaderManager.Get("default.shader")),
	audioPlayer("../data/audio.zip")
{
	board[0].reset();
	board[1].reset();

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
		SDL_BUTTON_RMASK
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


	kult::add<Component::Position>(marker[0]) = {
		glm::vec3(0, 0, -10000),
		glm::quat(),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::Render>(marker[0]) = {
		meshManager.Get("cross.obj"),
		textureManager.Get("red.rgba"),
		false
	};

	kult::add<Component::Position>(marker[1]) = {
		glm::vec3(0, 0, -10000),
		glm::quat(),
		glm::vec3(1, 1, 1)
	};
	kult::add<Component::Render>(marker[1]) = {
		meshManager.Get("circle.obj"),
		textureManager.Get("blue.rgba"),
		false
	};
	ghost = &marker[0];
	ghostid = false;
}


Application::~Application()
{
	camera.purge();
	light.purge();
	marker[0].purge();
	marker[1].purge();
}

void Application::PlacementLogic(SDL_Event& event)
{
	switch (event.type)
	{
	case SDL_MOUSEMOTION:
	{
		//Play a sound if the player is moving the camera
		glm::vec2 m(event.motion.x, event.motion.y);
		if (event.motion.state & SDL_BUTTON_RMASK && m.length() > 1 && !audioPlayer.IsPlaying("cameraClick"))
		{
			audioPlayer.Play("cameraClick", "camera.ogg", { 0, 0, 0 }, { 0, 0, 0 });
		}

		//Get a ray from camera and mouse position
		glm::vec3 ray = GetRayFromMouse(glm::vec2(event.motion.x, event.motion.y), camera);
		auto& cameraPos = kult::get<Component::Position>(camera).pos;

		//Calculate the intersection between the ray and board
		glm::f32 distance;
		bool isIntersecting = glm::intersectRayPlane(cameraPos, ray, glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), distance);

		//Round the result
		glm::vec3 finalPosition = cameraPos + ray * distance + glm::vec3(0.5f, 0.5f, 0.0f);
		finalPosition = glm::floor(finalPosition);
		finalPosition.z = 0.0f;

		if (isIntersecting)
		{
			//If the ghost is in a invalid position, set the position so it can't be seen
			kult::get<Component::Position>(*ghost).pos = glm::vec3(0, 0, -10000);
			int boardpos = WorldPositionToBoardPosition(finalPosition);
			if ((finalPosition.x <= 1 && finalPosition.x >= -1) && (finalPosition.y <= 1 && finalPosition.y >= -1) && !(board[0][boardpos] || board[1][boardpos]))
			{
				//If the ghost is in  a valid position, apply the position
				kult::get<Component::Position>(*ghost).pos = finalPosition;
			}
		}
		break;
	}
	case SDL_MOUSEBUTTONDOWN:
	{
		if (event.button.button == SDL_BUTTON_LEFT)
		{
			//Copy the ghost marker entity
			kult::entity e;
			kult::copy(e, *ghost);
			//Swap the current player turn
			ghostid = !ghostid;
			ghost = &marker[ghostid];
			//Update the board
			auto pos = kult::get<Component::Position>(e).pos;
			int boardpos = WorldPositionToBoardPosition(pos);
			board[ghostid][boardpos] = true;
		}
		break;
	}
	default:
		break;
	}
}

glm::vec3 Application::GetRayFromMouse(glm::vec2 mouse, kult::entity camera)
{
	auto screenSize = GetScreenSize();
	float mouseX = mouse.x / (screenSize.x  * 0.5f) - 1.0f;
	float mouseY = mouse.y / (screenSize.y * 0.5f) - 1.0f;
	auto& cameraPositionData = kult::get<Component::Position>(camera);

	glm::mat4 proj = glm::perspective(glm::radians(fov), (glm::f32)screenSize.x / screenSize.y, near, far);
	glm::vec3 lookDirection = cameraPositionData.rot * glm::vec3(1, 0, 0);
	glm::vec3 up = cameraPositionData.rot * glm::vec3(0, 0, 1);
	glm::mat4 view = glm::lookAt(cameraPositionData.pos, cameraPositionData.pos + lookDirection, up);

	glm::mat4 invVP = glm::inverse(proj * view);
	glm::vec4 screenPos = glm::vec4(mouseX, -mouseY, 1.0f, 1.0f);
	glm::vec4 worldPos = invVP * screenPos;

	glm::vec3 dir = glm::normalize(glm::vec3(worldPos));
	return dir;
}

int Application::WorldPositionToBoardPosition(glm::vec3 world)
{
	return (world.x + 1) * 3 + (world.y + 1);
}


void Application::HandleEvent(SDL_Event& event)
{
	PlacementLogic(event);
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
	Systems::Render(shader, camera, GetScreenSize(), fov, near, far);
}
