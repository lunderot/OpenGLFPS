#pragma once

#include "System.h"
#include "Shader.h"
#include "AssetManager.h"
#include "Mesh.h"
#include "Texture.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <kult.hpp>

class Application: public System
{
private:
	AssetManager<Mesh> meshManager;
	AssetManager<Shader> shaderManager;
	AssetManager<Texture> textureManager;

	Shader* shader;

	kult::entity camera;
	kult::entity cube;
	kult::entity tree;
	kult::entity floor;
public:
	Application(glm::uvec2 screenSize, const std::string& title, int argc, char* argv[]);
	virtual ~Application();

	void HandleEvent(SDL_Event& event);
	void Update(float deltaTime);
	void Render();

	template <unsigned numPoints>
	glm::vec3 GetBezierPoint(glm::vec3* points, float t)
	{
		glm::vec3 tmp[numPoints];
		memcpy(tmp, points, numPoints * sizeof(glm::vec3));
		int i = numPoints - 1;
		while (i > 0) {
			for (int k = 0; k < i; k++)
				tmp[k] = tmp[k] + t * (tmp[k + 1] - tmp[k]);
			i--;
		}
		return tmp[0];
	}
};

