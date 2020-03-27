#ifndef OPTIONS_HPP
#define OPTIONS_HPP

// Plotter options -------------------------

// Background
#define BACKG_R 0.
#define BACKG_G 0.
#define BACKG_B 0.14

// Selection
#define SEL_R 0.97
#define SEL_G 0.
#define SEL_B 1.
#define SEL_A 0.

#define PNT_SIZE 35
#define MAX_PNT_SIZE 500.0f
#define DESIRED_FPS 60
#define POLYGON_MODE 0              // If true, shows only the borders of the triangles
#define VERSION_GLFW_for_IMGUI "#version 330"

// Controls options ------------------------

#define CAM_MODE    1               // 1: First person,  2: Sphere,  3: Plane  4: First person shooter
#define NEAR_CLIP_PLANE 0.1f
#define FAR_CLIP_PLANE 1000.0f
#define ASPECT_RATIO 16.f/9.f       // 4.0f/3.0f
#define ADAPTATIVE_ASPECT_RATIO 1

//Window options ---------------------------

#define ANTIALIASING 4              // x4 antialiasing
#define VERSION_GLFW 3

// Window & controls options ---------------

#define WINDOW_WIDTH    1920/2      // 1024
#define WINDOW_HEIGHT   1080/2      // 768

// Layer options ---------------------------

#define DEFAULT_RED 1
#define DEFAULT_GREEN 1
#define DEFAULT_BLUE 1

#endif
