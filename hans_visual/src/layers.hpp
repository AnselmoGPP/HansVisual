#ifndef LAYERS_HPP
#define LAYERS_HPP

#include <iostream>
#include <vector>
#include <mutex>
#include <cmath>

// Store one xyz point (not used)
struct pnt3D {
    float x, y, z;

    pnt3D(float a = 0, float b = 0, float c = 0) : x(a), y(b), z(c) { }

    pnt3D newData(float a, float b, float c) {
        x = a; y = b; z = c;
        return *this;
    }
};

// Structure used for sending cubes to the visualizer
struct cube3D {
    float X, Y, Z, width, height, length, rot_H;      // rotation in radians

    // Parameters: x, y, z (cube's center), width, height, length, rot_H (horizontal rotation)
    cube3D(float x, float y, float z, float w, float h, float l, float rh) :
        X(x), Y(y), Z(z), width(w), height(h), length(l), rot_H(rh) { }
};

// Names used for selecting different buffers
enum object_type { points, lines, triangles, cubes, none };

// Define the kind of the additional array you send
enum data_buffer {
    colors,             // Define colors of each point
    categories,         // Define category of each point. If number of categories > colors in the palette, the visualizer starts again from the beginning of the pallete
    gradient            // Define a gradient for each point that goes from a minimum to a maximum value. You enter the absolute minimum and maximum.
};

// Used to allow only one layer edition per main-loop iteration
enum layer_state{ open, half_closed, closed };

struct layer
{
    layer() :
        layer_name(""), max_objs(0), layer_type(none) { }

    layer(const char *name, object_type type, unsigned int capacity) :
        layer_name(name), max_objs(capacity), layer_type(type)
    {
        mut = new std::mutex;

        // Fill the palette with the default colors
        std::vector<std::vector<float>> temp = {
                          {1.0f,	0.0f,	0.00f},
                          {0.0f,	1.0f,	0.00f},
                          {0.0f,	0.0f,	1.00f},
                          {1.0f,	0.0f,	1.00f},
                          {1.0f,	1.0f,	0.00f},
                          {0.0f,	1.0f,	1.00f},
                          {0.5f,	0.0f,	0.00f},
                          {0.0f,	0.5f,	0.00f},
                          {0.0f,	0.0f,	0.50f},
                          {0.5f,	0.5f,	0.00f},
                          {0.5f,	0.0f,	0.50f},
                          {0.0f,	0.5f,	0.50f},
                          {0.75f,   0.25f,	0.25f},
                          {0.25f,   0.75f,	0.25f},
                          {0.25f,   0.25f,	0.75f},
                          {0.25f,   0.25f,	0.00f},
                          {0.25f,   0.0f,	0.25f},
                          {0.0f,	0.25f,	0.25f},
                          {0.75f,   0.75f,	0.00f},
                          {0.0f,	0.75f,	0.75f},
                          {0.75f,   0.0f,	0.75f} };
        palette = new float[palette_size][3];
        for(size_t i = 0; i < palette_size; i++)
        {
            palette[i][0] = temp[i][0];
            palette[i][1] = temp[i][1];
            palette[i][2] = temp[i][2];
        }

        // Fill the buffers depending on the layer type (points, lines, triangles, cubes)
        if(layer_type == points)
        {
            layer_type_str = "points";
            points_buffer = new float[max_objs][3];
            points_color_buffer = new float[max_objs][4];
            points_strings = new std::string[max_objs];
        }
        if(layer_type == lines)
        {
            layer_type_str = "lines";
            lines_buffer = new float[max_objs][2][3];
            lines_color_buffer = new float[max_objs][2][4];
        }
        if(layer_type == triangles)
        {
            layer_type_str = "triangles";
            triangles_buffer = new float[max_objs][3][3];
            triangles_color_buffer = new float[max_objs][3][4];
        }
        if(layer_type == cubes)
        {
            layer_type_str = "cubes";
            cubes_buffer = new float[max_objs][12*3][3];
            cubes_color_buffer = new float[max_objs][12*3][4];
        }
    }

    // Copy constructor
    layer(const layer &obj)
    {
        // Main layer data
        layer_name = obj.layer_name;
        max_objs = obj.max_objs;
        layer_type = obj.layer_type;
        layer_type_str = obj.layer_type_str;
        objs_to_print = obj.objs_to_print;
        state = obj.state;
        checkbox_value = obj.checkbox_value;
        delete mut;
        mut = new std::mutex;

        // Colors
        palette_size = obj.palette_size;
        delete[] palette;
        palette = new float[palette_size][3];
        for(size_t i = 0; i < palette_size; i++)
        {
            palette[i][0] = obj.palette[i][0];
            palette[i][1] = obj.palette[i][1];
            palette[i][2] = obj.palette[i][2];
        }
        alpha_channel = obj.alpha_channel;

        // Buffers
        delete[] points_buffer;
        delete[] points_color_buffer;
        delete[] points_strings;
        delete[] lines_buffer;
        delete[] lines_color_buffer;
        delete[] triangles_buffer;
        delete[] triangles_color_buffer;
        delete[] cubes_buffer;
        delete[] cubes_color_buffer;

        points_buffer           = nullptr;
        points_color_buffer     = nullptr;
        points_strings          = nullptr;
        lines_buffer            = nullptr;
        lines_color_buffer      = nullptr;
        triangles_buffer        = nullptr;
        triangles_color_buffer  = nullptr;
        cubes_buffer            = nullptr;
        cubes_color_buffer      = nullptr;

        if(layer_type == points)
        {
            points_buffer = new float[max_objs][3];
            points_color_buffer = new float[max_objs][4];
            points_strings = new std::string[max_objs];

            for (int i = 0; i < max_objs; i++)
            {
                points_buffer[i][0] = obj.points_buffer[i][0];
                points_buffer[i][1] = obj.points_buffer[i][1];
                points_buffer[i][2] = obj.points_buffer[i][2];

                points_color_buffer[i][0] = obj.points_color_buffer[i][0];
                points_color_buffer[i][1] = obj.points_color_buffer[i][1];
                points_color_buffer[i][2] = obj.points_color_buffer[i][2];
                points_color_buffer[i][3] = obj.points_color_buffer[i][3];

                points_strings[i] = obj.points_strings[i];
            }
        }
        if(layer_type == lines)
        {
            lines_buffer = new float[max_objs][2][3];
            lines_color_buffer = new float[max_objs][2][4];

            for (int i = 0; i < max_objs; i++)
            {
                lines_buffer[i][0][0] = obj.lines_buffer[i][0][0];
                lines_buffer[i][0][1] = obj.lines_buffer[i][0][1];
                lines_buffer[i][0][2] = obj.lines_buffer[i][0][2];

                lines_buffer[i][1][0] = obj.lines_buffer[i][1][0];
                lines_buffer[i][1][1] = obj.lines_buffer[i][1][1];
                lines_buffer[i][1][2] = obj.lines_buffer[i][1][2];

                lines_color_buffer[i][0][0] = obj.lines_color_buffer[i][0][0];
                lines_color_buffer[i][0][1] = obj.lines_color_buffer[i][0][1];
                lines_color_buffer[i][0][2] = obj.lines_color_buffer[i][0][2];
                lines_color_buffer[i][0][3] = obj.lines_color_buffer[i][0][3];

                lines_color_buffer[i][1][0] = obj.lines_color_buffer[i][1][0];
                lines_color_buffer[i][1][1] = obj.lines_color_buffer[i][1][1];
                lines_color_buffer[i][1][2] = obj.lines_color_buffer[i][1][2];
                lines_color_buffer[i][1][3] = obj.lines_color_buffer[i][1][3];
            }

        }
        if(layer_type == triangles)
        {
            triangles_buffer = new float[max_objs][3][3];
            triangles_color_buffer = new float[max_objs][3][4];

            for (int i = 0; i < max_objs; i++)
            {
                triangles_buffer[i][0][0] = obj.triangles_buffer[i][0][0];
                triangles_buffer[i][0][1] = obj.triangles_buffer[i][0][1];
                triangles_buffer[i][0][2] = obj.triangles_buffer[i][0][2];

                triangles_buffer[i][1][0] = obj.triangles_buffer[i][1][0];
                triangles_buffer[i][1][1] = obj.triangles_buffer[i][1][1];
                triangles_buffer[i][1][2] = obj.triangles_buffer[i][1][2];

                triangles_buffer[i][2][0] = obj.triangles_buffer[i][2][0];
                triangles_buffer[i][2][1] = obj.triangles_buffer[i][2][1];
                triangles_buffer[i][2][2] = obj.triangles_buffer[i][2][2];

                triangles_color_buffer[i][0][0] = obj.triangles_color_buffer[i][0][0];
                triangles_color_buffer[i][0][1] = obj.triangles_color_buffer[i][0][1];
                triangles_color_buffer[i][0][2] = obj.triangles_color_buffer[i][0][2];
                triangles_color_buffer[i][0][3] = obj.triangles_color_buffer[i][0][3];

                triangles_color_buffer[i][1][0] = obj.triangles_color_buffer[i][1][0];
                triangles_color_buffer[i][1][1] = obj.triangles_color_buffer[i][1][1];
                triangles_color_buffer[i][1][2] = obj.triangles_color_buffer[i][1][2];
                triangles_color_buffer[i][1][3] = obj.triangles_color_buffer[i][1][3];

                triangles_color_buffer[i][2][0] = obj.triangles_color_buffer[i][2][0];
                triangles_color_buffer[i][2][1] = obj.triangles_color_buffer[i][2][1];
                triangles_color_buffer[i][2][2] = obj.triangles_color_buffer[i][2][2];
                triangles_color_buffer[i][2][3] = obj.triangles_color_buffer[i][2][3];
            }
        }
        if(layer_type == cubes)
        {
            cubes_buffer = new float[max_objs][12*3][3];
            cubes_color_buffer = new float[max_objs][12*3][4];

            for(int i = 0; i < max_objs; i++)
                for (int vert = 0; vert < 12*3; vert++)
                {
                    cubes_buffer[i][vert][0] = obj.cubes_buffer[i][vert][0];
                    cubes_buffer[i][vert][1] = obj.cubes_buffer[i][vert][1];
                    cubes_buffer[i][vert][2] = obj.cubes_buffer[i][vert][2];

                    cubes_color_buffer[i][vert][0] = obj.cubes_color_buffer[i][vert][0];
                    cubes_color_buffer[i][vert][1] = obj.cubes_color_buffer[i][vert][1];
                    cubes_color_buffer[i][vert][2] = obj.cubes_color_buffer[i][vert][2];
                    cubes_color_buffer[i][vert][3] = obj.cubes_color_buffer[i][vert][3];
                }
        }
    }

    // Copy assignment operator
    layer& operator=(const layer &obj)
    {
        // Main layer data
        layer_name = obj.layer_name;
        max_objs = obj.max_objs;
        layer_type = obj.layer_type;
        layer_type_str = obj.layer_type_str;
        objs_to_print = obj.objs_to_print;
        state = obj.state;
        checkbox_value = obj.checkbox_value;
        delete mut;
        mut = new std::mutex;

        // Colors
        palette_size = obj.palette_size;
        delete[] palette;
        palette = new float[palette_size][3];
        for(size_t i = 0; i < palette_size; i++)
        {
            palette[i][0] = obj.palette[i][0];
            palette[i][1] = obj.palette[i][1];
            palette[i][2] = obj.palette[i][2];
        }
        alpha_channel = obj.alpha_channel;

        // Buffers
        delete[] points_buffer;
        delete[] points_color_buffer;
        delete[] points_strings;
        delete[] lines_buffer;
        delete[] lines_color_buffer;
        delete[] triangles_buffer;
        delete[] triangles_color_buffer;
        delete[] cubes_buffer;
        delete[] cubes_color_buffer;

        points_buffer           = nullptr;
        points_color_buffer     = nullptr;
        points_strings          = nullptr;
        lines_buffer            = nullptr;
        lines_color_buffer      = nullptr;
        triangles_buffer        = nullptr;
        triangles_color_buffer  = nullptr;
        cubes_buffer            = nullptr;
        cubes_color_buffer      = nullptr;

        if(layer_type == points)
        {
            delete[] points_buffer;
            delete[] points_color_buffer;
            delete[] points_strings;
            points_buffer = new float[max_objs][3];
            points_color_buffer = new float[max_objs][4];
            points_strings = new std::string[max_objs];

            for (int i = 0; i < max_objs; i++)
            {
                points_buffer[i][0] = obj.points_buffer[i][0];
                points_buffer[i][1] = obj.points_buffer[i][1];
                points_buffer[i][2] = obj.points_buffer[i][2];

                points_color_buffer[i][0] = obj.points_color_buffer[i][0];
                points_color_buffer[i][1] = obj.points_color_buffer[i][1];
                points_color_buffer[i][2] = obj.points_color_buffer[i][2];
                points_color_buffer[i][3] = obj.points_color_buffer[i][3];

                points_strings[i] = obj.points_strings[i];
            }
        }
        if(layer_type == lines)
        {
            delete[] lines_buffer;
            delete[] lines_color_buffer;
            lines_buffer = new float[max_objs][2][3];
            lines_color_buffer = new float[max_objs][2][4];

            for (int i = 0; i < max_objs; i++)
            {
                lines_buffer[i][0][0] = obj.lines_buffer[i][0][0];
                lines_buffer[i][0][1] = obj.lines_buffer[i][0][1];
                lines_buffer[i][0][2] = obj.lines_buffer[i][0][2];

                lines_buffer[i][1][0] = obj.lines_buffer[i][1][0];
                lines_buffer[i][1][1] = obj.lines_buffer[i][1][1];
                lines_buffer[i][1][2] = obj.lines_buffer[i][1][2];

                lines_color_buffer[i][0][0] = obj.lines_color_buffer[i][0][0];
                lines_color_buffer[i][0][1] = obj.lines_color_buffer[i][0][1];
                lines_color_buffer[i][0][2] = obj.lines_color_buffer[i][0][2];
                lines_color_buffer[i][0][3] = obj.lines_color_buffer[i][0][3];

                lines_color_buffer[i][1][0] = obj.lines_color_buffer[i][1][0];
                lines_color_buffer[i][1][1] = obj.lines_color_buffer[i][1][1];
                lines_color_buffer[i][1][2] = obj.lines_color_buffer[i][1][2];
                lines_color_buffer[i][1][3] = obj.lines_color_buffer[i][1][3];
            }

        }
        if(layer_type == triangles)
        {
            delete[] triangles_buffer;
            delete[] triangles_color_buffer;
            triangles_buffer = new float[max_objs][3][3];
            triangles_color_buffer = new float[max_objs][3][4];

            for (int i = 0; i < max_objs; i++)
            {
                triangles_buffer[i][0][0] = obj.triangles_buffer[i][0][0];
                triangles_buffer[i][0][1] = obj.triangles_buffer[i][0][1];
                triangles_buffer[i][0][2] = obj.triangles_buffer[i][0][2];

                triangles_buffer[i][1][0] = obj.triangles_buffer[i][1][0];
                triangles_buffer[i][1][1] = obj.triangles_buffer[i][1][1];
                triangles_buffer[i][1][2] = obj.triangles_buffer[i][1][2];

                triangles_buffer[i][2][0] = obj.triangles_buffer[i][2][0];
                triangles_buffer[i][2][1] = obj.triangles_buffer[i][2][1];
                triangles_buffer[i][2][2] = obj.triangles_buffer[i][2][2];

                triangles_color_buffer[i][0][0] = obj.triangles_color_buffer[i][0][0];
                triangles_color_buffer[i][0][1] = obj.triangles_color_buffer[i][0][1];
                triangles_color_buffer[i][0][2] = obj.triangles_color_buffer[i][0][2];
                triangles_color_buffer[i][0][3] = obj.triangles_color_buffer[i][0][3];

                triangles_color_buffer[i][1][0] = obj.triangles_color_buffer[i][1][0];
                triangles_color_buffer[i][1][1] = obj.triangles_color_buffer[i][1][1];
                triangles_color_buffer[i][1][2] = obj.triangles_color_buffer[i][1][2];
                triangles_color_buffer[i][1][3] = obj.triangles_color_buffer[i][1][3];

                triangles_color_buffer[i][2][0] = obj.triangles_color_buffer[i][2][0];
                triangles_color_buffer[i][2][1] = obj.triangles_color_buffer[i][2][1];
                triangles_color_buffer[i][2][2] = obj.triangles_color_buffer[i][2][2];
                triangles_color_buffer[i][2][3] = obj.triangles_color_buffer[i][2][3];
            }
        }
        if(layer_type == cubes)
        {
            delete[] cubes_buffer;
            delete[] cubes_color_buffer;
            cubes_buffer = new float[max_objs][12*3][3];
            cubes_color_buffer = new float[max_objs][12*3][4];

            for(int i = 0; i < max_objs; i++)
                for (int vert = 0; vert < 12*3; vert++)
                {
                    cubes_buffer[i][vert][0] = obj.cubes_buffer[i][vert][0];
                    cubes_buffer[i][vert][1] = obj.cubes_buffer[i][vert][1];
                    cubes_buffer[i][vert][2] = obj.cubes_buffer[i][vert][2];

                    cubes_color_buffer[i][vert][0] = obj.cubes_color_buffer[i][vert][0];
                    cubes_color_buffer[i][vert][1] = obj.cubes_color_buffer[i][vert][1];
                    cubes_color_buffer[i][vert][2] = obj.cubes_color_buffer[i][vert][2];
                    cubes_color_buffer[i][vert][3] = obj.cubes_color_buffer[i][vert][3];
                }
        }

        return *this;
    }

    ~layer()
    {
        delete mut;

        delete[] palette;

        delete[] points_buffer;
        delete[] points_color_buffer;
        delete[] points_strings;

        delete[] lines_buffer;
        delete[] lines_color_buffer;

        delete[] triangles_buffer;
        delete[] triangles_color_buffer;

        delete[] cubes_buffer;
        delete[] cubes_color_buffer;
    }

    // Main layer data
    std::string layer_name;
    unsigned int max_objs;
    object_type layer_type;
    std::string layer_type_str = "none";
    size_t objs_to_print = 0;                    // Number of objects that are going to be printed
    layer_state state = open;
    bool checkbox_value = true;
    std::mutex *mut = nullptr;

    // Colors
    float(*palette)[3] = nullptr;                   // Palette of colors of the layer
    size_t palette_size = 21;                       // Size of the layer's palette (number of colors)
    float alpha_channel = 1.0f;
    const float default_color[3] = { 1., 1., 1. };

    // Points data
    float(*points_buffer)[3]                = nullptr;      // Stores all the coordinates of all the points of the layer
    float(*points_color_buffer)[4]          = nullptr;      // Stores the RGBA colors of each point
    std::string *points_strings             = nullptr;      // Optional data for each point (used in selections)
    // Lines data
    float(*lines_buffer)[2][3]              = nullptr;
    float(*lines_color_buffer)[2][4]        = nullptr;
    // Triangles
    float(*triangles_buffer)[3][3]          = nullptr;
    float(*triangles_color_buffer)[3][4]    = nullptr;
    // Cubes data
    float(*cubes_buffer)[12*3][3]           = nullptr;
    float(*cubes_color_buffer)[12*3][4]     = nullptr;

    // Send new data to the layer's buffers ---------------------------------------------------------
    // Send points to print. There are 4 ways to specify the colors of the objects sent:
    //          - Default: If you only pass the number of objects and a pointer to the array, the objects will have the default_color.
    //          - 'categories': Pass also an array with labels (clusters) for each object (range: [0, number of categories - 1]. Specify the array type as 'categories' (if not specified, it is considered 'categories'). The palette will be used to print each category with a different color.
    //          - 'colors': Pass an array with colors for each object. Specify the array type as 'colors'.
    //          - 'gradient': Pass an array with (gradual) values for each object (height, temperature...). Must include the minimum and maximum values (inclusive) (if not specified, minimum is 0 and maximum is 1). Specify the array type as 'gradient'. This coloring method should be used with a palette containing gradual colors.

    int save_points(unsigned int number_points, const float *arr, const float *labels = nullptr, std::string *points_data = nullptr, data_buffer array_type = categories, float min = 0, float max = 1)
    {
        if      (state == closed) return 1;
        else if (state == half_closed) state = closed;

        if      (layer_type == none) { error_message(1); return 1; }
        else if (layer_type != points) { error_message(5); return 1; }

        // Get the number of points the user wants to show on screen. Check whether the layer maximum size is being reached and, if it is, write to buffer only the maximum possible number of them.
        if (number_points > max_objs)
        {
            error_message(2, number_points);
            objs_to_print = max_objs;
        }
        else objs_to_print = number_points;

        // Write data to buffers (points and colors)
        float siz = max - min;		// Used for gradients
        int index;

        std::lock_guard<std::mutex> lock(*mut);

        for (int i = 0; i < objs_to_print; i++)			// Main loop for filling the corresponding points buffer
        {
            // Set point position in 3D;
            points_buffer[i][0] = arr[i * 3 + 0];
            points_buffer[i][1] = arr[i * 3 + 1];
            points_buffer[i][2] = arr[i * 3 + 2];

            // Set color of the point
            if		(labels     == nullptr)
            {
                points_color_buffer[i][0] = default_color[0];
                points_color_buffer[i][1] = default_color[1];
                points_color_buffer[i][2] = default_color[2];
                points_color_buffer[i][3] = alpha_channel;
            }
            else if (array_type == categories)
            {
                index = (int)labels[i] % palette_size;

                points_color_buffer[i][0] = palette[index][0];
                points_color_buffer[i][1] = palette[index][1];
                points_color_buffer[i][2] = palette[index][2];
                points_color_buffer[i][3] = alpha_channel;
            }
            else if	(array_type == colors)
            {
                points_color_buffer[i][0] = labels[i * 3 + 0];
                points_color_buffer[i][1] = labels[i * 3 + 1];
                points_color_buffer[i][2] = labels[i * 3 + 2];
                points_color_buffer[i][3] = alpha_channel;
            }
            else if (array_type == gradient)
            {
                if      (labels[i] <= min) index = 0;
                else if (labels[i] >= max) index = palette_size - 1;
                else    index = (int)((labels[i] - min) * (palette_size - 1)) / (int)siz;

                points_color_buffer[i][0] = palette[index][0];
                points_color_buffer[i][1] = palette[index][1];
                points_color_buffer[i][2] = palette[index][2];
                points_color_buffer[i][3] = alpha_channel;
            }

            // Fill/empty points_strings[] and empty selected_points[]
            if(points_data != nullptr)
            {
                points_strings[i] = points_data[i];
                //selected_points[i] = 0;
            }
            else
            {
                points_strings[i] = "";
                //selected_points[i] = 0;
            }
        }

        return 0;
    }

    int save_lines(unsigned int number_points, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1)
    {
        if      (state == closed) return 1;
        else if (state == half_closed) state = closed;

        if      (layer_type == none) { error_message(1); return 1; }
        else if (layer_type != lines) { error_message(5); return 1; }

        // Get the number of lines the user wants to show on screen. Check whether the layer maximum size is being reached and, if it is, write to buffer only the maximum possible number of them.
        if (number_points - 1 > max_objs)
        {
            error_message(3, number_points);
            objs_to_print = max_objs;
        }
        else objs_to_print = --number_points;

        // Write data to buffers (points and colors)
        float siz = max - min;		// Used for gradients
        int index = 0;
        int omitted_segments = 0;

        std::lock_guard<std::mutex> lock(*mut);

        for (int i = 0; i < objs_to_print; i++)		// Main loop for filling the corresponding lines buffer
        {
            // Check for the line jump
            if (arr[i * 3 + 3] == 1.2f && arr[i * 3 + 4] == 3.4f && arr[i * 3 + 5] == 5.6f) {
                omitted_segments += 2;
                i++;
                continue;
            }

            // Save two points (vertex) for each segment
            lines_buffer[i - omitted_segments][0][0] = arr[i * 3 + 0];
            lines_buffer[i - omitted_segments][0][1] = arr[i * 3 + 1];
            lines_buffer[i - omitted_segments][0][2] = arr[i * 3 + 2];

            lines_buffer[i - omitted_segments][1][0] = arr[i * 3 + 3];
            lines_buffer[i - omitted_segments][1][1] = arr[i * 3 + 4];
            lines_buffer[i - omitted_segments][1][2] = arr[i * 3 + 5];

            // Save colors for each vertex
            if		(labels == nullptr)
            {
                lines_color_buffer[i - omitted_segments][0][0] = default_color[0];
                lines_color_buffer[i - omitted_segments][0][1] = default_color[1];
                lines_color_buffer[i - omitted_segments][0][2] = default_color[2];
                lines_color_buffer[i - omitted_segments][0][3] = alpha_channel;

                lines_color_buffer[i - omitted_segments][1][0] = default_color[0];
                lines_color_buffer[i - omitted_segments][1][1] = default_color[1];
                lines_color_buffer[i - omitted_segments][1][2] = default_color[2];
                lines_color_buffer[i - omitted_segments][1][3] = alpha_channel;
            }
            else if (array_type == categories)  // labels contains the category of each segment (including line jumps)
            {
                index = (int)labels[i] % palette_size;

                lines_color_buffer[i - omitted_segments][0][0] = palette[index][0];
                lines_color_buffer[i - omitted_segments][0][1] = palette[index][1];
                lines_color_buffer[i - omitted_segments][0][2] = palette[index][2];
                lines_color_buffer[i - omitted_segments][0][3] = alpha_channel;

                lines_color_buffer[i - omitted_segments][1][0] = palette[index][0];
                lines_color_buffer[i - omitted_segments][1][1] = palette[index][1];
                lines_color_buffer[i - omitted_segments][1][2] = palette[index][2];
                lines_color_buffer[i - omitted_segments][1][3] = alpha_channel;
            }
            else if	(array_type == colors)      // labels contains the color of each segment (including line jumps)
            {
                lines_color_buffer[i - omitted_segments][0][0] = labels[i * 3 + 0];
                lines_color_buffer[i - omitted_segments][0][1] = labels[i * 3 + 1];
                lines_color_buffer[i - omitted_segments][0][2] = labels[i * 3 + 2];
                lines_color_buffer[i - omitted_segments][0][3] = alpha_channel;

                lines_color_buffer[i - omitted_segments][1][0] = labels[i * 3 + 0];
                lines_color_buffer[i - omitted_segments][1][1] = labels[i * 3 + 1];
                lines_color_buffer[i - omitted_segments][1][2] = labels[i * 3 + 2];
                lines_color_buffer[i - omitted_segments][1][3] = alpha_channel;
            }
            else if (array_type == gradient)
            {
                if (labels[i] <= min) index = 0;
                else if (labels[i] >= max) index = palette_size - 1;
                else {
                    index = ((labels[i] - min) * (palette_size - 1)) / siz;
                }

                lines_color_buffer[i - omitted_segments][0][0] = palette[index][0];
                lines_color_buffer[i - omitted_segments][0][1] = palette[index][1];
                lines_color_buffer[i - omitted_segments][0][2] = palette[index][2];
                lines_color_buffer[i - omitted_segments][0][3] = alpha_channel;

                lines_color_buffer[i - omitted_segments][1][0] = palette[index][0];
                lines_color_buffer[i - omitted_segments][1][1] = palette[index][1];
                lines_color_buffer[i - omitted_segments][1][2] = palette[index][2];
                lines_color_buffer[i - omitted_segments][1][3] = alpha_channel;
            }
        }

        return 0;
    }

    int save_triangles(unsigned int number_triangles, const float *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1)
    {
        if      (state == closed) return 1;
        else if (state == half_closed) state = closed;

        if      (layer_type == none) { error_message(1); return 1; }
        else if (layer_type != triangles) { error_message(5); return 1; }

        // Get the number of triangles the user wants to show on screen. Check whether the layer maximum size is being reached and, if it is, write to buffer only the maximum possible number of them.
        if (number_triangles > max_objs) {
            error_message(2, number_triangles);
            objs_to_print = max_objs;
        }
        else objs_to_print = number_triangles;

        // Write data to buffers (points and colors)
        float siz = max - min;		// Used for gradients
        int index;

        std::lock_guard<std::mutex> lock(*mut);

        for (int i = 0; i < objs_to_print; i++)			// Main loop for filling the corresponding points buffer
        {
            // Set triangle position in 3D;
            triangles_buffer[i][0][0] = arr[i * 9 + 0];
            triangles_buffer[i][0][1] = arr[i * 9 + 1];
            triangles_buffer[i][0][2] = arr[i * 9 + 2];

            triangles_buffer[i][1][0] = arr[i * 9 + 3];
            triangles_buffer[i][1][1] = arr[i * 9 + 4];
            triangles_buffer[i][1][2] = arr[i * 9 + 5];

            triangles_buffer[i][2][0] = arr[i * 9 + 6];
            triangles_buffer[i][2][1] = arr[i * 9 + 7];
            triangles_buffer[i][2][2] = arr[i * 9 + 8];

            // Set color of the point
            if		(labels     == nullptr)
            {
                triangles_color_buffer[i][0][0] = default_color[0];
                triangles_color_buffer[i][0][1] = default_color[1];
                triangles_color_buffer[i][0][2] = default_color[2];
                triangles_color_buffer[i][0][3] = alpha_channel;

                triangles_color_buffer[i][1][0] = default_color[0];
                triangles_color_buffer[i][1][1] = default_color[1];
                triangles_color_buffer[i][1][2] = default_color[2];
                triangles_color_buffer[i][1][3] = alpha_channel;

                triangles_color_buffer[i][2][0] = default_color[0];
                triangles_color_buffer[i][2][1] = default_color[1];
                triangles_color_buffer[i][2][2] = default_color[2];
                triangles_color_buffer[i][2][3] = alpha_channel;
            }
            else if (array_type == categories)
            {
                index = (int)labels[i] % palette_size;

                triangles_color_buffer[i][0][0] = palette[index][0];
                triangles_color_buffer[i][0][1] = palette[index][1];
                triangles_color_buffer[i][0][2] = palette[index][2];
                triangles_color_buffer[i][0][3] = alpha_channel;

                triangles_color_buffer[i][1][0] = palette[index][0];
                triangles_color_buffer[i][1][1] = palette[index][1];
                triangles_color_buffer[i][1][2] = palette[index][2];
                triangles_color_buffer[i][1][3] = alpha_channel;

                triangles_color_buffer[i][2][0] = palette[index][0];
                triangles_color_buffer[i][2][1] = palette[index][1];
                triangles_color_buffer[i][2][2] = palette[index][2];
                triangles_color_buffer[i][2][3] = alpha_channel;
            }
            else if	(array_type == colors)
            {
                triangles_color_buffer[i][0][0] = labels[i * 3 + 0];
                triangles_color_buffer[i][0][1] = labels[i * 3 + 1];
                triangles_color_buffer[i][0][2] = labels[i * 3 + 2];
                triangles_color_buffer[i][0][3] = alpha_channel;

                triangles_color_buffer[i][1][0] = labels[i * 3 + 0];
                triangles_color_buffer[i][1][1] = labels[i * 3 + 1];
                triangles_color_buffer[i][1][2] = labels[i * 3 + 2];
                triangles_color_buffer[i][1][3] = alpha_channel;

                triangles_color_buffer[i][2][0] = labels[i * 3 + 0];
                triangles_color_buffer[i][2][1] = labels[i * 3 + 1];
                triangles_color_buffer[i][2][2] = labels[i * 3 + 2];
                triangles_color_buffer[i][2][3] = alpha_channel;
            }
            else if (array_type == gradient)
            {
                if      (labels[i] <= min) index = 0;
                else if (labels[i] >= max) index = palette_size - 1;
                else {
                    index = (int)((labels[i] - min) * (palette_size - 1)) / (int)siz;
                }

                triangles_color_buffer[i][0][0] = palette[index][0];
                triangles_color_buffer[i][0][1] = palette[index][1];
                triangles_color_buffer[i][0][2] = palette[index][2];
                triangles_color_buffer[i][0][3] = alpha_channel;

                triangles_color_buffer[i][1][0] = palette[index][0];
                triangles_color_buffer[i][1][1] = palette[index][1];
                triangles_color_buffer[i][1][2] = palette[index][2];
                triangles_color_buffer[i][1][3] = alpha_channel;

                triangles_color_buffer[i][2][0] = palette[index][0];
                triangles_color_buffer[i][2][1] = palette[index][1];
                triangles_color_buffer[i][2][2] = palette[index][2];
                triangles_color_buffer[i][2][3] = alpha_channel;
            }
        }

        return 0;
    }

    int save_cubes(unsigned int number_cubes, const cube3D *arr, const float *labels = nullptr, data_buffer array_type = categories, float min = 0, float max = 1)
    {
        if      (state == closed) return 1;
        else if (state == half_closed) state = closed;

        if      (layer_type == none) { error_message(1); return 1; }
        else if (layer_type != cubes) { error_message(5); return 1; }

        // Get the number of cubes the user wants to show on screen. Check whether the layer maximum size is being reached and, if it is, write to buffer only the maximum possible number of them.
        if (number_cubes > max_objs) {
            error_message(4, number_cubes);
            objs_to_print = max_objs;
        }
        else objs_to_print = number_cubes;

        // Write data to buffers (cubes and colors)
        float X, Y, Z, x, y, z, rot_H, rot_V;

        std::lock_guard<std::mutex> lock(*mut);

        // Fill cubes_buffer                            >>> This draws triangles anti-clockwise
        for (size_t i = 0; i < objs_to_print; i++) {

            X = arr[i].X;
            Y = arr[i].Y;
            Z = arr[i].Z;
            x = arr[i].width / 2;
            y = arr[i].height / 2;
            z = arr[i].length / 2;
            rot_H = arr[i].rot_H;

            /*
                        // Computations used with no rotations
                        float x1 = X - x,   y1 = Y + y,   z1 = Z + z;
                        float x2 = X - x,   y2 = Y - y,   z2 = Z + z;
                        float x3 = X + x,   y3 = Y + y,   z3 = Z + z;
                        float x4 = X + x,   y4 = Y - y,   z4 = Z + z;
                        float x5 = X + x,   y5 = Y + y,   z5 = Z - z;
                        float x6 = X + x,   y6 = Y - y,   z6 = Z - z;
                        float x7 = X - x,   y7 = Y + y,   z7 = Z - z;
                        float x8 = X - x,   y8 = Y - y,   z8 = Z - z;
            */

            float x1 = -x, y1 = Y + y, z1 = +z;
            float x2 = -x, y2 = Y - y, z2 = +z;
            float x3 = +x, y3 = Y + y, z3 = +z;
            float x4 = +x, y4 = Y - y, z4 = +z;
            float x5 = +x, y5 = Y + y, z5 = -z;
            float x6 = +x, y6 = Y - y, z6 = -z;
            float x7 = -x, y7 = Y + y, z7 = -z;
            float x8 = -x, y8 = Y - y, z8 = -z;

            rotation_H(x1, z1, X, Z, rot_H);
            rotation_H(x2, z2, X, Z, rot_H);
            rotation_H(x3, z3, X, Z, rot_H);
            rotation_H(x4, z4, X, Z, rot_H);
            rotation_H(x5, z5, X, Z, rot_H);
            rotation_H(x6, z6, X, Z, rot_H);
            rotation_H(x7, z7, X, Z, rot_H);
            rotation_H(x8, z8, X, Z, rot_H);

            // 1-2-3
            cubes_buffer[i][0][0] = x1;
            cubes_buffer[i][0][1] = y1;
            cubes_buffer[i][0][2] = z1;
            cubes_buffer[i][1][0] = x2;
            cubes_buffer[i][1][1] = y2;
            cubes_buffer[i][1][2] = z2;
            cubes_buffer[i][2][0] = x3;
            cubes_buffer[i][2][1] = y3;
            cubes_buffer[i][2][2] = z3;
            // 2-4-3
            cubes_buffer[i][3][0] = x2;
            cubes_buffer[i][3][1] = y2;
            cubes_buffer[i][3][2] = z2;
            cubes_buffer[i][4][0] = x4;
            cubes_buffer[i][4][1] = y4;
            cubes_buffer[i][4][2] = z4;
            cubes_buffer[i][5][0] = x3;
            cubes_buffer[i][5][1] = y3;
            cubes_buffer[i][5][2] = z3;
            // 3-4-5
            cubes_buffer[i][6][0] = x3;
            cubes_buffer[i][6][1] = y3;
            cubes_buffer[i][6][2] = z3;
            cubes_buffer[i][7][0] = x4;
            cubes_buffer[i][7][1] = y4;
            cubes_buffer[i][7][2] = z4;
            cubes_buffer[i][8][0] = x5;
            cubes_buffer[i][8][1] = y5;
            cubes_buffer[i][8][2] = z5;
            // 4-6-5
            cubes_buffer[i][9][0] = x4;
            cubes_buffer[i][9][1] = y4;
            cubes_buffer[i][9][2] = z4;
            cubes_buffer[i][10][0] = x6;
            cubes_buffer[i][10][1] = y6;
            cubes_buffer[i][10][2] = z6;
            cubes_buffer[i][11][0] = x5;
            cubes_buffer[i][11][1] = y5;
            cubes_buffer[i][11][2] = z5;
            // 5-6-7
            cubes_buffer[i][12][0] = x5;
            cubes_buffer[i][12][1] = y5;
            cubes_buffer[i][12][2] = z5;
            cubes_buffer[i][13][0] = x6;
            cubes_buffer[i][13][1] = y6;
            cubes_buffer[i][13][2] = z6;
            cubes_buffer[i][14][0] = x7;
            cubes_buffer[i][14][1] = y7;
            cubes_buffer[i][14][2] = z7;
            // 6-8-7
            cubes_buffer[i][15][0] = x6;
            cubes_buffer[i][15][1] = y6;
            cubes_buffer[i][15][2] = z6;
            cubes_buffer[i][16][0] = x8;
            cubes_buffer[i][16][1] = y8;
            cubes_buffer[i][16][2] = z8;
            cubes_buffer[i][17][0] = x7;
            cubes_buffer[i][17][1] = y7;
            cubes_buffer[i][17][2] = z7;
            // 7-8-1
            cubes_buffer[i][18][0] = x7;
            cubes_buffer[i][18][1] = y7;
            cubes_buffer[i][18][2] = z7;
            cubes_buffer[i][19][0] = x8;
            cubes_buffer[i][19][1] = y8;
            cubes_buffer[i][19][2] = z8;
            cubes_buffer[i][20][0] = x1;
            cubes_buffer[i][20][1] = y1;
            cubes_buffer[i][20][2] = z1;
            // 8-2-1
            cubes_buffer[i][21][0] = x8;
            cubes_buffer[i][21][1] = y8;
            cubes_buffer[i][21][2] = z8;
            cubes_buffer[i][22][0] = x2;
            cubes_buffer[i][22][1] = y2;
            cubes_buffer[i][22][2] = z2;
            cubes_buffer[i][23][0] = x1;
            cubes_buffer[i][23][1] = y1;
            cubes_buffer[i][23][2] = z1;
            // 1-3-7
            cubes_buffer[i][24][0] = x1;
            cubes_buffer[i][24][1] = y1;
            cubes_buffer[i][24][2] = z1;
            cubes_buffer[i][25][0] = x3;
            cubes_buffer[i][25][1] = y3;
            cubes_buffer[i][25][2] = z3;
            cubes_buffer[i][26][0] = x7;
            cubes_buffer[i][26][1] = y7;
            cubes_buffer[i][26][2] = z7;
            // 3-5-7
            cubes_buffer[i][27][0] = x3;
            cubes_buffer[i][27][1] = y3;
            cubes_buffer[i][27][2] = z3;
            cubes_buffer[i][28][0] = x5;
            cubes_buffer[i][28][1] = y5;
            cubes_buffer[i][28][2] = z5;
            cubes_buffer[i][29][0] = x7;
            cubes_buffer[i][29][1] = y7;
            cubes_buffer[i][29][2] = z7;
            // 2-8-4
            cubes_buffer[i][30][0] = x2;
            cubes_buffer[i][30][1] = y2;
            cubes_buffer[i][30][2] = z2;
            cubes_buffer[i][31][0] = x8;
            cubes_buffer[i][31][1] = y8;
            cubes_buffer[i][31][2] = z8;
            cubes_buffer[i][32][0] = x4;
            cubes_buffer[i][32][1] = y4;
            cubes_buffer[i][32][2] = z4;
            // 4-8-6
            cubes_buffer[i][33][0] = x4;
            cubes_buffer[i][33][1] = y4;
            cubes_buffer[i][33][2] = z4;
            cubes_buffer[i][34][0] = x8;
            cubes_buffer[i][34][1] = y8;
            cubes_buffer[i][34][2] = z8;
            cubes_buffer[i][35][0] = x6;
            cubes_buffer[i][35][1] = y6;
            cubes_buffer[i][35][2] = z6;
        }

        // Fill cubes_color_buffer
        float siz = max - min;		// Used for gradients
        int index;

        for (int j = 0; j < number_cubes; j++)
        {
            if(labels == nullptr)
            {
                for (int k = 0; k < 12 * 3; k++)    // Go through all the points of all triangles in one box
                {
                    cubes_color_buffer[j][k][0] = 0.1f;			//default_color[0];
                    cubes_color_buffer[j][k][1] = 0.9f;			//default_color[1];
                    cubes_color_buffer[j][k][2] = 0.1f;			//default_color[2];
                    cubes_color_buffer[j][k][3] = alpha_channel;
                }
            }
            else if(array_type == categories)
            {
                index = (int)labels[j] % palette_size;

                for (int k = 0; k < 12 * 3; k++)
                {
                    cubes_color_buffer[j][k][0] = palette[index][0];
                    cubes_color_buffer[j][k][1] = palette[index][1];
                    cubes_color_buffer[j][k][2] = palette[index][2];
                    cubes_color_buffer[j][k][3] = alpha_channel;
                }
            }
            else if(array_type == colors)
            {
                for(int k = 0; k < 12 * 3; k++)
                {
                    cubes_color_buffer[j][k][0] = labels[j * 3 + 0];
                    cubes_color_buffer[j][k][1] = labels[j * 3 + 1];
                    cubes_color_buffer[j][k][2] = labels[j * 3 + 2];
                    cubes_color_buffer[j][k][3] = alpha_channel;
                }
            }
            else if(array_type == gradient)
            {
                if      (labels[j] <= min) index = 0;
                else if (labels[j] >= max) index = palette_size - 1;
                else index = (int)((labels[j] - min) * (palette_size - 1)) / (int)siz;

                for(int k = 0; k < 12 * 3; k++)
                {
                    cubes_color_buffer[j][k][0] = palette[index][0];
                    cubes_color_buffer[j][k][1] = palette[index][1];
                    cubes_color_buffer[j][k][2] = palette[index][2];
                    cubes_color_buffer[j][k][3] = alpha_channel;
                }
            }
        }

        return 0;
    }

    // Send a new palette of colors to replace the current one --------------------------------------
    // Three modes for sending:
    //          - RGB values are in the range [0, 255]
    //          - RGB_01 values are in the range [0, 1]
    //          - HSV (Hue, Saturation, Value): H (int) is in [0, 360], S (double) in [0, 1.], V (double) in [0, 1.]

    int save_palette_RGB_01(float *new_palette, int number_colors)
    {
        if(layer_type == none) { error_message(1); return 1; }

        palette_size = number_colors;
        delete [] palette;
        palette = new float[palette_size][3];

        for(int i = 0; i < palette_size; i++)
        {
            palette[i][0] = new_palette[i * 3 + 0];
            palette[i][1] = new_palette[i * 3 + 1];
            palette[i][2] = new_palette[i * 3 + 2];
        }

        return 0;
    }

    int save_palette_RGB(float *new_palette, int number_colors)
    {
        if(layer_type == none) { error_message(1); return 1; }

        palette_size = number_colors;
        delete [] palette;
        palette = new float[palette_size][3];

        for(int i = 0; i < palette_size; i++)
        {
            palette[i][0] = new_palette[i * 3 + 0] / 255;
            palette[i][1] = new_palette[i * 3 + 1] / 255;
            palette[i][2] = new_palette[i * 3 + 2] / 255;
        }

        return 0;
    }

    int save_palette_HSV(float *new_palette, int number_colors)
    {
        if(layer_type == none) { error_message(1); return 1; }

        palette_size = number_colors;
        delete [] palette;
        palette = new float[palette_size][3];

        for(int i = 0; i < palette_size; i++)
            HSVtoRGB(new_palette[i * 3 + 0], new_palette[i * 3 + 1], new_palette[i * 3 + 2], &palette[i][0]);

        return 0;
    }

    // Bonus functions ------------------------------------------------------------------------------

    // Get a pointer to the first individual element of the vertex buffer or color buffer without needing to check the layer_type (these functions do that for you)
    float* get_vertex_ptr()
    {
        if     (layer_type == points)    return &points_buffer[0][0];
        else if(layer_type == lines)     return &lines_buffer[0][0][0];
        else if(layer_type == triangles) return &triangles_buffer[0][0][0];
        else if(layer_type == cubes)     return &cubes_buffer[0][0][0];
    }
    float* get_colors_ptr()
    {
        if     (layer_type == points)    return &points_color_buffer[0][0];
        else if(layer_type == lines)     return &lines_color_buffer[0][0][0];
        else if(layer_type == triangles) return &triangles_color_buffer[0][0][0];
        else if(layer_type == cubes)     return &cubes_color_buffer[0][0][0];
    }

    // Change the alpha values from the corresponding color buffer
    int set_alpha_channel(float alpha_value)
    {
        if(layer_type == none) { error_message(1); return 1; }
        else if(alpha_value < 0. || alpha_value > 1.) { error_message(6); return 1; }
        else if(alpha_channel == alpha_value) return 0;
        else alpha_channel = alpha_value;

        if (layer_type == points)
        {
            for (int i = 0; i < max_objs; i++)
                points_color_buffer[i][3] = alpha_channel;
        }
        else if (layer_type == lines)
        {
            for (int i = 0; i < max_objs; i++)
            {
                lines_color_buffer[i][0][3] = alpha_channel;
                lines_color_buffer[i][1][3] = alpha_channel;
            }
        }
        else if (layer_type == triangles)
        {
            for (int i = 0; i < max_objs; i++)
            {
                triangles_color_buffer[i][0][3] = alpha_channel;
                triangles_color_buffer[i][1][3] = alpha_channel;
                triangles_color_buffer[i][2][3] = alpha_channel;
            }
        }
        else if (layer_type == cubes)
        {
            for (int i = 0; i < max_objs; i++)
                for (int j = 0; j < 12 * 3; j++)
                    cubes_color_buffer[i][j][3] = alpha_channel;
        }

        return 0;
    }

    void convert_RGB255toRGB(float *colors, int num_colors)
    {
        for (int i = 0; i < num_colors; i++)
        {
            colors[i * 3 + 0] /= 255;
            colors[i * 3 + 1] /= 255;
            colors[i * 3 + 2] /= 255;
        }
    }

    void convert_HSVtoRGB(float *colors, int num_colors)
    {
        for (int i = 0; i < num_colors; i++)
            HSVtoRGB(colors[i * 3 + 0], colors[i * 3 + 1], colors[i * 3 + 2], &colors[i * 3 + 0]);
    }

    void transform_coordinates(float *points_arr, int number_points) {

        float temp;

        for (int i = 0; i < number_points; i++)
        {
            temp = points_arr[i * 3 + 0];

            points_arr[i * 3 + 0] = - points_arr[i * 3 + 1];
            points_arr[i * 3 + 1] =   points_arr[i * 3 + 2];
            points_arr[i * 3 + 2] = - temp;
        }
    }

    // Given some X values for a polynomial, get the Y values. Pass the minimum and maximum x values, and the number of x values to take inside that range (sample_size) (1 will be added at the end). Pass the coefficients of the polynomial (this determines its grade)
    void polynomial_graph(float (*result)[3], float min_x, float max_x, int sample_size, float *coefficients, float number_of_coefficients)
    {
        float step = (max_x - min_x) / sample_size;

        for(int i = 0; i < sample_size; i++)
            polynomial_x(&result[i][0], min_x + step * i, coefficients, number_of_coefficients);

        polynomial_x(&result[sample_size][0], max_x, coefficients, number_of_coefficients);
    }

    void icosahedron(float side_length, float(*points)[3]) {

        float S = side_length;
        const double pi = 3.14159265359;
        float t1 = 2 * pi / 5;
        float t2 = (pi / 2) - t1;
        float t4 = t1 / 2;
        float t3 = t4 - pi / 10;

        float R = (S / 2) / std::sin(t4);
        float H = std::cos(t4) * R;
        float Cx = R * std::sin(t2);
        float Cz = R * std::cos(t2);
        float H1 = std::sqrt(S * S - R * R);
        float H2 = std::sqrt((H + R) * (H + R) - H * H);
        float Y2 = (H2 - H1) / 2;
        float Y1 = Y2 + H1;

        points[0][0] = 0.;
        points[0][1] = Y1;
        points[0][2] = 0.;

        points[1][0] = R;
        points[1][1] = Y2;
        points[1][2] = 0.;

        points[2][0] = Cx;
        points[2][1] = Y2;
        points[2][2] = Cz;

        points[3][0] = -H;
        points[3][1] = Y2;
        points[3][2] = S / 2;

        points[4][0] = -H;
        points[4][1] = Y2;
        points[4][2] = -S / 2;

        points[5][0] = Cx;
        points[5][1] = Y2;
        points[5][2] = -Cz;

        points[6][0] = -R;
        points[6][1] = -Y2;
        points[6][2] = 0.;

        points[7][0] = -Cx;
        points[7][1] = -Y2;
        points[7][2] = -Cz;

        points[8][0] = H;
        points[8][1] = -Y2;
        points[8][2] = -S / 2;

        points[9][0] = H;
        points[9][1] = -Y2;
        points[9][2] = S / 2;

        points[10][0] = -Cx;
        points[10][1] = -Y2;
        points[10][2] = Cz;

        points[11][0] = 0.;
        points[11][1] = -Y1;
        points[11][2] = 0.;
    }

private:    // --------------------------------------------------------------------------------------

    void polynomial_x(float *results, float x, float *coefficients, float number_of_coefficients)
    {
        // X
        results[0] = x;
        // Y
        results[1] = coefficients[0];
        for(int i = 1; i < number_of_coefficients; i++)
            results[1] += coefficients[i] * pow(x, i);
        // Z
        results[2] = 0;
    }

    void error_message(unsigned int code, unsigned int number_objs = 0)
    {
        switch(code)
        {
        case 1:
            std::cout << "The layer \"" << layer_name << "\" is of type \"none\". You cannot send data to it until you initialize it with its constructor" << std::endl;
            break;
        case 2:
            std::cout << "Too many points for the layer \"" << layer_name << "\" (" << number_objs << " > " << max_objs << ')' << std::endl;
            break;
        case 3:
            std::cout << "Too many lines for the buffer of the layer \"" << layer_name << "\" (" << --number_objs << " > " << max_objs << ')' << std::endl;
            break;
        case 4:
            std::cout << "Too many cubes for the buffer of the layer \"" << layer_name << "\" (" << number_objs << " > " << max_objs << ')' << std::endl;
            break;
        case 5:
            std::cout << "The layer \"" << layer_name << "\" is of type \"" << layer_type_str << "\". You cannot send a different type of objects to it" << std::endl;
        case 6:
            std::cout << "Cannot modify the alpha channel of layer \"" << layer_name << "\". The alpha channel must be in the range [0, 1]" << std::endl;
        }
    }

    // Parameters: X, Y (cube's center), x, y (point), rot (radians). It considers x as OpenGL's x, and y as OpenGL's -z.
    void rotation_H(float &x, float &y, float X, float Y, float rot) {

        y = -y;

        float alpha = atan(y / x);
        if (x < 0 && y >= 0) alpha += 3.1415926535f;
        else if (y < 0 && x < 0) alpha += 3.1415926535f;
        else if (y < 0 && x >= 0) alpha += 2.f * 3.1415926535f;

        float hip = sqrt(x * x + y * y);

        float beta = alpha + rot;

        x = hip * cos(beta) + X;
        y = -(hip * sin(beta)) + Y;
    }

    void HSVtoRGB(int H, double S, double V, float output[3])
    {
        // https://gist.github.com/kuathadianto/200148f53616cbd226d993b400214a7f

        double C = S * V;
        double X = C * (1 - abs(fmod(H / 60.0, 2) - 1));
        double m = V - C;
        double Rs, Gs, Bs;

        if (H >= 0 && H < 60) {
            Rs = C;
            Gs = X;
            Bs = 0;
        }
        else if (H >= 60 && H < 120) {
            Rs = X;
            Gs = C;
            Bs = 0;
        }
        else if (H >= 120 && H < 180) {
            Rs = 0;
            Gs = C;
            Bs = X;
        }
        else if (H >= 180 && H < 240) {
            Rs = 0;
            Gs = X;
            Bs = C;
        }
        else if (H >= 240 && H < 300) {
            Rs = X;
            Gs = 0;
            Bs = C;
        }
        else {
            Rs = C;
            Gs = 0;
            Bs = X;
        }

        output[0] = (Rs + m);
        output[1] = (Gs + m);
        output[2] = (Bs + m);
    }

};

#endif
