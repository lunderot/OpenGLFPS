cmake_minimum_required(VERSION 3.8)
cmake_policy(VERSION 2.6)
project(OpenGLFPS)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
find_package(OpenGL REQUIRED)

add_subdirectory(external)

set(CMAKE_CXX_STANDARD 11)

include_directories(
	include
	"${PROJECT_SOURCE_DIR}/external/glew/include"
	"${PROJECT_SOURCE_DIR}/external/glm"
	"${PROJECT_SOURCE_DIR}/external/kult"
	"${PROJECT_SOURCE_DIR}/external/SDL/include"
	"${PROJECT_SOURCE_DIR}/external/openal/include"
	"${PROJECT_SOURCE_DIR}/external/stb"
	"${PROJECT_SOURCE_DIR}/external/miniz"
	)

add_executable(
	OpenGLFPS
	
	src/Application.cpp
	src/AssetManager
	src/main.cpp
	src/System.cpp
	src/AudioPlayer.cpp
	src/systems
	
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
)

target_link_libraries(
	OpenGLFPS
	
	${OPENGL_LIBRARIES}
	glew
	SDL2main
	SDL2
	OpenAL32
)
