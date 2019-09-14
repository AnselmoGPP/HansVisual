# cd /home/username/dev/visual/_BUILD
# cmake -G"Unix Makefiles" ..
# cmake -G"Visual Studio 15 2017 Win64" ..
# cmake -G"Visual Studio 15 2017 Win32" ..

CMAKE_MINIMUM_REQUIRED (VERSION 3.0)
PROJECT (hans_visualizer)

ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/glew/glew-2.1.0/build/cmake)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/glfw/glfw-3.3)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/extern/glm/glm-0.9.9.5)
ADD_SUBDIRECTORY(${CMAKE_CURRENT_SOURCE_DIR}/hans_visual)
