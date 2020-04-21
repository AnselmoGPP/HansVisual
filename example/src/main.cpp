#include "header.hpp"

//#define DELETE 1

int main()
{
    fill_buffers();

    HansVisual display;
    //HansVisual display_1;
    //HansVisual display_2(display_1);
    //display = display_2;

    // Generate the layers you will see
    display.allow_points_selection();
    display.draw_grid(6, 41, 0.0f, 0.0f, 0.4f);
    display.draw_axis(10, true);
    //display.draw_sphere_center(1);

    display.add_layer("Points 1",    points, 20);

    display.add_layer("Points 2",    points, 256);
    display.add_layer("Points 3",    points, 20);
    display.add_layer("Lines 1",     lines, 150);
    display.add_layer("Lines 2",     lines, 150);
    display.add_layer("Lines 3",     lines, 150);
    display.add_layer("Triangles 1", triangles, 50);
    display.add_layer("Triangles 2", triangles, 50);
    display.add_layer("Triangles 3", triangles, 50);
    display.add_layer("Cubes 1",     cubes, 50);
    display.add_layer("Cubes 2",     cubes, 50);
    display.add_layer("Cubes 3",     cubes, 50);

    // Show the visualizer's window
    display.open_window();

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // ----- Send points -----------------------------------------------------------

    // No mode (uses default color or color specified by the user) (white points)
      toolKit::transform_coordinates(pnts, 12);                                                     // Transform the points coordinates in a buffer from automotive system to OpenGL system
    display.lay("Points 1").send_points(12, pnts);                                                  // Paint some points in default color (white)
    display.lay("Points 1").send_points(12, pnts, 1., 1., 1.);                                      // Specify the color of the points
    display.lay("Points 1").send_points(12, pnts, 1., 1., 1., points_names);                        // Add an additional buffer with strings containing info associated to each point (shown when selecting points)

    // Mode "categories" (uses layer's palette) (12 points with 10 colors from palette, 3 points with same color)
      display.lay("Points 1").send_palette_HSV(points_colors_HSV, 12);                              // Change the standard palette of a certain layer. Here, we enter the palette in HSV format
    display.lay("Points 1").send_points_categories(12, pnts, points_categories);                    // Paint points using the standard palette and an array of labels for each point (useful for clustering/segmentation)
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Points 1").send_points_categories(12, pnts, points_categories, points_names);      // Same functionality as the previous funcion call, but this time we pass a string with data used in point selection

    // Mode "colors" (user defines colors)
      toolKit::convert_HSVtoRGB(points_colors_HSV_2, 12);                                           // Convert an array of colors in HSV system to RGB system, so you can use it in the next function call
    display.lay("Points 2").send_points_colors(12, pnts, points_colors_RGB);                        // (3 RGB colors) Paint the points and provide an array with the color of each one in RGB system
    display.lay("Points 2").send_points_colors(12, pnts, points_colors_HSV_2, points_names);        // (3 RGB colors)
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Points 2").send_points_colors(256, points_line, modified_rainbow);                 // (rainbow points)

    // Mode "gradient" (color depends on the gradient assigned to each point)
      display.lay("Points 3").send_palette_RGB(points_gradient_palette, 21);                            // Change the standard palette of a certain layer. Enter the palette in RGB format (0-255)
      display.lay("Points 3").send_palette_RGB_01(points_gradient_palette, 21);                         // Change the standard palette of a certain layer. Enter the palette in RGB_01 format (0-1)
    display.lay("Points 3").send_points_gradients(12, pnts, points_gradients, 6, 7);                    // (half black, half blue points) Paint points and provide an array with labels for each color. A point's color will depend on the label and range
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Points 3").send_points_gradients(12, pnts, points_gradients, 1, 12, points_names);     // (black-blue gradient) Same functionality as the previous function call, but now the range is bigger (and we added strings for point selection)

    // ----- Send lines ------------------------------------------------------------

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // No mode (white cube)
    display.lay("Lines 1").send_lines(12, linesCube);                                                   // (White points)
    display.lay("Lines 1").send_lines(12, linesCube, 1., 1., 1.);                                       // (White points)

    // Mode "categories"
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Lines 1").send_lines_categories(12, linesCube, lines_categories);                      // (box with 12 colors from palette)
    // display.lay("Lines 1").send_lines_categories(unsigned int number_lines, const float (*arr)[2][3], const unsigned int  *categories);

    // Mode "colors"
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Lines 2").send_lines_colors(12, linesCube, lines_colors);                              // (3 RGB colors box)
    // display.lay("Lines 2").send_lines_colors(unsigned int number_lines, const float (*arr)[2][3], const float (*colors)[3]);

    // Mode "gradient"
      display.lay("Lines 3").send_palette_RGB_01(parable_gradient_palette, 101);
    display.lay("Lines 3").send_lines_gradients(100, parable, parable_gradients, 0, 0);                 // (red parable)
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Lines 3").send_lines_gradients(100, parable, parable_gradients, 10, 60);               // (black to red parable)
    // display.lay("Lines 3").send_lines_gradients(unsigned int number_lines, const float (*arr)[2][3], const float *gradients, float min = 0, float max = 1);

    std::this_thread::sleep_for(std::chrono::seconds(1));

    // ----- Send triangles --------------------------------------------------------

    // No mode
    display.lay("Triangles 1").send_triangles(4, triangs);                                              // (White triangles)
    display.lay("Triangles 1").send_triangles(4, triangs, 1., 1., 1.);                                  // (White triangles)

    // Mode "categories"
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Triangles 1").send_triangles_categories(4, triangs, triangles_categories);             // (4 triangles using palette colors: 2 solid and 2 combinations of 3)
    // display.lay("Triangles 1").send_triangles_categories(unsigned int number_triangles, const float (*arr)[3][3], const unsigned int  *categories);

    // Mode "colors"
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Triangles 2").send_triangles_colors(4, triangs, triangles_colors);                     // (3 RGB triangles and 1 combining RGB)
    // display.lay("Triangles 2").send_triangles_colors(unsigned int number_triangles, const float (*arr)[3][3], const float (*colors)[3]);

    // Mode "gradient"
      display.lay("Triangles 3").send_palette_RGB_01(triangles_gradient_palette, 11);
    display.lay("Triangles 3").send_triangles_gradients(4, triangs, triangles_gradients, 0, 0);         // (1 black and 3 green triangles)
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Triangles 3").send_triangles_gradients(4, triangs, triangles_gradients, 0, 10);        // (black to green triangles)
    // display.lay("Triangles 3").send_triangles_gradients(unsigned int number_triangles, const float (*arr)[3][3], const float *gradients, float min = 0, float max = 1);

    std::this_thread::sleep_for(std::chrono::seconds(2));

    // ----- Send cubes ------------------------------------------------------------

    // No mode
    display.lay("Cubes 1").send_cubes(3, myCubes);											// (White boxes)
    display.lay("Cubes 1").send_cubes(3, myCubes, 1., 1., 1.);                              // (White boxes)

    // Mode "categories"
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Cubes 1").send_cubes_categories(3, myCubes, cubes_categories);             // (RGB boxes)

    // Mode "colors"
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Cubes 2").send_cubes_colors(3, myCubes, cubes_colors);                     // (3 boxes: Green, blue, green+blue)

    // Mode "gradient"
      display.lay("Cubes 3").send_palette_RGB_01(cubes_gradient_palette, 21);
    display.lay("Cubes 3").send_cubes_gradients(3, myCubes, cubes_gradients, 0, 0);
    std::this_thread::sleep_for(std::chrono::seconds(1));
    display.lay("Cubes 3").send_cubes_gradients(3, myCubes, cubes_gradients, 1, 12);        // (black to blue boxes)

    // ----- Delete layers this way ------------------------------------------------

    //display.delete_layer("Points 1");
    //display.delete_layer("Lines 2");
    //display.delete_layer("Triangles 3");
    //display.delete_layer("Cubes 1");

    // ----- Send data to the "data window" ----------------------------------------

    std::this_thread::sleep_for(std::chrono::seconds(2));

    std::string additional_data[10] =
    {
        "You can send data to this \"data window\"",
        "Just call the method \"fill_data_window\"",
        "Pass a pointer to an array of X strings to it",
        "The method will publish one string per line",
        "The empty strings (=\"\") won't be published",
        "",
        "",
        "",
        "",
        ""
    };

    display.fill_data_window(additional_data, 7);

    display.wait();
}
