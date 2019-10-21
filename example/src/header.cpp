
#include "header.hpp"


// myclass class ----------------------------------------------------------------------

myclass::myclass() {

    visual = visualizerClass (
        3, b.point_layer_names, b.max_points_per_layer,
        3, b.line_layer_names, b.max_lines_per_layer,
        3, b.triangle_layer_names, b.max_triangles_per_layer,
        3, b.cube_layer_names, b.max_cubes_per_layer
    );
};

void myclass::send_data() {

        // Draw points ------------------------------

        visual.transform_coordinates(&b.pnts[0][0], 12);                                                // Transform the points coordinates in a buffer from automotive system to OpenGL system
        visual.send_palette_HSV("Points 1", points, &b.points_colors_HSV[0][0], 12);                    // Change the standard palette of a certain layer. Enter the palette in HSV format
        visual.send_points("Points 1", 12, &b.pnts[0][0]);                                              // Paint some points in black
        visual.send_points("Points 1", 12, &b.pnts[0][0], b.points_categories, nullptr, categories);	// Paint points using the standard palette and an array of labels for each point (useful for clustering/segmentation)
        visual.send_points("Points 1", 12, &b.pnts[0][0], b.points_categories, b.points_names);        // Same functionality as the previous funcion call (by default, it's interpreted as CATEGORIES), but this time we introduced a string with data used in point selection

        visual.send_points("Points 2", 12, &b.pnts[0][0], &b.points_colors_RGB[0][0], nullptr, colors);		// Paint the points and provide an array with the color of each one in RGB system
        //visual.convert_HSVtoRGB(&b.points_colors_HSV_2[0][0], 12);									// Convert an array of colors in HSV system to RGB system, so you can use it in the next send_points()
        visual.send_points("Points 2", 12, &b.pnts[0][0], &b.points_colors_HSV_2[0][0], nullptr, colors);	// Paint points and provide an array with the color of each one in RGB system (if it's HSV, first convert it to RGB)
        visual.send_points("Points 2", 256, &b.points_line_2[0][0], &visual.modified_rainbow[0][0], nullptr, colors);

        //visual.send_palette_HSV("Points 3", points, &b.points_gradient_palette[0][0], 21);		// Change the standard palette of a certain layer. Enter the palette in HSV format
        visual.send_palette_RGB("Points 3", points, &b.points_gradient_palette[0][0], 21);		// Change the standard palette of a certain layer. Enter the palette in RGB format (0-255)
        visual.send_palette_RGB_01("Points 3", points, &b.points_gradient_palette[0][0], 21);	// Change the standard palette of a certain layer. Enter the palette in RGB format (0-1)
        visual.send_points("Points 3", 12, &b.pnts[0][0], b.points_gradients, nullptr, gradient, 6, 7);	// Paint points and provide an array with labels for each color. A point's color will depend on the label and range
        visual.send_points("Points 3", 12, &b.pnts[0][0], b.points_gradients, nullptr, gradient, 1, 12);	// Same functionality as the previous function call, but now the range is bigger


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

// buffers struct ----------------------------------------------------------------------

buffers::buffers() {

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

        // Fill array points_line_2
        for (int i = 0; i < 256; i++) {
            points_line_2[i][0] = i / 10.f;
            points_line_2[i][1] = 0;
            points_line_2[i][2] = 0;
        };
}
