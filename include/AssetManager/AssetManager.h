#pragma once
#include <string>
#include <map>
#include <fstream>
#include <vector>

#include "miniz.h"

namespace AssetManager
{
	template <typename T, typename U = void>
	class AssetManager
	{
	private:
		std::string assetPath;
		std::map<std::string, T*> assets;
		const U* userData;
		mz_zip_archive archive;
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
		memset(&archive, 0, sizeof(archive));
		mz_zip_reader_init_file(&archive, assetPath.c_str(), 0);
		mz_zip_error error = mz_zip_get_last_error(&archive);
		if (error)
		{
			throw std::runtime_error(mz_zip_get_error_string(error));
		}
	}

	template <typename T, typename U>
	AssetManager<T, U>::~AssetManager()
	{
		for (auto i : assets)
		{
			delete i.second;
		}
		mz_zip_reader_end(&archive);
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

		std::vector<unsigned char> filedata;
		mz_zip_archive_file_stat file_stat;

		int id = mz_zip_reader_locate_file(&archive, name.c_str(), nullptr, 0);
		mz_zip_reader_file_stat(&archive, id, &file_stat);
		
		filedata.resize(file_stat.m_uncomp_size);
		mz_zip_reader_extract_to_mem(&archive, file_stat.m_file_index, filedata.data(), filedata.capacity(), 0);

		if (!alreadyLoaded)
		{
			asset = new T(filedata.data(), filedata.size(), name, userData);
		}
		else
		{
			assets[name]->Load(filedata.data(), filedata.size(), name, userData);
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
