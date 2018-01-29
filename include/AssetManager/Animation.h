#pragma once
#include <string>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace AssetManager
{
	class Animation
	{
	public:
		struct Keyframe
		{
			float time;
			glm::vec3 translation;
			glm::quat rotation;
			glm::vec3 scale;
		};
	private:
		unsigned numBones;
		unsigned numKeyframes;
		std::vector<std::vector<Keyframe>> keyframes;
		std::vector<int> hierarchy;
	public:
		Animation(std::istream* buffer, const std::string& filename, const void* userData);
		~Animation();

		void Load(std::istream* buffer, const std::string& filename, const void* userData);
		void Unload();

		const std::vector<std::vector<Keyframe>>& GetKeyframes() const;
		const std::vector<int> Animation::GetHierarchy() const;
		unsigned GetNumBones() const;
		unsigned GetNumKeyframes() const;
	};
}
