#include "Application.h"

#include "systems/Physics.h"
#include "systems/Render.h"
#include "systems/Freelook.h"
#include "systems/Draglook.h"
#include "systems/Freemove.h"
#include "systems/Listener.h"
#include "systems/MenuRender.h"

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
	menuShader(shaderManager.Get("menu.shader")),
	audioPlayer("../data/audio.zip"),
	state(MENU)
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
	score[0] = 0;
	score[1] = 0;

	mask[0] = 0b111'000'000;
	mask[1] = 0b000'111'000;
	mask[2] = 0b000'000'111;
	mask[3] = 0b100'100'100;
	mask[4] = 0b010'010'010;
	mask[5] = 0b001'001'001;
	mask[6] = 0b100'010'001;
	mask[7] = 0b001'010'100;


	glm::vec3 menuOffset = glm::vec3(50, 250, 0);
	glm::vec3 menuCenterOffset = glm::vec3(250.0f / 2.0f, 80.0f / 2.0f, 0.0f);

	kult::add<Component::Position>(menuStart) = {
		glm::vec3(0, 80*0, 0) + menuOffset + menuCenterOffset,
		glm::quat(glm::vec3(0, 0, 0)),
		glm::vec3(250, 80, 1)
	};
	kult::add<Component::MenuRender>(menuStart) = {
		meshManager.Get("quad.obj"),
		textureManager.Get("start.rgba"),
		false
	};

	kult::add<Component::Position>(menuOptions) = {
		glm::vec3(0, 80*1, 0) + menuOffset + menuCenterOffset,
		glm::quat(glm::vec3(0, 0, 0)),
		glm::vec3(250, 80, 1)
	};
	kult::add<Component::MenuRender>(menuOptions) = {
		meshManager.Get("quad.obj"),
		textureManager.Get("options.rgba"),
		false
	};

	kult::add<Component::Position>(menuExit) = {
		glm::vec3(0, 80*2, 0) + menuOffset + menuCenterOffset,
		glm::quat(glm::vec3(0, 0, 0)),
		glm::vec3(250, 80, 1)
	};
	kult::add<Component::MenuRender>(menuExit) = {
		meshManager.Get("quad.obj"),
		textureManager.Get("exit.rgba"),
		false
	};
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
			kult::add<kult::component<'mark', bool>>(e);

			//Update the board
			auto pos = kult::get<Component::Position>(e).pos;
			int boardpos = WorldPositionToBoardPosition(pos);
			board[ghostid][boardpos] = true;

			//Swap the current player turn
			ghostid = !ghostid;
			ghost = &marker[ghostid];

			CalculateScore();
			std::cout << "score[0] = " << score[0] << std::endl;
			std::cout << "score[1] = " << score[1] << std::endl;
		}
		break;
	}
	case SDL_KEYDOWN:
	{
		if (event.key.keysym.sym == SDLK_0)
		{
			ResetBoard();
		}
		break;
	}
	default:
		break;
	}
}

void Application::CalculateScore()
{
	for (size_t i = 0; i < 2; i++)
	{
		score[i] = 0;
		for (size_t j = 0; j < 8; j++)
		{
			if ((board[i].to_ulong() & mask[j]) == mask[j])
			{
				score[i]++;
			}
		}
	}
}

void Application::ResetBoard()
{
	for (auto &id : kult::join<kult::component<'mark', bool>>())
	{
		kult::purge(id);
	}
	board[0].reset();
	board[1].reset();
	kult::get<Component::Position>(marker[0]).pos = glm::vec3(0, 0, -10000);
	kult::get<Component::Position>(marker[1]).pos = glm::vec3(0, 0, -10000);
	ghost = &marker[0];
	ghostid = false;
	score[0] = 0;
	score[1] = 0;
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

	switch (state)
	{
	case Application::MENU:
		Systems::MenuRender(menuShader, GetScreenSize(), static_cast<bool>(configManager.Get("iMenuDebug")->GetInt()));
	case Application::GAME:
		Systems::Render(shader, camera, GetScreenSize(), fov, near, far);
		break;
	default:
		break;
	}
}
