#include "HansVisual.hpp"

HansVisual::HansVisual() : display(&layersSet)
{

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

// <<<<<<<<<<<<<< Make this function work after open_window())
void HansVisual::add_layer(const char *name, object_type type, unsigned int capacity)
{
    layersSet.push_back(layer(name, type, capacity));
}

int HansVisual::clear_layer(std::string layer_name)
{
    std::vector<size_t> found_layers = find_layers(layer_name);

    if(found_layers.size() == 0) { std::cout << "Layer \"" << layer_name << "\" not found" << std::endl; return 1; }
    else
    {
        for(size_t i : found_layers)
        {
            std::lock_guard<std::mutex> lock(*layersSet[i].mut);
            layersSet[i].objs_to_print = 0;
        }
        return 0;
    }
}

void HansVisual::draw_grid(float cell_size, unsigned int grid_size, float R, float G, float B)
{
    if(grid_size < 1 || cell_size <= 0) return;

    unsigned int num_lines = 2 * (grid_size + 1);
    float side_length = cell_size * grid_size;
    float initial_coord = -(grid_size * cell_size / 2);
    float z0 = 0;

    float (*grid)[2][3] = new float[num_lines][2][3];

    for(size_t i = 0; i < num_lines/2; ++i)
    {
        grid[i][0][0] =  initial_coord + i * cell_size;
        grid[i][0][1] =  z0;
        grid[i][0][2] =  initial_coord;

        grid[i][1][0] =  initial_coord + i * cell_size;
        grid[i][1][1] =  z0;
        grid[i][1][2] = -initial_coord;


        grid[num_lines - 1 - i][0][0] =  initial_coord;
        grid[num_lines - 1 - i][0][1] =  z0;
        grid[num_lines - 1 - i][0][2] =  initial_coord + i * cell_size;

        grid[num_lines - 1 - i][1][0] = -initial_coord;
        grid[num_lines - 1 - i][1][1] =  z0;
        grid[num_lines - 1 - i][1][2] =  initial_coord + i * cell_size;
    }

    add_layer("Grid", lines, num_lines);
    lay("Grid").send_lines(num_lines, grid, R, G, B);
    delete[] grid;
}

void HansVisual::draw_axis(float length, bool system)
{
    float axis[3][2][3];

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

        axis[3][0][0] = 0;
        axis[3][0][1] = 0;
        axis[3][0][2] = 0;
        axis[3][1][0] = 0;
        axis[3][1][1] = 0;
        axis[3][1][2] = length;
    }
    else
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

        axis[3][0][0] = 0;
        axis[3][0][1] = 0;
        axis[3][0][2] = 0;
        axis[3][1][0] = 0;
        axis[3][1][1] = 0;
        axis[3][1][2] = length;
    }

    float colors[3][3] = {
        {1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {0.0, 0.0, 1.0}
    };

    add_layer("Axis", lines, 3);
    lay("Axis").send_lines_colors(3, axis, colors);
}

// Display system -------------------------------------

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

// Private ------------------------------------------

std::vector<size_t> HansVisual::find_layers(std::string layer_name)
{
    std::vector<size_t> found;

    for(size_t i = 0; i < layersSet.size(); ++i)
        if(layersSet[i].layer_name == layer_name) found.push_back(i);

    return found;
}
