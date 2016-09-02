#pragma once
#include <string>
#include <map>
#include <fstream>

namespace AssetManager
{

	template <typename T, typename U = void>
	class AssetManager
	{
	private:
		std::string assetPath;
		std::map<std::string, T*> assets;
		const U* userData;
	public:
		AssetManager(const std::string& assetPath, const U* userData = nullptr);
		~AssetManager();

		T* Get(const std::string& name);
		void Reload(const std::string& name);
		void ReloadAll();
	private:
		T* Load(const std::string& name, bool alreadyLoaded = false);
	};

	template <typename T, typename U>
	AssetManager<T, U>::AssetManager(const std::string& assetPath, const U* userData) :
		assetPath(assetPath),
		userData(userData)
	{
	}

	template <typename T, typename U>
	AssetManager<T, U>::~AssetManager()
	{
		for (auto i : assets)
		{
			delete i.second;
		}
	}

	template <typename T, typename U>
	T* AssetManager<T, U>::Get(const std::string& name)
	{
		T* asset = nullptr;
		typename std::map<std::string, T*>::iterator it = assets.find(name);
		if (it != assets.end())
		{
			//Return the asset if it is already loaded
			asset = it->second;
		}
		else
		{
			//Load and return the asset if it is not already loaded
			asset = assets[name] = Load(name);
		}
		return asset;
	}

	template <typename T, typename U>
	T* AssetManager<T, U>::Load(const std::string& name, bool alreadyLoaded)
	{
		T* asset = nullptr;
		std::ifstream inputFile(assetPath + name, std::ios::binary);
		if (inputFile.good())
		{
			//Extract only the filename after the forward slash
			std::size_t position = name.find_last_of('/');
			std::string filename;
			if (position == std::string::npos)
			{
				filename = name;
			}
			else
			{
				filename = name.substr(position + 1);
			}
			if (!alreadyLoaded)
			{
				asset = new T(&inputFile, filename, userData);
			}
			else
			{
				assets[name]->Load(&inputFile, filename, userData);
			}
		}
		else
		{
			throw std::runtime_error("Failed to open asset: " + name);
		}
		return asset;
	}

	template <typename T, typename U>
	void AssetManager<T, U>::Reload(const std::string& name)
	{
		assets[name]->Unload();
		Load(name, true);
	}

	template <typename T, typename U>
	void AssetManager<T, U>::ReloadAll()
	{
		for (auto i : assets)
		{
			assets[i.first]->Unload();
			Load(i.first, true);
		}
	}
}
