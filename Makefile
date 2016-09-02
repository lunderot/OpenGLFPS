all:
	g++ src/*.cpp src/AssetManager/*.cpp src/systems/*.cpp \
	-I include \
	-I external/kult \
	-I external/glm \
	-lSDL2 \
	-lGLEW -lGLU -lGL \
	-std=c++11 \
	-w \
	-o OpenGLFPS
