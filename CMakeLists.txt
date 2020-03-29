cmake_minimum_required(VERSION 3.8)
project(OpenGLFPS)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
include_directories(external/glew/include)
include_directories(external/openal/include)
include_directories(external/stb)
include_directories(external/kult)
include_directories(external/miniz)
include_directories(external/glm/glm)

add_executable(
	main
	
	src/Application.cpp
	src/AssetManager
	src/main.cpp
	src/System.cpp
	src/AudioPlayer.cpp
	
	src/AssetManager/Config.cpp
	src/AssetManager/Mesh.cpp
	src/AssetManager/Scene.cpp
	src/AssetManager/Shader.cpp
	src/AssetManager/Texture.cpp
	src/AssetManager/Audio.cpp
	
	src/systems/Freelook.cpp
	src/systems/Freemove.cpp
	src/systems/Physics.cpp
	src/systems/Render.cpp
	src/systems/Listener.cpp
	
	external/miniz/miniz.c
	
	include/Application.h
	include/AssetManager
	include/AudioPlayer.h
	include/components
	include/System.h
	include/systems
	include/AssetManager/AssetManager.h
	include/AssetManager/Audio.h
	include/AssetManager/Config.h
	include/AssetManager/Mesh.h
	include/AssetManager/Scene.h
	include/AssetManager/Shader.h
	include/AssetManager/Texture.h
	include/AssetManager/Util.h
	include/components/Freelook.h
	include/components/Freemove.h
	include/components/Light.h
	include/components/Listener.h
	include/components/Physics.h
	include/components/Position.h
	include/components/Render.h
	include/systems/Freelook.h
	include/systems/Freemove.h
	include/systems/Listener.h
	include/systems/Physics.h
	include/systems/Render.h
)

set_property(TARGET main PROPERTY CXX_STANDARD 11)
target_compile_options(main PRIVATE -Wall)

# glew
add_definitions(-DGLEW_STATIC)
add_subdirectory(external/glew EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE libglew_static)

# glm
add_subdirectory(external/glm EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE glm)

# SDL
add_subdirectory(external/SDL EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE mingw32)
target_link_libraries(main PRIVATE SDL2main)
target_link_libraries(main PRIVATE SDL2)

# OpenAL
OPTION(ALSOFT_EXAMPLES "al-examples" OFF)
OPTION(ALSOFT_UTILITY "al-util" OFF)
OPTION(ALSOFT_TEST "al-test" OFF)
add_subdirectory(external/openal EXCLUDE_FROM_ALL)
target_link_libraries(main PRIVATE OpenAL)
