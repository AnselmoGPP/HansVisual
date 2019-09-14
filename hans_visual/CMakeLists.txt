# cd /home/username/dev/visual/_BUILD
# cmake -G"Unix Makefiles" ..
# cmake -G"Visual Studio 15 2017 Win64" ..

cmake_minimum_required (VERSION 3.0)
project (hans_visual)

find_package(OpenGL REQUIRED)
#find_package(glfw3 3.3 REQUIRED)

MESSAGE(STATUS "OPENGL_LIBRARY: " ${OPENGL_LIBRARY})

# include_directories()
# add_subdirectory()

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)
ADD_COMPILE_DEFINITIONS(GLEW_STATIC=1)
ADD_COMPILE_DEFINITIONS(IMGUI_IMPL_OPENGL_LOADER_GLEW=1)

## visualizer_class -----------------------------------------------------------

ADD_LIBRARY(${PROJECT_NAME} STATIC
	src/main.cpp
	src/controls.hpp
	src/controls.cpp
	src/shader.hpp
	src/shader.cpp
	src/visualizerClass.hpp
	src/visualizerClass.cpp
	
	../extern/imgui/imgui-1.72b/imgui.cpp
	../extern/imgui/imgui-1.72b/imgui_demo.cpp
	../extern/imgui/imgui-1.72b/imgui_draw.cpp
	../extern/imgui/imgui-1.72b/imgui_widgets.cpp
	../extern/imgui/imgui-1.72b/examples/imgui_impl_glfw.cpp
	../extern/imgui/imgui-1.72b/examples/imgui_impl_opengl3.cpp
)

# Link the include files (.h)
target_include_directories( ${PROJECT_NAME} PUBLIC
	../extern/glfw/glfw-3.3/include
	../extern/glm/glm-0.9.9.5
	../extern/glew/glew-2.1.0/include
	../extern/imgui/imgui-1.72b
)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CURRENT_CMAKE_DIR}/bin)
