set(SDL_STATIC OFF)
add_subdirectory(SDL)
add_subdirectory(glew/build/cmake)

set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_UTILITY OFF)
set(ALSOFT_TEST OFF)
add_subdirectory(openal)