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

// <<<<<<<<<<<<<<
void HansVisual::draw_grid(float cell_size, int grid_size, int H, double S, double V)
{
    /*
    if(grid_size < 1 || cell_size <= 0) return;

    bool grid_exists = false;
    for(size_t i = 0; i < layersSet.size(); ++i) if(layersSet[i].layer_name == "Grid") grid_exists = true;
    if(!grid_exists)
    {
        // Create grid layer
    }

    // Create array
    int num_vertex = 2 * (grid_size + 1);



    // Eliminar esta metodología:
    myLines[i][0] = 1.2f;				// This point signals a jump between lines
    myLines[i][1] = 3.4f;
    myLines[i][2] = 5.6f;


    send_points("Points 2", 12, &pnts[0][0], &points_colors_RGB[0][0], nullptr, colors);
*/
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
