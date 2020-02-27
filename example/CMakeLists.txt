# cd home/username/dev/OGL/Shaper/src
# cmake -G"Unix Makefiles" ..

CMAKE_MINIMUM_REQUIRED(VERSION 3.12)
PROJECT (Shaper)

FIND_PACKAGE(OpenGL REQUIRED)

add_definitions(
	-std=c++11
)

ADD_EXECUTABLE(${PROJECT_NAME}
	src/main.cpp
	src/header.cpp
	src/header.hpp
)

MESSAGE(STATUS "Project source dir: " ${PROJECT_SOURCE_DIR})
MESSAGE(STATUS "OpenGL library: " ${OPENGL_LIBRARY})

# Includes (.h) ----------------------------------------

TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC
	${PROJECT_SOURCE_DIR}/../hans_visual/src
	${PROJECT_SOURCE_DIR}/../extern/glfw/glfw-3.3/include
	${PROJECT_SOURCE_DIR}/../extern/glew/glew-2.1.0/include
	${PROJECT_SOURCE_DIR}/../extern/glm/glm-0.9.9.5
	${PROJECT_SOURCE_DIR}/../extern/imgui/imgui-1.72b
)	

# Libraries (.a, .lib) ---------------------------------

if( WIN32 )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME} 
		optimized	${OPENGL_LIBRARY} 
		optimized	${PROJECT_SOURCE_DIR}/../_BUILD/hans_visual/Release/hans_visual.lib
		optimized	${PROJECT_SOURCE_DIR}/../_BUILD/extern/glfw/glfw-3.3/src/Release/glfw3.lib
		optimized	${PROJECT_SOURCE_DIR}/../_BUILD/lib/Release/libglew32.lib
		optimized	${PROJECT_SOURCE_DIR}/../_BUILD/extern/glm/glm-0.9.9.5/glm/Release/glm_static.lib
	)

	TARGET_LINK_LIBRARIES( ${PROJECT_NAME} 
		debug		${OPENGL_LIBRARY} 
		debug		${PROJECT_SOURCE_DIR}/../_BUILD/hans_visual/Debug/hans_visual.lib
		debug		${PROJECT_SOURCE_DIR}/../_BUILD/extern/glfw/glfw-3.3/src/Debug/glfw3.lib
		debug		${PROJECT_SOURCE_DIR}/../_BUILD/lib/Debug/libglew32d.lib
		debug		${PROJECT_SOURCE_DIR}/../_BUILD/extern/glm/glm-0.9.9.5/glm/Debug/glm_static.lib
	)
endif()

if( UNIX )
	TARGET_LINK_LIBRARIES( ${PROJECT_NAME} 
		optimized	${OPENGL_LIBRARY} 
		optimized	${PROJECT_SOURCE_DIR}/../_BUILD/hans_visual/libhans_visual.a
		optimized	${PROJECT_SOURCE_DIR}/../_BUILD/extern/glfw/glfw-3.3/src/libglfw3.a
		optimized	${PROJECT_SOURCE_DIR}/../_BUILD/lib/libGLEW.a
		optimized	${PROJECT_SOURCE_DIR}/../_BUILD/extern/glm/glm-0.9.9.5/glm/libglm_static.a
		-lGL -lGLU -lXrandr -lXext -lX11 -lrt -ldl -lpthread -lm
	)

	# Uncomment this if you compile the hans_visual lib and its helper libs in debug mode. Then, link your app to these debug libs modifiying the paths below
	#TARGET_LINK_LIBRARIES( ${PROJECT_NAME} 
	#	debug		${OPENGL_LIBRARY} 
	#	optimized	${PROJECT_SOURCE_DIR}/../_BUILD/hans_visual/libhans_visual.a
	#	optimized	${PROJECT_SOURCE_DIR}/../_BUILD/extern/glfw/glfw-3.3/src/libglfw3.a
	#	optimized	${PROJECT_SOURCE_DIR}/../_BUILD/lib/libGLEW.a
	#	optimized	${PROJECT_SOURCE_DIR}/../_BUILD/extern/glm/glm-0.9.9.5/glm/libglm_static.a
	#)
endif()

