#include <iostream>
#include <thread>

#include "HansVisual.hpp"

HansVisual::HansVisual() : display(&layersSet, &layerSetMutex) { }

HansVisual::HansVisual(const HansVisual &obj) : layersSet(obj.layersSet), display(&layersSet, &layerSetMutex)
{
    layersSet = obj.layersSet;
}

HansVisual& HansVisual::operator=(const HansVisual &obj)
{
    layersSet = obj.layersSet;

    return *this;
}

// Layer system ---------------------------------------

layer & HansVisual::lay(std::string layer_name)
{
    for(size_t i = 0; i < layersSet.size(); ++i)
        if(layersSet[i].layer_name == layer_name) return layersSet[i];

    std::cout << "Layer \"" << layer_name << "\" was not found" << std::endl;
    return emptyLayer;
}

void HansVisual::get_layer_names(std::vector<std::string> &list)
{
    for(layer lay : layersSet) list.push_back(lay.layer_name);
}

void HansVisual::add_layer(const char *name, object_type type, unsigned int capacity)
{
    // Check reserved layer names
    std::string l_name;
    if(l_name == SELECTION_SQUARE_LAYER_NAME || l_name == SELECTED_POINTS_LAYER_NAME)
    {
        std::cout << "Not valid name for a layer (reserved name)" << std::endl;
        return;
    }

    // Check whether the name is already being used by another layer
    for(size_t i = 0; i < layersSet.size(); ++i)
        if(layersSet[i].layer_name == name) { std::cout << "Layer \"" << name << "\" already exists" << std::endl; return; }

    // Add the layer to the set
    std::lock_guard<std::mutex> lock(layerSetMutex);
    display.resize_buffer_set(layersSet.size() + 1);        // Empty function
    layersSet.push_back(layer(name, type, capacity));
}

int HansVisual::delete_layer(const char *layer_name)
{
    int layer_num = find_layer(layer_name);
    if(layer_num >= 0)
    {
        std::lock_guard<std::mutex> lock(layerSetMutex);
        display.resize_buffer_set(layersSet.size() - 1);
        layersSet.erase(layersSet.begin() + layer_num);
    }
    else { std::cout << "Layer \"" << layer_name << "\" not found" << std::endl; return 1; }
}

int HansVisual::clear_layer(std::string layer_name)
{
    int layer_num = find_layer(layer_name);

    if(layer_num >= 0)
    {
        std::lock_guard<std::mutex> lock(*layersSet[layer_num].layerMutex);
        layersSet[layer_num].objs_to_print = 0;
    }
    else { std::cout << "Layer \"" << layer_name << "\" not found" << std::endl; return 1; }
}

void HansVisual::draw_grid(float cell_size, unsigned int grid_size, float R, float G, float B)
{
    if(grid_size < 1 || cell_size <= 0) return;

    unsigned int num_lines = 2 * (grid_size + 1);
    float side_length = cell_size * grid_size;
    float initial_coord = side_length / 2;
    float z0 = 0;

    float (*grid)[2][3] = new float[num_lines][2][3];

    for(size_t i = 0; i < num_lines/2; ++i)
    {
        grid[i][0][0] =  initial_coord - i * cell_size;
        grid[i][0][1] =  initial_coord;
        grid[i][0][2] =  z0;

        grid[i][1][0] =  initial_coord - i * cell_size;
        grid[i][1][1] =  -initial_coord;
        grid[i][1][2] =  z0;


        grid[num_lines - 1 - i][0][0] =  initial_coord;
        grid[num_lines - 1 - i][0][1] =  initial_coord - i * cell_size;
        grid[num_lines - 1 - i][0][2] =  z0;

        grid[num_lines - 1 - i][1][0] = -initial_coord;
        grid[num_lines - 1 - i][1][1] =  initial_coord - i * cell_size;
        grid[num_lines - 1 - i][1][2] =  z0;
    }

    add_layer("Grid", lines, num_lines);
    lay("Grid").send_lines(num_lines, grid, R, G, B);
    delete[] grid;
}

void HansVisual::draw_axis(float length, bool system)
{
    float axis[3][2][3];
    float colors[3][3];

    // OpenGL coordinate system
    if(system)
    {
        axis[0][0][0] = 0;
        axis[0][0][1] = 0;
        axis[0][0][2] = 0;
        axis[0][1][0] = length;
        axis[0][1][1] = 0;
        axis[0][1][2] = 0;

        axis[1][0][0] = 0;
        axis[1][0][1] = 0;
        axis[1][0][2] = 0;
        axis[1][1][0] = 0;
        axis[1][1][1] = length;
        axis[1][1][2] = 0;

        axis[2][0][0] = 0;
        axis[2][0][1] = 0;
        axis[2][0][2] = 0;
        axis[2][1][0] = 0;
        axis[2][1][1] = 0;
        axis[2][1][2] = length;

        colors[0][0] = 1.0;
        colors[0][1] = 0.0;
        colors[0][2] = 0.0;

        colors[1][0] = 0.0;
        colors[1][1] = 1.0;
        colors[1][2] = 0.0;

        colors[2][0] = 0.0;
        colors[2][1] = 0.0;
        colors[2][2] = 1.0;
    }
    // Automotive coordinate system
    else
    {
        axis[0][0][0] = 0;
        axis[0][0][1] = 0;
        axis[0][0][2] = 0;
        axis[0][1][0] = -length;
        axis[0][1][1] = 0;
        axis[0][1][2] = 0;

        axis[1][0][0] = 0;
        axis[1][0][1] = 0;
        axis[1][0][2] = 0;
        axis[1][1][0] = 0;
        axis[1][1][1] = length;
        axis[1][1][2] = 0;

        axis[2][0][0] = 0;
        axis[2][0][1] = 0;
        axis[2][0][2] = 0;
        axis[2][1][0] = 0;
        axis[2][1][1] = 0;
        axis[2][1][2] = -length;

        colors[0][0] = 0.0;
        colors[0][1] = 1.0;
        colors[0][2] = 0.0;

        colors[1][0] = 0.0;
        colors[1][1] = 0.0;
        colors[1][2] = 1.0;

        colors[2][0] = 1.0;
        colors[2][1] = 0.0;
        colors[2][2] = 0.0;
    }

    add_layer("Axis", lines, 3);
    lay("Axis").send_lines_colors(3, axis, colors);
}

void HansVisual::allow_points_selection()
{
    // Add the selection square layer
    layer sel_sqr(SELECTION_SQUARE_LAYER_NAME, lines, 4);
    sel_sqr.dimensions = 2;
    sel_sqr.checkbox_visible = false;
    layersSet.push_back(sel_sqr);

    // Add the selected points layer
    layer selected_pnts(SELECTED_POINTS_LAYER_NAME, points, 0);
    layersSet.push_back(selected_pnts);
}

int HansVisual::set_layer_state(std::string layer_name, bool state)
{
    int layer_num = find_layer(layer_name);

    if(layer_num >= 0)
    {
        std::lock_guard<std::mutex> lock(*layersSet[layer_num].layerMutex);
        layersSet[layer_num].checkbox_value = state;
    }
    else { std::cout << "Layer \"" << layer_name << "\" not found" << std::endl; return 1; }
}

// Plotter system -------------------------------------

void HansVisual::fill_data_window(const std::string *data_strings, int num_strings)
{
    display.fill_data_window(data_strings, num_strings);
}

int HansVisual::open_window() { return display.open_window(); }

bool HansVisual::window_is_open() { return display.window_is_open(); }

void HansVisual::close_window() { display.close_window(); }

void HansVisual::wait()
{
    while(display.window_is_open())
        std::this_thread::sleep_for(std::chrono::seconds(1));
}

void HansVisual::get_cam_position(float *position)
{
    display.getCamPosition(position);
}

// Private ------------------------------------------

int HansVisual::find_layer(std::string layer_name)
{
    for (size_t i = 0; i < layersSet.size(); ++i)
        if (layersSet[i].layer_name == layer_name) return i;

    return -1;
}
