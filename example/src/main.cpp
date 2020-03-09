#include "header.hpp"

int main()
{
    fill_buffers();

    // Different ways of creating a visualizer object
    HansVisual display;
    //display.draw_grid(1, 30, 230, 1., 1.);

    // Add some layers to the visualizer
    display.add_layer("Points 1", points, 20);
    display.add_layer("Points 2", points, 256);
    display.add_layer("Points 3", points, 20);
    display.add_layer("Lines 1", lines, 150);
    display.add_layer("Lines 2", lines, 150);
    display.add_layer("Lines 3", lines, 150);
    display.add_layer("Triangles 1", triangles, 50);
    display.add_layer("Triangles 2", triangles, 50);
    display.add_layer("Triangles 3", triangles, 50);
    display.add_layer("Cubes 1", cubes, 50);
    display.add_layer("Cubes 2", cubes, 50);
    display.add_layer("Cubes 3", cubes, 50);

    // Show the visualizer's window
    display.open_window();

    // ----- Send points -----------------------------------------------------------

    // Ways of sending points in no mode (uses default color or color specified by the user)
    toolKit::transform_coordinates(pnts, 12);                                                   // Transform the points coordinates in a buffer from automotive system to OpenGL system

    display.send_points("Points 1", 12, pnts);                                                  // Paint some points in default color (white)
    display.send_points("Points 1", 12, pnts, 1., 1., 1.);                                      // Specify the color of the points
    display.send_points("Points 1", 12, pnts, 1., 1., 1., points_names);                        // Add an additional buffer with strings containing info associated to each point (shown when selecting points)

    // Ways of sending points in mode "categories" (uses layer's palette)
    display.send_palette_HSV("Points 1", points_colors_HSV, 12);                                // Change the standard palette of a certain layer. Here, we enter the palette in HSV format

    display.send_points_categories("Points 1", 12, pnts, points_categories);                    // Paint points using the standard palette and an array of labels for each point (useful for clustering/segmentation)
    display.send_points_categories("Points 1", 12, pnts, points_categories, points_names);      // Same functionality as the previous funcion call, but this time we pass a string with data used in point selection

    // Ways of sending points in mode "colors" (user defines colors)
    toolKit::convert_HSVtoRGB(points_colors_HSV_2, 12);                                         // Convert an array of colors in HSV system to RGB system, so you can use it in the next function call

    display.send_points_colors("Points 2", 12, pnts, points_colors_RGB);                        // Paint the points and provide an array with the color of each one in RGB system
    display.send_points_colors("Points 2", 12, pnts, points_colors_HSV_2, points_names);
    display.send_points_colors("Points 2", 256, points_line, modified_rainbow);

    // Ways of sending points in mode "gradient" (color depends on the gradient assigned to each point)
    display.send_palette_RGB("Points 3", points_gradient_palette, 21);                          // Change the standard palette of a certain layer. Enter the palette in RGB format (0-255)
    display.send_palette_RGB_01("Points 3", points_gradient_palette, 21);                       // Change the standard palette of a certain layer. Enter the palette in RGB_01 format (0-1)

    display.send_points_gradients("Points 3", 12, pnts, points_gradients, 6, 7);                // Paint points and provide an array with labels for each color. A point's color will depend on the label and range
    display.send_points_gradients("Points 3", 12, pnts, points_gradients, 1, 12, points_names); // Same functionality as the previous function call, but now the range is bigger (and we added strings for point selection)

    // ----- Send lines ------------------------------------------------------------

    // No mode
    display.send_lines("Lines 1", 12, linesCube);
    display.send_lines("Lines 1", 12, linesCube, 1., 1., 1.);

    // Mode "categories"
    display.send_lines_categories("Lines 1", 12, linesCube, lines_categories);

    // Mode "colors"
    display.send_lines_colors("Lines 2", 12, linesCube, lines_colors);

    // Mode "gradient"
    display.send_palette_RGB_01("Lines 3", parable_gradient_palette, 101);

    display.send_lines_gradients("Lines 3", 111, parable, parable_gradients, 0, 0);
    display.send_lines_gradients("Lines 3", 111, parable, parable_gradients, 10, 40);
/*
    // ----- Send triangles --------------------------------------------------------

    // No mode
    display.send_triangles("Triangles 1", 4, &triangs[0][0][0]);

    // Mode "categories"
    display.send_triangles("Triangles 1", 4, &triangs[0][0][0], &triangles_categories[0], categories);		// Possible flags: CATEGORIES, COLORS, GRADIENT.
    display.send_triangles("Triangles 1", 4, &triangs[0][0][0], &triangles_categories[0]);					// By default, interpreted as CATEGORIES

    // Mode "colors"
    display.send_triangles("Triangles 2", 4, &triangs[0][0][0], &triangles_colors[0][0], colors);

    // Mode "gradient"
        display.send_palette_RGB_01("Triangles 3", &triangles_gradient_palette[0][0], 11);
    display.send_triangles("Triangles 3", 4, &triangs[0][0][0], triangles_gradients, gradient, 0, 0);
    display.send_triangles("Triangles 3", 4, &triangs[0][0][0], triangles_gradients, gradient, 0, 10);

    // ----- Send cubes ------------------------------------------------------------

    // No mode
    display.send_cubes("Cubes 1", 3, myCubes);											// Load boxes on the visualizer

    // Mode "categories"
    display.send_cubes("Cubes 1", 3, myCubes, cubes_categories, categories);			// Possible flags: CATEGORIES, COLORS, GRADIENT.
    display.send_cubes("Cubes 1", 3, myCubes, cubes_categories);						// By default, interpreted as CATEGORIES

    // Mode "colors"
    display.send_cubes("Cubes 2", 3, myCubes, &cubes_colors[0][0], colors);

    // Mode "gradient"
        display.send_palette_RGB_01("Cubes 3", &cubes_gradient_palette[0][0], 21);
    display.send_cubes("Cubes 3", 3, myCubes, cubes_gradients, gradient, 0, 0);
    display.send_cubes("Cubes 3", 3, myCubes, cubes_gradients, gradient, 1, 12);
*/
    // ----- Send data to the "data window" ----------------------------------------

    std::string additional_data[10] =
    {
        "You can send data to this \"data window\"",
        "You only have to call the method \"fill_data_window\"",
        "Pass a pointer to an array of X strings to it",
        "The method will publish one string per line",
        "The empty strings (=\"\") won't be published",
        "", "", "", "", ""
    };

    display.fill_data_window(additional_data, 7);

/*
    for(;;)
    {
        // Why the addition stops at 32.0000? Why greater numbers grow faster (or smallers grow slower)?
        //std::this_thread::sleep_for(std::chrono::milliseconds(50));
        for(int j = 0; j < 12; j++) pnts[j][0] += 0.000001;
        display.send_points("Points 1", 12, &pnts[0][0], points_categories, points_names);
    }
*/

    display.wait();
}
