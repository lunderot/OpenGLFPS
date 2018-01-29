#include "AssetManager/Animation.h"

namespace AssetManager
{
	Animation::Animation(std::istream* buffer, const std::string& filename, const void* userData)
	{
		Load(buffer, filename, userData);
	}

	Animation::~Animation()
	{
		Unload();
	}

	void Animation::Load(std::istream* buffer, const std::string& filename, const void* userData)
	{
		//Read the number of bones and keyframes
		*buffer >> numBones >> numKeyframes;

		//Reserve and read the heirarchy list
		hierarchy.resize(numBones);
		for (auto& it: hierarchy)
			*buffer >> it;

		//Reserve space for all the keyframes and bones
		keyframes.resize(numKeyframes);
		for (auto& it : keyframes)
			it.resize(numBones);

		//Read keyframes
		for (int i = 0; i < numKeyframes; i++)
		{
			float time;
			*buffer >> time;
			for (unsigned j = 0; j < numBones; j++)
			{
				keyframes[i][j].time = time;
				*buffer >>
					keyframes[i][j].translation.x >>
					keyframes[i][j].translation.y >>
					keyframes[i][j].translation.z;
				*buffer >>
					keyframes[i][j].rotation.x >>
					keyframes[i][j].rotation.y >>
					keyframes[i][j].rotation.z >>
					keyframes[i][j].rotation.w;
				*buffer >>
					keyframes[i][j].scale.x >>
					keyframes[i][j].scale.y >>
					keyframes[i][j].scale.z;
			}
		}
	}

	void Animation::Unload()
	{
		keyframes.clear();
		hierarchy.clear();
	}

	const std::vector<std::vector<Animation::Keyframe>>& Animation::GetKeyframes() const
	{
		return keyframes;
	}

	const std::vector<int> Animation::GetHierarchy() const
	{
		return hierarchy;
	}

	unsigned Animation::GetNumBones() const
	{
		return numBones;
	}

	unsigned Animation::GetNumKeyframes() const
	{
		return numKeyframes;
	}
}
