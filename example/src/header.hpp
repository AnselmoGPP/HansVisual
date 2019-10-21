
#include "visualizerClass.hpp"

struct buffers {

    buffers();
    // Set layer's names and capacity
    std::string point_layer_names[3] =          { "Points 1", "Points 2", "Points 3" };
    unsigned int max_points_per_layer[3] =      { 20, 256, 20 };
    std::string line_layer_names[3] =           { "Lines 1", "Lines 2", "Lines 3" };
    unsigned int max_lines_per_layer[3] =       { 200, 200, 200 };
    std::string triangle_layer_names[3] =       { "Triangles 1", "Triangles 2", "Triangles 3" };
    unsigned int max_triangles_per_layer[3] =   { 50, 50, 50 };
    std::string cube_layer_names[3] =           { "Cubes 1", "Cubes 2", "Cubes 3" };
    unsigned int max_cubes_per_layer[3] =       { 3, 3, 3 };

    // Point buffers
    float pnts[12][3];                              // Fill the icos[] array with points coordinates (in this case 12 points, with 3 coordinates each)
    float points_categories[12] = { 0,1,2,3,4,5,6,7,8,10,10,10 };
    float points_colors_HSV[12][3] = {
        20.0f,  1.0f, 1.0f,
        40.0f,  1.0f, 1.0f,
        60.0f,  1.0f, 1.0f,
        80.0f,  1.0f, 1.0f,
        100.0f, 1.0f, 1.0f,
        120.0f, 1.0f, 1.0f,
        140.0f, 1.0f, 1.0f,
        160.0f, 1.0f, 1.0f,
        180.0f, 1.0f, 1.0f,
        200.0f, 1.0f, 1.0f,
        220.0f, 1.0f, 1.0f,
        240.0f, 1.0f, 1.0f
    };
    float points_colors_HSV_2[12][3] = {
    255-28.0f,  1.0f, 1.0f,
    255-36.0f,  1.0f, 1.0f,
    255-36.0f,  1.0f, 1.0f,
    255-43.0f,  1.0f, 1.0f,
    255-43.0f, 1.0f, 1.0f,
    255-43.0f, 1.0f, 1.0f,
    255-84.0f, 1.0f, 1.0f,
    255-239.0f, 1.0f, 1.0f,
    255-239.0f, 1.0f, 1.0f,
    255-166.0f, 1.0f, 1.0f,
    255-169.0f, 1.0f, 1.0f,
    255-169.0f, 1.0f, 1.0f
    };
    float points_colors_RGB[12][3] = {
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f
    };
    float points_gradient_palette[21][3] = {
        0.0f, 0.0f, 0.00f,
        0.0f, 0.0f, 0.05f,
        0.0f, 0.0f, 0.10f,
        0.0f, 0.0f, 0.15f,
        0.0f, 0.0f, 0.20f,
        0.0f, 0.0f, 0.25f,
        0.0f, 0.0f, 0.30f,
        0.0f, 0.0f, 0.35f,
        0.0f, 0.0f, 0.40f,
        0.0f, 0.0f, 0.45f,
        0.0f, 0.0f, 0.50f,
        0.0f, 0.0f, 0.55f,
        0.0f, 0.0f, 0.60f,
        0.0f, 0.0f, 0.65f,
        0.0f, 0.0f, 0.70f,
        0.0f, 0.0f, 0.75f,
        0.0f, 0.0f, 0.80f,
        0.0f, 0.0f, 0.85f,
        0.0f, 0.0f, 0.90f,
        0.0f, 0.0f, 0.95f,
        0.0f, 0.0f, 1.00f
    };
    float points_gradients[12] = { 1,2,3,4,5,6,7,8,9,10,11,12 };

    float points_line[30][3] = {
        -1.5, 0., 0.,
        -1.4, 0., 0.,
        -1.3, 0., 0.,
        -1.2, 0., 0.,
        -1.1, 0., 0.,
        -1.0, 0., 0.,
        -0.9, 0., 0.,
        -0.8, 0., 0.,
        -0.7, 0., 0.,
        -0.6, 0., 0.,

        -0.5, 0., 0.,
        -0.4, 0., 0.,
        -0.3, 0., 0.,
        -0.2, 0., 0.,
        -0.1, 0., 0.,
        -0.0, 0., 0.,
         0.1, 0., 0.,
         0.2, 0., 0.,
         0.3, 0., 0.,
         0.4, 0., 0.,

         0.5, 0., 0.,
         0.6, 0., 0.,
         0.7, 0., 0.,
         0.8, 0., 0.,
         0.9, 0., 0.,
         1.0, 0., 0.,
         1.1, 0., 0.,
         1.2, 0., 0.,
         1.3, 0., 0.,
         1.4, 0., 0.
    };
    float points_colors_line_HSV[30][3] = {
         0.0f,  1.0f, 1.0f,
        10.0f,  1.0f, 1.0f,
        20.0f,  1.0f, 1.0f,
        30.0f,  1.0f, 1.0f,
        40.0f, 1.0f, 1.0f,
        50.0f, 1.0f, 1.0f,
        60.0f, 1.0f, 1.0f,
        70.0f, 1.0f, 1.0f,
        80.0f, 1.0f, 1.0f,
        90.0f, 1.0f, 1.0f,
        100.0f, 1.0f, 1.0f,
        110.0f, 1.0f, 1.0f,
        120.0f,  1.0f, 1.0f,
        130.0f,  1.0f, 1.0f,
        140.0f,  1.0f, 1.0f,
        150.0f,  1.0f, 1.0f,
        160.0f, 1.0f, 1.0f,
        170.0f, 1.0f, 1.0f,
        180.0f, 1.0f, 1.0f,
        190.0f, 1.0f, 1.0f,
        200.0f, 1.0f, 1.0f,
        210.0f, 1.0f, 1.0f,
        220.0f, 1.0f, 1.0f,
        230.0f, 1.0f, 1.0f,
        240.0f,  1.0f, 1.0f,
        250.0f,  1.0f, 1.0f,
        255.0f,  1.0f, 1.0f,
        255.0f,  1.0f, 1.0f,
        255.0f, 1.0f, 1.0f,
        255.0f, 1.0f, 1.0f
    };
    float points_line_2[256][3];
    std::string points_names[12] = { "Inda", "Dani", "Jesus", "Caderno", "Berges", "Xabi", "Parajua", "Aitor", "Anselmo", "Cecibel", "Oscar", "Miguel" };

    // Line buffers
    float myLines[101 + 1 + 9][3];                  // Fill this array with the 2 lines we created previously + one line jump between them
    float lines_categories[101 + 1 + 9 - 1] = {     // Send a category for each segment (number of segments = number of points - 1)
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        5, 5,       // << These two segments are the line jump. They are not going to be displayed on screen
        1, 1, 1, 1, 2, 2, 2, 2
    };
    float lines_colors[101 + 1 + 9 - 1][3] = {      // Send a color for each segment (number of segments = number of points - 1)
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,
        1.0f, 0.0f, 0.0,

        0.0f, 0.0f, 0.0,          // << This is the line jump
        0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    float lines_gradient_palette[101][3] = {
        0.00f, 0.0f, 0.0f,
        0.01f, 0.0f, 0.0f,
        0.02f, 0.0f, 0.0f,
        0.03f, 0.0f, 0.0f,
        0.04f, 0.0f, 0.0f,
        0.05f, 0.0f, 0.0f,
        0.06f, 0.0f, 0.0f,
        0.07f, 0.0f, 0.0f,
        0.08f, 0.0f, 0.0f,
        0.09f, 0.0f, 0.0f,

        0.10f, 0.0f, 0.0f,
        0.11f, 0.0f, 0.0f,
        0.12f, 0.0f, 0.0f,
        0.13f, 0.0f, 0.0f,
        0.14f, 0.0f, 0.0f,
        0.15f, 0.0f, 0.0f,
        0.16f, 0.0f, 0.0f,
        0.17f, 0.0f, 0.0f,
        0.18f, 0.0f, 0.0f,
        0.19f, 0.0f, 0.0f,

        0.20f, 0.0f, 0.0f,
        0.21f, 0.0f, 0.0f,
        0.22f, 0.0f, 0.0f,
        0.23f, 0.0f, 0.0f,
        0.24f, 0.0f, 0.0f,
        0.25f, 0.0f, 0.0f,
        0.26f, 0.0f, 0.0f,
        0.27f, 0.0f, 0.0f,
        0.28f, 0.0f, 0.0f,
        0.29f, 0.0f, 0.0f,

        0.30f, 0.0f, 0.0f,
        0.31f, 0.0f, 0.0f,
        0.32f, 0.0f, 0.0f,
        0.33f, 0.0f, 0.0f,
        0.34f, 0.0f, 0.0f,
        0.35f, 0.0f, 0.0f,
        0.36f, 0.0f, 0.0f,
        0.37f, 0.0f, 0.0f,
        0.38f, 0.0f, 0.0f,
        0.39f, 0.0f, 0.0f,

        0.40f, 0.0f, 0.0f,
        0.41f, 0.0f, 0.0f,
        0.42f, 0.0f, 0.0f,
        0.43f, 0.0f, 0.0f,
        0.44f, 0.0f, 0.0f,
        0.45f, 0.0f, 0.0f,
        0.46f, 0.0f, 0.0f,
        0.47f, 0.0f, 0.0f,
        0.48f, 0.0f, 0.0f,
        0.49f, 0.0f, 0.0f,

        0.50f, 0.0f, 0.0f,
        0.51f, 0.0f, 0.0f,
        0.52f, 0.0f, 0.0f,
        0.53f, 0.0f, 0.0f,
        0.54f, 0.0f, 0.0f,
        0.55f, 0.0f, 0.0f,
        0.56f, 0.0f, 0.0f,
        0.57f, 0.0f, 0.0f,
        0.58f, 0.0f, 0.0f,
        0.59f, 0.0f, 0.0f,

        0.60f, 0.0f, 0.0f,
        0.61f, 0.0f, 0.0f,
        0.62f, 0.0f, 0.0f,
        0.63f, 0.0f, 0.0f,
        0.64f, 0.0f, 0.0f,
        0.65f, 0.0f, 0.0f,
        0.66f, 0.0f, 0.0f,
        0.67f, 0.0f, 0.0f,
        0.68f, 0.0f, 0.0f,
        0.69f, 0.0f, 0.0f,

        0.70f, 0.0f, 0.0f,
        0.71f, 0.0f, 0.0f,
        0.72f, 0.0f, 0.0f,
        0.73f, 0.0f, 0.0f,
        0.74f, 0.0f, 0.0f,
        0.75f, 0.0f, 0.0f,
        0.76f, 0.0f, 0.0f,
        0.77f, 0.0f, 0.0f,
        0.78f, 0.0f, 0.0f,
        0.79f, 0.0f, 0.0f,

        0.80f, 0.0f, 0.0f,
        0.81f, 0.0f, 0.0f,
        0.82f, 0.0f, 0.0f,
        0.83f, 0.0f, 0.0f,
        0.84f, 0.0f, 0.0f,
        0.85f, 0.0f, 0.0f,
        0.86f, 0.0f, 0.0f,
        0.87f, 0.0f, 0.0f,
        0.88f, 0.0f, 0.0f,
        0.89f, 0.0f, 0.0f,

        0.90f, 0.0f, 0.0f,
        0.91f, 0.0f, 0.0f,
        0.92f, 0.0f, 0.0f,
        0.93f, 0.0f, 0.0f,
        0.94f, 0.0f, 0.0f,
        0.95f, 0.0f, 0.0f,
        0.96f, 0.0f, 0.0f,
        0.97f, 0.0f, 0.0f,
        0.98f, 0.0f, 0.0f,
        0.99f, 0.0f, 0.0f,

        1.00f, 0.0f, 0.0f
    };
    float lines_gradients[101 + 1 + 9 - 1] = {
        10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,
        60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,
        10,10,
        10,24,36,50,63,77,92,109
    };

    // Triangle buffers
    float triangles[4][3][3] = {
         4.0f,  4.0f,  0.5f,
         4.0f,  4.0f, -0.5f,
         3.5f,  4.5f,  0.0f,

        -4.0f,  4.0f,  0.5f,
        -4.0f,  4.0f, -0.5f,
        -3.5f,  4.5f,  0.0f,

         0.5f,  4.0f,  4.0f,
        -0.5f,  4.0f,  4.0f,
         0.0f,  4.5f,  3.5f,

         0.5f,  4.0f, -4.0f,
        -0.5f,  4.0f, -4.0f,
         0.0f,  4.5f, -3.5f
    };
    float triangles_categories[4] = { 0, 0, 1, 1 };
    float triangles_colors[4][3] = {
        1.0f, 0.0f, 0.0f,
        0.2f, 0.3f, 0.5f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };
    float triangles_gradient_palette[11][3] = {
        0.0f, 0.0f, 0.0f,
        0.0f, 0.1f, 0.0f,
        0.0f, 0.2f, 0.0f,
        0.0f, 0.3f, 0.0f,
        0.0f, 0.4f, 0.0f,
        0.0f, 0.5f, 0.0f,
        0.0f, 0.6f, 0.0f,
        0.0f, 0.7f, 0.0f,
        0.0f, 0.8f, 0.0f,
        0.0f, 0.9f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    float triangles_gradients[4] = { 0, 3, 6, 10 };

    // Cube buffers
    cube3D myCubes[3] = {
        cube3D(0, 0, 0, 1, 1, 1, 0.0*3.1415),
        cube3D(1, 1, 1, 2, 1, 0.5, 0.3*3.1415),
        cube3D(1, -1, 0, 1, 0.5, 0.1, 0.6*3.1415)
    };
    float cubes_categories[3] = { 0, 1, 2 };
    float cubes_colors[3][3] = {
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f
    };
    float cubes_gradient_palette[21][3] = {
        0.0f, 0.0f, 0.00f,
        0.0f, 0.0f, 0.05f,
        0.0f, 0.0f, 0.10f,
        0.0f, 0.0f, 0.15f,
        0.0f, 0.0f, 0.20f,
        0.0f, 0.0f, 0.25f,
        0.0f, 0.0f, 0.30f,
        0.0f, 0.0f, 0.35f,
        0.0f, 0.0f, 0.40f,
        0.0f, 0.0f, 0.45f,
        0.0f, 0.0f, 0.50f,
        0.0f, 0.0f, 0.55f,
        0.0f, 0.0f, 0.60f,
        0.0f, 0.0f, 0.65f,
        0.0f, 0.0f, 0.70f,
        0.0f, 0.0f, 0.75f,
        0.0f, 0.0f, 0.80f,
        0.0f, 0.0f, 0.85f,
        0.0f, 0.0f, 0.90f,
        0.0f, 0.0f, 0.95f,
        0.0f, 0.0f, 1.00f
    };
    float cubes_gradients[3] = { 1, 6, 12 };
};

class myclass {
public:
    myclass();

    buffers b;

    visualizerClass visual;

    void send_data();
};