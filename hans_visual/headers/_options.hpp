#ifndef OPTIONS_HPP
#define OPTIONS_HPP

// HansVisual ------------------------------

#define SELECTION_SQUARE_LAYER_NAME "Sel. square"
#define SELECTED_POINTS_LAYER_NAME "Selections"

// Plotter options -------------------------

#define BACKG_R 0.
#define BACKG_G 0.
#define BACKG_B 0.14
#define PNT_SIZE 35
#define MAX_PNT_SIZE 500.0f
#define DESIRED_FPS 60
#define POLYGON_MODE 0              // If true, shows only the borders of the triangles
#define CULL_FACES false

// Selection -------------------------------

#define SEL_R 1.
#define SEL_G 1.
#define SEL_B 0.
#define SEL_A 1.
#define MIN_DISTANCE 0.01
#define STRINGS_EXTRACTION true

// GUI -------------------------------------

#define VERSION_GLFW_for_IMGUI "#version 330"
#define SHOW_CHECKBOXES false
#define SHOW_DATA false
#define SHOW_OPTIONS false

// Camera options --------------------------

#define CAM_MODE    1               // 0: First person,  1: Sphere,  2: Plane  3: First person shooter  4: Zenith  5: Zenith sphere
#define NEAR_CLIP_PLANE 0.1f
#define FAR_CLIP_PLANE 1000.0f
    // Sphere
    #define ENABLE_SCROLL_SPEEDUP true
    #define SCROLL_SPEEDUP 0.1f

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
