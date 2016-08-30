all:
	g++ src/*.cpp src/AssetManager/*.cpp src/systems/*.cpp \
	-I include \
	-I external/glew-2.0.0/include \
	-I external/kult \
	-I external/glm \
	-I external/SDL2-2.0.4/include \
	-lSDL2 \
	-lGLEW -lGLU -lGL \
	-std=c++11 \
	-w \
	-o OpenGLFPS
