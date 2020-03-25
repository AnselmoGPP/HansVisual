#ifndef HANSVISUAL_HPP
#define HANSVISUAL_HPP

#include <vector>
#include "layer.hpp"
#include "plotter.hpp"

class HansVisual
{
    std::vector<layer> layersSet;
    plotter display;

    layer emptyLayer;

    std::vector<size_t> find_layers(std::string layer_name);        // Used in clear_layer()

public:
    HansVisual();

    // Layer system ----------------------------------------------

    // Get a reference to the first layer called "layer_name". If none is found, a reference to a layer "none" is returned
    layer & lay(std::string layer_name);

    // Fill a vector<string> with the names of all layers
    void get_layer_names(std::vector<std::string> &list);

    // Add a new layer to layerSet (only before calling open_window()). Indicate the element type (points, lines, triangles, cubes), name and capacity (number of elements that layer supports)
    void add_layer(const char *name, object_type type, unsigned int capacity);

    // Empty the contents from a layer
    int clear_layer(std::string layer_name);

    // Create a layer with a grid (or modify it if already exists). Parameters: Size of one cell's side, number of cells per side, and HSV color.
    void draw_grid(float cell_size, unsigned int grid_size, float R, float G, float B);

    // Draw axis XYZ. Two systems: OpenGL (true), automotive (false)
    void draw_axis(float length, bool system);

    // Plotter system --------------------------------------------

    // Public GUI method. Publish data in the "data window". Send a pointer to an array of 10 std::strings. The empty strings (="") won't be published.
    void fill_data_window(const std::string *data_strings, int num_strings);

    // Create a window and open a new thread that runs the visualizer
    int open_window();

    // Check whether the window is open
    bool window_is_open();

    // Close the window
    void close_window();

    // Waits until the display is closed
    void wait();

};

#endif
