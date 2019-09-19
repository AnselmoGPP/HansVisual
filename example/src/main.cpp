// http://csharphelper.com/blog/2015/12/build-a-geodesic-sphere-with-wpf-and-c/

#include "visualizerClass.hpp"
#include <cmath>
#include <chrono>

void test_1();
void test_2();

int main() {

    //test_1();		// Use the visualizer as a local object
    test_2();		// Use the visualizer as a member object.

}

// Test 1 ----------------------------------------------

struct buffers {

    buffers() {

		visualizerClass vis;


		// Fill pnts[] with an icosahedron
        vis.icosahedron(2, &pnts[0]);


        float box[9][3] = {
			-3,  3,  3,
			-3, -3,  3,
			 3, -3,  3,
			 3,  3,  3,
			 3,  3, -3,
			 3, -3, -3,
			-3, -3, -3,
			-3,  3, -3,
            -3,  3,  3
        };

		float parable[101][3];

		// Fill parable[][] with a parable
		vis.pol_3th_degree(&parable[0][0], -10, 10, 100, 0, 0, 1, 0);

		// Fill myLines[] with a parable and a "box"
		int i;
		for (i = 0; i < 101; i++) {
			myLines[i][0] = parable[i][0];
			myLines[i][1] = parable[i][1];
			myLines[i][2] = parable[i][2];
		}
		myLines[i][0] = 1.2f;				// This point signals a jump between lines
		myLines[i][1] = 3.4f;
		myLines[i][2] = 5.6f;
		i++;
        for (int j = 0; j < 9; j++) {
			myLines[i + j][0] = box[j][0];
			myLines[i + j][1] = box[j][1];
			myLines[i + j][2] = box[j][2];
		}
	}

	// Set layer's names and capacity
	std::string point_layer_names[3] = { "Points 1", "Points 2", "Points 3" };
	unsigned int max_points_per_layer[3] = { 20, 20, 20 };
	std::string line_layer_names[3] = { "Lines 1", "Lines 2", "Lines 3" };
	unsigned int max_lines_per_layer[3] = { 200, 200, 200 };
	std::string triangle_layer_names[3] = { "Triangles 1", "Triangles 2", "Triangles 3" };
	unsigned int max_triangles_per_layer[3] = { 50, 50, 50 };
	std::string cube_layer_names[3] = { "Cubes 1", "Cubes 2", "Cubes 3" };
	unsigned int max_cubes_per_layer[3] = { 3, 3, 3 };

	// Point buffers
	float pnts[12][3];						// Fill the icos[] array with points coordinates (in this case 12 points, with 3 coordinates each)
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
	20.0f,  1.0f, 1.0f,
	20.0f,  1.0f, 1.0f,
	20.0f,  1.0f, 1.0f,
	20.0f,  1.0f, 1.0f,
	100.0f, 1.0f, 1.0f,
	100.0f, 1.0f, 1.0f,
	100.0f, 1.0f, 1.0f,
	100.0f, 1.0f, 1.0f,
	210.0f, 1.0f, 1.0f,
	210.0f, 1.0f, 1.0f,
	210.0f, 1.0f, 1.0f,
	210.0f, 1.0f, 1.0f
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

	// Line buffers
    float myLines[101 + 1 + 9][3];		// Fill this array with the 2 lines we created previously + one line jump between them
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

void test_1() {

	buffers b;

	visualizerClass visual(
		3, b.point_layer_names, b.max_points_per_layer,
		3, b.line_layer_names, b.max_lines_per_layer,
		3, b.triangle_layer_names, b.max_triangles_per_layer,
		3, b.cube_layer_names, b.max_cubes_per_layer
	);

	visual.open_window();


	// Draw points ------------------------------

	visual.transform_coordinates(&b.pnts[0][0], 12);									// Transform the points coordinates in a buffer from automotive system to OpenGL system
	visual.send_palette_HSV("Points 1", points, &b.points_colors_HSV[0][0], 21);		// Change the standard palette of a certain layer. Enter the palette in HSV format
	visual.send_points("Points 1", 12, &b.pnts[0][0]);									// Paint some points in black
	visual.send_points("Points 1", 12, &b.pnts[0][0], b.points_categories, categories);	// Paint points using the standard palette and an array of labels for each point (useful for clustering/segmentation)
	visual.send_points("Points 1", 12, &b.pnts[0][0], b.points_categories);				// Same functionality as the previous funcion call (by default, it's interpreted as CATEGORIES)

	visual.send_points("Points 2", 12, &b.pnts[0][0], &b.points_colors_RGB[0][0], colors);		// Paint the points and provide an array with the color of each one in RGB system
	visual.convert_HSVtoRGB(&b.points_colors_HSV_2[0][0], 21);									// Convert an array of colors in HSV system to RGB system, so you can use it in the next send_points()
	visual.send_points("Points 2", 12, &b.pnts[0][0], &b.points_colors_HSV_2[0][0], colors);	// Paint points and provide an array with the color of each one in RGB system (if it's HSV, first convert it to RGB)

	visual.send_palette_HSV("Points 3", points, &b.points_gradient_palette[0][0], 21);		// Change the standard palette of a certain layer. Enter the palette in HSV format
	visual.send_palette_RGB("Points 3", points, &b.points_gradient_palette[0][0], 21);		// Change the standard palette of a certain layer. Enter the palette in RGB format (0-255)
	visual.send_palette_RGB_01("Points 3", points, &b.points_gradient_palette[0][0], 21);	// Change the standard palette of a certain layer. Enter the palette in RGB format (0-1)
	visual.send_points("Points 3", 12, &b.pnts[0][0], b.points_gradients, gradient, 6, 7);	// Paint points and provide an array with labels for each color. A point's color will depend on the label and range
	visual.send_points("Points 3", 12, &b.pnts[0][0], b.points_gradients, gradient, 1, 12);	// Same functionality as the previous function call, but now the range is bigger


	// Draw lines -------------------------------

    visual.send_lines("Lines 1", 111, &b.myLines[0][0]);
    visual.send_lines("Lines 1", 111, &b.myLines[0][0], &b.lines_categories[0], categories);        // Possible flags: CATEGORIES, COLORS, GRADIENT.
    visual.send_lines("Lines 1", 111, &b.myLines[0][0], &b.lines_categories[0]);                    // By default, interpreted as CATEGORIES

    visual.send_lines("Lines 2", 111, &b.myLines[0][0], &b.lines_colors[0][0], colors);

    visual.send_palette_RGB_01("Lines 3", lines, &b.lines_gradient_palette[0][0], 101);
    visual.send_lines("Lines 3", 111, &b.myLines[0][0], b.lines_gradients, gradient, 0, 0);
    visual.send_lines("Lines 3", 111, &b.myLines[0][0], b.lines_gradients, gradient, 10, 109);


	// Draw triangles ---------------------------

	visual.send_triangles("Triangles 1", 4, &b.triangles[0][0][0]);
	visual.send_triangles("Triangles 1", 4, &b.triangles[0][0][0], &b.triangles_categories[0], categories);		// Possible flags: CATEGORIES, COLORS, GRADIENT.
	visual.send_triangles("Triangles 1", 4, &b.triangles[0][0][0], &b.triangles_categories[0]);					// By default, interpreted as CATEGORIES

	visual.send_triangles("Triangles 2", 4, &b.triangles[0][0][0], &b.triangles_colors[0][0], colors);

	visual.send_palette_RGB_01("Triangles 3", triangles, &b.triangles_gradient_palette[0][0], 11);
	visual.send_triangles("Triangles 3", 4, &b.triangles[0][0][0], b.triangles_gradients, gradient, 0, 0);
	visual.send_triangles("Triangles 3", 4, &b.triangles[0][0][0], b.triangles_gradients, gradient, 0, 10);


	// Draw cubes -------------------------------

	visual.send_cubes("Cubes 1", 3, b.myCubes);											// Load boxes on the visualizer
	visual.send_cubes("Cubes 1", 3, b.myCubes, b.cubes_categories, categories);			// Possible flags: CATEGORIES, COLORS, GRADIENT.
	visual.send_cubes("Cubes 1", 3, b.myCubes, b.cubes_categories);						// By default, interpreted as CATEGORIES

	visual.send_cubes("Cubes 2", 3, b.myCubes, &b.cubes_colors[0][0], colors);

	visual.send_palette_RGB_01("Cubes 3", cubes, &b.cubes_gradient_palette[0][0], 21);
	visual.send_cubes("Cubes 3", 3, b.myCubes, b.cubes_gradients, gradient, 0, 0);
	visual.send_cubes("Cubes 3", 3, b.myCubes, b.cubes_gradients, gradient, 1, 12);


	// Send data to the "data window" --------------
	std::string additional_data[10] =
	{
		"You can send data to this \"data window\"",
		"You only have to call the method \"fill_data_window\"",
		"Pass a pointer to an array of X strings to it",
		"The method will publish one string per line",
		"The empty strings (=\"\") won't be published",
		"", "", "", "", ""
	};

	visual.fill_data_window(additional_data, 7);


	char abc;
	std::cin >> abc;
}

// Test 2 ----------------------------------------------

class myclass {
public:
    myclass()
	{
		visual = visualizerClass (
			3, b.point_layer_names, b.max_points_per_layer,
			3, b.line_layer_names, b.max_lines_per_layer,
			3, b.triangle_layer_names, b.max_triangles_per_layer,
			3, b.cube_layer_names, b.max_cubes_per_layer
		);
	};
	
	buffers b;

	visualizerClass visual;

	void send_data() {

		// Draw points ------------------------------

		visual.transform_coordinates(&b.pnts[0][0], 12);									// Transform the points coordinates in a buffer from automotive system to OpenGL system
		visual.send_palette_HSV("Points 1", points, &b.points_colors_HSV[0][0], 21);		// Change the standard palette of a certain layer. Enter the palette in HSV format
		visual.send_points("Points 1", 12, &b.pnts[0][0]);									// Paint some points in black
		visual.send_points("Points 1", 12, &b.pnts[0][0], b.points_categories, categories);	// Paint points using the standard palette and an array of labels for each point (useful for clustering/segmentation)
		visual.send_points("Points 1", 12, &b.pnts[0][0], b.points_categories);				// Same functionality as the previous funcion call (by default, it's interpreted as CATEGORIES)

		visual.send_points("Points 2", 12, &b.pnts[0][0], &b.points_colors_RGB[0][0], colors);		// Paint the points and provide an array with the color of each one in RGB system
		visual.convert_HSVtoRGB(&b.points_colors_HSV_2[0][0], 21);									// Convert an array of colors in HSV system to RGB system, so you can use it in the next send_points()
		visual.send_points("Points 2", 12, &b.pnts[0][0], &b.points_colors_HSV_2[0][0], colors);	// Paint points and provide an array with the color of each one in RGB system (if it's HSV, first convert it to RGB)

		visual.send_palette_HSV("Points 3", points, &b.points_gradient_palette[0][0], 21);		// Change the standard palette of a certain layer. Enter the palette in HSV format
		visual.send_palette_RGB("Points 3", points, &b.points_gradient_palette[0][0], 21);		// Change the standard palette of a certain layer. Enter the palette in RGB format (0-255)
		visual.send_palette_RGB_01("Points 3", points, &b.points_gradient_palette[0][0], 21);	// Change the standard palette of a certain layer. Enter the palette in RGB format (0-1)
		visual.send_points("Points 3", 12, &b.pnts[0][0], b.points_gradients, gradient, 6, 7);	// Paint points and provide an array with labels for each color. A point's color will depend on the label and range
		visual.send_points("Points 3", 12, &b.pnts[0][0], b.points_gradients, gradient, 1, 12);	// Same functionality as the previous function call, but now the range is bigger


		// Draw lines -------------------------------

		visual.send_lines("Lines 1", 111, &b.myLines[0][0]);
		visual.send_lines("Lines 1", 111, &b.myLines[0][0], &b.lines_categories[0], categories);        // Possible flags: CATEGORIES, COLORS, GRADIENT.
		visual.send_lines("Lines 1", 111, &b.myLines[0][0], &b.lines_categories[0]);                    // By default, interpreted as CATEGORIES

		visual.send_lines("Lines 2", 111, &b.myLines[0][0], &b.lines_colors[0][0], colors);

		visual.send_palette_RGB_01("Lines 3", lines, &b.lines_gradient_palette[0][0], 101);
		visual.send_lines("Lines 3", 111, &b.myLines[0][0], b.lines_gradients, gradient, 0, 0);
		visual.send_lines("Lines 3", 111, &b.myLines[0][0], b.lines_gradients, gradient, 10, 109);


		// Draw triangles ---------------------------

		visual.send_triangles("Triangles 1", 4, &b.triangles[0][0][0]);
		visual.send_triangles("Triangles 1", 4, &b.triangles[0][0][0], &b.triangles_categories[0], categories);		// Possible flags: CATEGORIES, COLORS, GRADIENT.
		visual.send_triangles("Triangles 1", 4, &b.triangles[0][0][0], &b.triangles_categories[0]);					// By default, interpreted as CATEGORIES

		visual.send_triangles("Triangles 2", 4, &b.triangles[0][0][0], &b.triangles_colors[0][0], colors);

		visual.send_palette_RGB_01("Triangles 3", triangles, &b.triangles_gradient_palette[0][0], 11);
		visual.send_triangles("Triangles 3", 4, &b.triangles[0][0][0], b.triangles_gradients, gradient, 0, 0);
		visual.send_triangles("Triangles 3", 4, &b.triangles[0][0][0], b.triangles_gradients, gradient, 0, 10);


		// Draw cubes -------------------------------

		visual.send_cubes("Cubes 1", 3, b.myCubes);											// Load boxes on the visualizer
		visual.send_cubes("Cubes 1", 3, b.myCubes, b.cubes_categories, categories);			// Possible flags: CATEGORIES, COLORS, GRADIENT.
		visual.send_cubes("Cubes 1", 3, b.myCubes, b.cubes_categories);						// By default, interpreted as CATEGORIES

		visual.send_cubes("Cubes 2", 3, b.myCubes, &b.cubes_colors[0][0], colors);

		visual.send_palette_RGB_01("Cubes 3", cubes, &b.cubes_gradient_palette[0][0], 21);
		visual.send_cubes("Cubes 3", 3, b.myCubes, b.cubes_gradients, gradient, 0, 0);
		visual.send_cubes("Cubes 3", 3, b.myCubes, b.cubes_gradients, gradient, 1, 12);


		// Send data to the "data window" --------------
		std::string additional_data[10] =
		{
			"You can send data to this \"data window\"",
			"You only have to call the method \"fill_data_window\"",
			"Pass a pointer to an array of X strings to it",
			"The method will publish one string per line",
			"The empty strings (=\"\") won't be published",
			"", "", "", "", ""
		};

		visual.fill_data_window(additional_data, 7);
	}
};

void test_2() {

	buffers dat;

    myclass obj;
	obj.visual.open_window();

	obj.send_data();

	obj.visual.add_layer(points, "New layer", 12);
	obj.visual.send_points("New layer", 12, &dat.pnts[0][0]);

	char abc;
	std::cin >> abc;
}
