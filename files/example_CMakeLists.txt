#https://stackoverflow.com/questions/22598208/linking-opencv-libraries-included-as-an-external-project-via-cmake
#https://cmake.org/cmake/help/v3.0/command/find_package.html

CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

PROJECT(ocv_test)

FIND_PACKAGE(OpenCV REQUIRED PATHS ${CMAKE_CURRENT_SOURCE_DIR}/../opencv-4.1.1/_BUILD)

ADD_EXECUTABLE( ${PROJECT_NAME}
	src/main.cpp
	src/header.cpp
)

SET_PROPERTY(
	TARGET ${PROJECT_NAME} 
	PROPERTY CXX_STANDARD 14
)

TARGET_INCLUDE_DIRECTORIES( ${PROJECT_NAME} PUBLIC
	include
	#${OpenCV_INCLUDE_DIRS}
)

TARGET_LINK_LIBRARIES( ${PROJECT_NAME} PUBLIC
	${OpenCV_LIBS}
)

