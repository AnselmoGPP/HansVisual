#include "visualizerClass.hpp"

// Main public members ---------------------------------------------------

visualizerClass::visualizerClass(
	unsigned int point_lay,		std::string *point_names,		unsigned int *max_points,
	unsigned int line_lay,		std::string *line_names,		unsigned int *max_lines,
    unsigned int triangles_lay,	std::string *triangle_names,	unsigned int *max_triangles,
    unsigned int cubes_lay,		std::string *cube_names,		unsigned int *max_cubes ) {

	// Default constructor (no arguments) just creates the default palette colors and the empty vectors and arrays for each object type.

	// Configure the layer scheme
	layer_data = layer_system(
		point_lay, point_names, max_points, 
		line_lay, line_names, max_lines,  
        triangles_lay, triangle_names, max_triangles,
        cubes_lay, cube_names, max_cubes );

     char test2[21] = { 0x3c, 0x3c, 0x3c, 0x20, 0x20, 0x48, 0x61,
                        0x6e, 0x73, 0x56, 0x69, 0x73, 0x75, 0x61,
                        0x6c, 0x20, 0x20, 0x3e, 0x3e, 0x3e, 0x00 };
     for(int i = 0; i < 21; i++) test[i] = test2[i];

	// Create the default palette colors
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

    default_palette = new float[default_palette_size][3];

    for(int i = 0; i < default_palette_size; i++)
    {
        default_palette[i][0] = temp[i][0];
        default_palette[i][1] = temp[i][1];
        default_palette[i][2] = temp[i][2];
    }

	// Set buffers and parameters -----------------------
	unsigned int num_layers;

    checkboxes_values = std::vector<bool*>(num_objects);		// Lets allocate space for 4 sets of checkboxes (points, lines, triangles, cubes)
    objects_to_print =  std::vector<std::vector<size_t>>(num_objects);
    palette_sizes =     std::vector<std::vector<size_t>>(num_objects);
    alpha_channels =    std::vector<std::vector<float>>(num_objects);
    mut =               std::vector<std::mutex*>(num_objects);
    palettes =          std::vector<std::vector<float(*)[3]>>(num_objects);

	// Point buffers
	num_layers = layer_data.point_layers;

    checkboxes_values[points] = new bool[num_layers];
    objects_to_print[points] = std::vector<size_t>(num_layers);
    palette_sizes[points] = std::vector<size_t>(num_layers);
    alpha_channels[points] = std::vector<float>(num_layers);
    mut[points] = new std::mutex[num_layers];

	point_buffers = std::vector<float (*)[3]>(num_layers);
	point_color_buffers = std::vector<float(*)[4]>(num_layers);
    palettes[points] = std::vector<float(*)[3]>(num_layers);

	for (int i = 0; i < num_layers; i++) 
	{
		point_buffers[i] = new float[layer_data.max_points[i]][3];
		point_color_buffers[i] = new float[layer_data.max_points[i]][4];
        palette_sizes[points][i] = default_palette_size;
        palettes[points][i] = new float[palette_sizes[points][i]][3];
		for (int j = 0; j < default_palette_size; j++)
		{
            palettes[points][i][j][0] = default_palette[j][0];
            palettes[points][i][j][1] = default_palette[j][1];
            palettes[points][i][j][2] = default_palette[j][2];
		}
		checkboxes_values[points][i] = true;
        alpha_channels[points][i] = 1.0f;
        objects_to_print[points][i] = 0;
	}

	// Line buffers
	num_layers = layer_data.line_layers;

    checkboxes_values[lines] = new bool(num_layers);
    objects_to_print[lines] = std::vector<size_t>(num_layers);
    palette_sizes[lines] = std::vector<size_t>(num_layers);
    alpha_channels[lines] = std::vector<float>(num_layers);
	mut[lines] = new std::mutex[num_layers];

	line_buffers = std::vector<float(*)[2][3]>(num_layers);
	line_color_buffers = std::vector<float(*)[2][4]>(num_layers);
    palettes[lines] = std::vector<float(*)[3]>(num_layers);

	for (int i = 0; i < num_layers; i++)
	{
		line_buffers[i] = new float[layer_data.max_lines[i]][2][3];
		line_color_buffers[i] = new float[layer_data.max_lines[i]][2][4];
        palette_sizes[lines][i] = default_palette_size;
        palettes[lines][i] = new float[palette_sizes[lines][i]][3];
		for (int j = 0; j < default_palette_size; j++)
		{
            palettes[lines][i][j][0] = default_palette[j][0];
            palettes[lines][i][j][1] = default_palette[j][1];
            palettes[lines][i][j][2] = default_palette[j][2];
		}
		checkboxes_values[lines][i] = true;
        alpha_channels[lines][i] = 1.0f;
        objects_to_print[lines][i] = 0;
	}

	// Triangle buffers
    num_layers = layer_data.triangle_layers;

    checkboxes_values[triangles] = new bool(num_layers);
    objects_to_print[triangles] = std::vector<size_t>(num_layers);
    palette_sizes[triangles] = std::vector<size_t>(num_layers);
    alpha_channels[triangles] = std::vector<float>(num_layers);
	mut[triangles] = new std::mutex[num_layers];

    triangle_buffers = std::vector<float(*)[3][3]>(num_layers);
    triangle_color_buffers = std::vector<float(*)[3][4]>(num_layers);
    palettes[triangles] = std::vector<float(*)[3]>(num_layers);

    for (int i = 0; i < num_layers; i++)
    {
        triangle_buffers[i] = new float[layer_data.max_triangles[i]][3][3];
        triangle_color_buffers[i] = new float[layer_data.max_triangles[i]][3][4];
        palette_sizes[triangles][i] = default_palette_size;
        palettes[triangles][i] = new float[palette_sizes[triangles][i]][3];
        for (int j = 0; j < default_palette_size; j++)
        {
            palettes[triangles][i][j][0] = default_palette[j][0];
            palettes[triangles][i][j][1] = default_palette[j][1];
            palettes[triangles][i][j][2] = default_palette[j][2];
        }
        checkboxes_values[triangles][i] = true;
        alpha_channels[triangles][i] = 0.3f;
        objects_to_print[triangles][i] = 0;
    }

	// Cubes buffers
    num_layers = layer_data.cube_layers;

    checkboxes_values[cubes] = new bool(num_layers);
    objects_to_print[cubes] = std::vector<size_t>(num_layers);
    palette_sizes[cubes] = std::vector<size_t>(num_layers);
    alpha_channels[cubes] = std::vector<float>(num_layers);
	mut[cubes] = new std::mutex[num_layers];

    cube_buffers = std::vector<float(*)[12*3][3]>(num_layers);
    cube_color_buffers = std::vector<float(*)[12*3][4]>(num_layers);
    palettes[cubes] = std::vector<float(*)[3]>(num_layers);

    for (int i = 0; i < num_layers; i++)
    {
        cube_buffers[i] = new float[layer_data.max_cubes[i]][12*3][3];
        cube_color_buffers[i] = new float[layer_data.max_cubes[i]][12*3][4];
        palette_sizes[cubes][i] = default_palette_size;
        palettes[cubes][i] = new float[palette_sizes[cubes][i]][3];
        for (int j = 0; j < default_palette_size; j++)
        {
            palettes[cubes][i][j][0] = default_palette[j][0];
            palettes[cubes][i][j][1] = default_palette[j][1];
            palettes[cubes][i][j][2] = default_palette[j][2];
        }
        checkboxes_values[cubes][i] = true;
        alpha_channels[cubes][i] = 0.3f;
        objects_to_print[cubes][i] = 0;
    }

	// Others
	data_window_size = 1;
    data_window = new std::string[data_window_size];
}

visualizerClass::~visualizerClass(){ 

	for (int i = 0; i < layer_data.point_layers; i++) {
		delete[] point_buffers[i];
		delete[] point_color_buffers[i];
        delete[] palettes[points][i];
	}

	for (int i = 0; i < layer_data.line_layers; i++) {
		delete[] line_buffers[i];
		delete[] line_color_buffers[i];
        delete[] palettes[lines][i];
	}

	for (int i = 0; i < layer_data.triangle_layers; i++) {
		delete[] triangle_buffers[i];
		delete[] triangle_color_buffers[i];
        delete[] palettes[triangles][i];
	}

	for (int i = 0; i < layer_data.cube_layers; i++) {
		delete[] cube_buffers[i];
		delete[] cube_color_buffers[i];
        delete[] palettes[cubes][i];
	}

    for(int i = 0; i < num_objects; i++)
        delete[] checkboxes_values[i];

    delete[] default_palette;
    delete[] data_window;
}

visualizerClass& visualizerClass::operator=(const visualizerClass &obj) {

	for (int i = 0; i < 3; i++) backg_color[i] = obj.backg_color[i];

	unsigned int num_layers;

	// GUI state -----
	show_checkboxes = obj.show_checkboxes;
	show_data = obj.show_data;
	show_options = obj.show_options;

	data_window_size = obj.data_window_size;
	delete[] data_window;
	data_window = new std::string[data_window_size];
	for(int i = 0; i < data_window_size; i++) data_window[i] = obj.data_window[i];


	// Points data -----
    num_layers = obj.layer_data.point_layers;

	objects_to_print[points] = obj.objects_to_print[points];
	palette_sizes[points] = obj.palette_sizes[points];
    alpha_channels[points] = obj.alpha_channels[points];

	delete[] mut[points];
	mut[points] = new std::mutex[num_layers];

	delete[] checkboxes_values[points];
	checkboxes_values[points] = new bool(num_layers);

	for (int i = 0; i < layer_data.point_layers; i++)
	{
		delete[] point_buffers[i];
		delete[] point_color_buffers[i];
		delete[] palettes[points][i];
	}
    point_buffers = std::vector<float(*)[3]>(num_layers);
    point_color_buffers = std::vector<float(*)[4]>(num_layers);
    palettes[points] = std::vector<float(*)[3]>(num_layers);

    for (int i = 0; i < num_layers; i++)
	{
        checkboxes_values[points][i] = obj.checkboxes_values[points][i];

		point_buffers[i] = new float[obj.layer_data.max_points[i]][3];
		point_color_buffers[i] = new float[obj.layer_data.max_points[i]][4];

		for (int j = 0; j < obj.layer_data.max_points[i]; j++)
		{
			point_buffers[i][j][0] = obj.point_buffers[i][j][0];
			point_buffers[i][j][1] = obj.point_buffers[i][j][1];
			point_buffers[i][j][2] = obj.point_buffers[i][j][2];

			point_color_buffers[i][j][0] = obj.point_color_buffers[i][j][0];
			point_color_buffers[i][j][1] = obj.point_color_buffers[i][j][1];
			point_color_buffers[i][j][2] = obj.point_color_buffers[i][j][2];
			point_color_buffers[i][j][3] = obj.point_color_buffers[i][j][3];
		}

        palettes[points][i] = new float[palette_sizes[points][i]][3];

        for (int k = 0; k < palette_sizes[points][i]; k++)
		{
			palettes[points][i][k][0] = obj.palettes[points][i][k][0];
			palettes[points][i][k][1] = obj.palettes[points][i][k][1];
			palettes[points][i][k][2] = obj.palettes[points][i][k][2];
		}
	}


	// Lines data -----
	num_layers = obj.layer_data.line_layers;

	objects_to_print[lines] = obj.objects_to_print[lines];
	palette_sizes[lines] = obj.palette_sizes[lines];
	alpha_channels[lines] = obj.alpha_channels[lines];

	delete[] mut[lines];
	mut[lines] = new std::mutex[num_layers];

	delete[] checkboxes_values[lines];
	checkboxes_values[lines] = new bool(num_layers);

	for (int i = 0; i < layer_data.line_layers; i++)
	{
		delete[] line_buffers[i];
		delete[] line_color_buffers[i];
		delete[] palettes[lines][i];
	}
	line_buffers = std::vector<float(*)[2][3]>(num_layers);
	line_color_buffers = std::vector<float(*)[2][4]>(num_layers);
	palettes[lines] = std::vector<float(*)[3]>(num_layers);

	for (int i = 0; i < num_layers; i++)
	{
		checkboxes_values[lines][i] = obj.checkboxes_values[lines][i];

		line_buffers[i] = new float[obj.layer_data.max_lines[i]][2][3];
		line_color_buffers[i] = new float[obj.layer_data.max_lines[i]][2][4];

		for (int j = 0; j < obj.layer_data.max_lines[i]; j++)
		{
			line_buffers[i][j][0][0] = obj.line_buffers[i][j][0][0];
			line_buffers[i][j][0][1] = obj.line_buffers[i][j][0][1];
			line_buffers[i][j][0][2] = obj.line_buffers[i][j][0][2];

			line_buffers[i][j][1][0] = obj.line_buffers[i][j][1][0];
			line_buffers[i][j][1][1] = obj.line_buffers[i][j][1][1];
			line_buffers[i][j][1][2] = obj.line_buffers[i][j][1][2];

			line_color_buffers[i][j][0][0] = obj.line_color_buffers[i][j][0][0];
			line_color_buffers[i][j][0][1] = obj.line_color_buffers[i][j][0][1];
			line_color_buffers[i][j][0][2] = obj.line_color_buffers[i][j][0][2];
			line_color_buffers[i][j][0][3] = obj.line_color_buffers[i][j][0][3];

			line_color_buffers[i][j][1][0] = obj.line_color_buffers[i][j][1][0];
			line_color_buffers[i][j][1][1] = obj.line_color_buffers[i][j][1][1];
			line_color_buffers[i][j][1][2] = obj.line_color_buffers[i][j][1][2];
			line_color_buffers[i][j][1][3] = obj.line_color_buffers[i][j][1][3];
		}

		palettes[lines][i] = new float[palette_sizes[lines][i]][3];

		for (int k = 0; k < palette_sizes[lines][i]; k++)
		{
			palettes[lines][i][k][0] = obj.palettes[lines][i][k][0];
			palettes[lines][i][k][1] = obj.palettes[lines][i][k][1];
			palettes[lines][i][k][2] = obj.palettes[lines][i][k][2];
		}
	}


	// Triangles data -----
	num_layers = obj.layer_data.triangle_layers;

	objects_to_print[triangles] = obj.objects_to_print[triangles];
	palette_sizes[triangles] = obj.palette_sizes[triangles];
	alpha_channels[triangles] = obj.alpha_channels[triangles];

	delete[] mut[triangles];
	mut[triangles] = new std::mutex[num_layers];

	delete[] checkboxes_values[triangles];
	checkboxes_values[triangles] = new bool(num_layers);

	for (int i = 0; i < layer_data.triangle_layers; i++)
	{
		delete[] triangle_buffers[i];
		delete[] triangle_color_buffers[i];
		delete[] palettes[triangles][i];
	}
	triangle_buffers = std::vector<float(*)[3][3]>(num_layers);
	triangle_color_buffers = std::vector<float(*)[3][4]>(num_layers);
	palettes[triangles] = std::vector<float(*)[3]>(num_layers);

	for (int i = 0; i < num_layers; i++)
	{
		checkboxes_values[triangles][i] = obj.checkboxes_values[triangles][i];

		triangle_buffers[i] = new float[obj.layer_data.max_triangles[i]][3][3];
		triangle_color_buffers[i] = new float[obj.layer_data.max_triangles[i]][3][4];

		for (int j = 0; j < obj.layer_data.max_triangles[i]; j++)
		{
			triangle_buffers[i][j][0][0] = obj.triangle_buffers[i][j][0][0];
			triangle_buffers[i][j][0][1] = obj.triangle_buffers[i][j][0][1];
			triangle_buffers[i][j][0][2] = obj.triangle_buffers[i][j][0][2];

			triangle_buffers[i][j][1][0] = obj.triangle_buffers[i][j][1][0];
			triangle_buffers[i][j][1][1] = obj.triangle_buffers[i][j][1][1];
			triangle_buffers[i][j][1][2] = obj.triangle_buffers[i][j][1][2];

			triangle_buffers[i][j][2][0] = obj.triangle_buffers[i][j][2][0];
			triangle_buffers[i][j][2][1] = obj.triangle_buffers[i][j][2][1];
			triangle_buffers[i][j][2][2] = obj.triangle_buffers[i][j][2][2];

			triangle_color_buffers[i][j][0][0] = obj.triangle_color_buffers[i][j][0][0];
			triangle_color_buffers[i][j][0][1] = obj.triangle_color_buffers[i][j][0][1];
			triangle_color_buffers[i][j][0][2] = obj.triangle_color_buffers[i][j][0][2];
			triangle_color_buffers[i][j][0][3] = obj.triangle_color_buffers[i][j][0][3];

			triangle_color_buffers[i][j][1][0] = obj.triangle_color_buffers[i][j][1][0];
			triangle_color_buffers[i][j][1][1] = obj.triangle_color_buffers[i][j][1][1];
			triangle_color_buffers[i][j][1][2] = obj.triangle_color_buffers[i][j][1][2];
			triangle_color_buffers[i][j][1][3] = obj.triangle_color_buffers[i][j][1][3];

			triangle_color_buffers[i][j][2][0] = obj.triangle_color_buffers[i][j][2][0];
			triangle_color_buffers[i][j][2][1] = obj.triangle_color_buffers[i][j][2][1];
			triangle_color_buffers[i][j][2][2] = obj.triangle_color_buffers[i][j][2][2];
			triangle_color_buffers[i][j][2][3] = obj.triangle_color_buffers[i][j][2][3];
		}

		palettes[triangles][i] = new float[palette_sizes[triangles][i]][3];

		for (int k = 0; k < palette_sizes[triangles][i]; k++)
		{
			palettes[triangles][i][k][0] = obj.palettes[triangles][i][k][0];
			palettes[triangles][i][k][1] = obj.palettes[triangles][i][k][1];
			palettes[triangles][i][k][2] = obj.palettes[triangles][i][k][2];
		}
	}


	// Cubes data -----
	num_layers = obj.layer_data.cube_layers;

	objects_to_print[cubes] = obj.objects_to_print[cubes];
	palette_sizes[cubes] = obj.palette_sizes[cubes];
	alpha_channels[cubes] = obj.alpha_channels[cubes];

	delete[] mut[cubes];
	mut[cubes] = new std::mutex[num_layers];

	delete[] checkboxes_values[cubes];
	checkboxes_values[cubes] = new bool(num_layers);

	for (int i = 0; i < layer_data.cube_layers; i++)
	{
		delete[] cube_buffers[i];
		delete[] cube_color_buffers[i];
		delete[] palettes[cubes][i];
	}
	cube_buffers = std::vector<float(*)[12*3][3]>(num_layers);
	cube_color_buffers = std::vector<float(*)[12*3][4]>(num_layers);
	palettes[cubes] = std::vector<float(*)[3]>(num_layers);

	for (int i = 0; i < num_layers; i++)
	{
		checkboxes_values[cubes][i] = obj.checkboxes_values[cubes][i];

		cube_buffers[i] = new float[obj.layer_data.max_cubes[i]][12*3][3];
		cube_color_buffers[i] = new float[obj.layer_data.max_cubes[i]][12*3][4];

		for(int cub = 0; cub < obj.layer_data.max_cubes[i]; cub++)
			for (int vert = 0; vert < 12*3; vert++)
			{
				cube_buffers[i][cub][vert][0] = obj.cube_buffers[i][cub][vert][0];
				cube_buffers[i][cub][vert][1] = obj.cube_buffers[i][cub][vert][1];
				cube_buffers[i][cub][vert][2] = obj.cube_buffers[i][cub][vert][2];

				cube_color_buffers[i][cub][vert][0] = obj.cube_color_buffers[i][cub][vert][0];
				cube_color_buffers[i][cub][vert][1] = obj.cube_color_buffers[i][cub][vert][1];
				cube_color_buffers[i][cub][vert][2] = obj.cube_color_buffers[i][cub][vert][2];
				cube_color_buffers[i][cub][vert][3] = obj.cube_color_buffers[i][cub][vert][3];
			}

		palettes[cubes][i] = new float[palette_sizes[cubes][i]][3];

		for (int k = 0; k < palette_sizes[cubes][i]; k++)
		{
			palettes[cubes][i][k][0] = obj.palettes[cubes][i][k][0];
			palettes[cubes][i][k][1] = obj.palettes[cubes][i][k][1];
			palettes[cubes][i][k][2] = obj.palettes[cubes][i][k][2];
		}
	}


	// Layers data -----
	layer_data = obj.layer_data;

    return *this;
}

int visualizerClass::open_window() {

    // Initialize GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);								// x4 antialiasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// GLFW version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    std::thread running(&visualizerClass::run_thread, this);
    running.detach();
    return 0;
}

void visualizerClass::add_layer(object_type type, const char *name, unsigned int capacity) {

    std::string layer_name = name;
	int siz;

	float(*new_palette)[3] = new float[default_palette_size][3];
	for (int i = 0; i < default_palette_size; i++) {
		new_palette[i][0] = default_palette[i][0];
		new_palette[i][1] = default_palette[i][1];
		new_palette[i][2] = default_palette[i][2];
	}

	if (type == points)
	{
		int siz = layer_data.point_layers;

		// Layer system object
		layer_data.point_layers++;
		layer_data.point_layers_names.push_back(layer_name);
		layer_data.max_points.push_back(capacity);

		// Checkboxes
		bool *temp = new bool(siz + 1);
		for (int i = 0; i < siz; i++) temp[i] = checkboxes_values[points][i];
		temp[siz] = true;
		delete[] checkboxes_values[points];
		checkboxes_values[points] = temp;

		// Parameters
		objects_to_print[points].push_back(0);
		palette_sizes[points].push_back(default_palette_size);
		alpha_channels[points].push_back(1.0f);

		delete[] mut[points];
		mut[points] = new std::mutex[siz + 1];

		// Buffers
		palettes[points].push_back(new_palette);

		float(*new_buff)[3] = new float[capacity][3];
		point_buffers.push_back(new_buff);

		float(*new_col)[4] = new float[capacity][4];
		point_color_buffers.push_back(new_col);
	}
	if (type == lines)
	{
		int siz = layer_data.line_layers;

		// Layer system object
		layer_data.line_layers++;
		layer_data.line_layers_names.push_back(layer_name);
		layer_data.max_lines.push_back(capacity);

		// Checkboxes
		bool *temp = new bool(siz + 1);
		for (int i = 0; i < siz; i++) temp[i] = checkboxes_values[lines][i];
		temp[siz] = true;
		delete[] checkboxes_values[lines];
		checkboxes_values[lines] = temp;

		// Parameters
		objects_to_print[lines].push_back(0);
		palette_sizes[lines].push_back(default_palette_size);
		alpha_channels[lines].push_back(1.0f);

		delete[] mut[lines];
		mut[lines] = new std::mutex[siz + 1];

		// Buffers
		palettes[lines].push_back(new_palette);

		float(*new_buff)[2][3] = new float[capacity][2][3];
		line_buffers.push_back(new_buff);

		float(*new_col)[2][4] = new float[capacity][2][4];
		line_color_buffers.push_back(new_col);
	}
	if (type == triangles)
	{
		int siz = layer_data.triangle_layers;

		// Layer system object
		layer_data.triangle_layers++;
		layer_data.triangle_layers_names.push_back(layer_name);
		layer_data.max_triangles.push_back(capacity);

		// Checkboxes
		bool *temp = new bool(siz + 1);
		for (int i = 0; i < siz; i++) temp[i] = checkboxes_values[triangles][i];
		temp[siz] = true;
		delete[] checkboxes_values[triangles];
		checkboxes_values[triangles] = temp;

		// Parameters
		objects_to_print[triangles].push_back(0);
		palette_sizes[triangles].push_back(default_palette_size);
		alpha_channels[triangles].push_back(0.3f);

		delete[] mut[triangles];
		mut[triangles] = new std::mutex[siz + 1];

		// Buffers
		palettes[triangles].push_back(new_palette);

		float(*new_buff)[3][3] = new float[capacity][3][3];
		triangle_buffers.push_back(new_buff);

		float(*new_col)[3][4] = new float[capacity][3][4];
		triangle_color_buffers.push_back(new_col);
	}
	if (type == cubes)
	{
		int siz = layer_data.triangle_layers;

		// Layer system object
		layer_data.cube_layers++;
		layer_data.cube_layers_names.push_back(layer_name);
		layer_data.max_cubes.push_back(capacity);

		// Checkboxes
		bool *temp = new bool(siz + 1);
		for (int i = 0; i < siz; i++) temp[i] = checkboxes_values[cubes][i];
		temp[siz] = true;
		delete[] checkboxes_values[cubes];
		checkboxes_values[cubes] = temp;

		// Parameters
		objects_to_print[cubes].push_back(0);
		palette_sizes[cubes].push_back(default_palette_size);
		alpha_channels[cubes].push_back(0.3f);

		delete[] mut[cubes];
		mut[cubes] = new std::mutex[siz + 1];

		// Buffers
		palettes[cubes].push_back(new_palette);

		float(*new_buff)[12 * 3][3] = new float[capacity][12 * 3][3];
		cube_buffers.push_back(new_buff);

		float(*new_col)[12 * 3][4] = new float[capacity][12 * 3][4];
		cube_color_buffers.push_back(new_col);
	}
}

void visualizerClass::send_points(std::string layer_name, int number_points, const float *arr, const float *labels, data_buffer array_type, float min, float max) {

	// Look for the corresponding point layer and its parameters
	unsigned int layer, max_points;

	if (get_layer_data(layer_name, points, layer, max_points)) return;

	// Get the number of points the user wants to show on screen. Check whether the layer maximum size is being reached and, if it is, write to buffer only the maximum possible number of them.
    if (number_points > max_points) {
        std::cout << "Too many points for the buffer of the layer \"" << layer_name << "\" (" << number_points << " > " << max_points << ')' << std::endl;
        objects_to_print[points][layer] = max_points;
    }
    else objects_to_print[points][layer] = number_points;


	// Write data to buffers (points and colors)
    float siz = max - min;		// Used for gradients
    int index;

    std::lock_guard<std::mutex> lock_points(mut[points][layer]);

    for (int i = 0; i < objects_to_print[points][layer]; i++)			// Main loop for filling the corresponding points buffer
    {
        // Set point position in 3D;
        point_buffers[layer][i][0] = arr[i * 3 + 0];
		point_buffers[layer][i][1] = arr[i * 3 + 1];
		point_buffers[layer][i][2] = arr[i * 3 + 2];

        // Set color of the point
        if		(labels     == nullptr)
        {
			point_color_buffers[layer][i][0] = 0;
			point_color_buffers[layer][i][1] = 0;
			point_color_buffers[layer][i][2] = 0;
            point_color_buffers[layer][i][3] = alpha_channels[points][layer];
        }
        else if (array_type == categories)
        {
            index = (int)labels[i] % palette_sizes[points][layer];

            point_color_buffers[layer][i][0] = palettes[points][layer][index][0];
            point_color_buffers[layer][i][1] = palettes[points][layer][index][1];
            point_color_buffers[layer][i][2] = palettes[points][layer][index][2];
            point_color_buffers[layer][i][3] = alpha_channels[points][layer];
        }
        else if	(array_type == colors)
        {
            point_color_buffers[layer][i][0] = labels[i * 3 + 0];
            point_color_buffers[layer][i][1] = labels[i * 3 + 1];
            point_color_buffers[layer][i][2] = labels[i * 3 + 2];
            point_color_buffers[layer][i][3] = alpha_channels[points][layer];
        }
        else if (array_type == gradient)
        {
            if      (labels[i] <= min) index = 0;
            else if (labels[i] >= max) index = palette_sizes[points][layer] - 1;
            else {
                index = (int)((labels[i] - min) * (palette_sizes[points][layer] - 1)) / (int)siz;
            }

            point_color_buffers[layer][i][0] = palettes[points][layer][index][0];
            point_color_buffers[layer][i][1] = palettes[points][layer][index][1];
            point_color_buffers[layer][i][2] = palettes[points][layer][index][2];
            point_color_buffers[layer][i][3] = alpha_channels[points][layer];
        }
    }
}

void visualizerClass::send_lines(std::string layer_name, int number_points, const float *arr, const float *labels, data_buffer array_type, float min, float max) {

	// Look for the corresponding line layer and its parameters
	unsigned int layer, max_lines;

	if (get_layer_data(layer_name, lines, layer, max_lines)) return;

	// Get the number of lines the user wants to show on screen. Check whether the layer maximum size is being reached and, if it is, write to buffer only the maximum possible number of them.
    if (number_points - 1 > max_lines) {
        std::cout << "Too many lines for the buffer of the layer \"" << layer_name << "\" (" << --number_points << " > " << max_lines << ')' << std::endl;
        objects_to_print[lines][layer] = max_lines;
	}
    else objects_to_print[lines][layer] = --number_points;

    // Write data to buffers (points and colors)
	float siz = max - min;		// Used for gradients
	int index = 0;
	int omitted_segments = 0;

    std::lock_guard<std::mutex> lock_points(mut[lines][layer]);

    for (int i = 0; i < objects_to_print[lines][layer]; i++)		// Main loop for filling the corresponding lines buffer
    {
        // Check for the line jump
        if (arr[i * 3 + 3] == 1.2f && arr[i * 3 + 4] == 3.4f && arr[i * 3 + 5] == 5.6f) {
            omitted_segments += 2;
            i++;
            continue;
        }

        // Save two points (vertex) for each segment
        line_buffers[layer][i - omitted_segments][0][0] = arr[i * 3 + 0];
        line_buffers[layer][i - omitted_segments][0][1] = arr[i * 3 + 1];
        line_buffers[layer][i - omitted_segments][0][2] = arr[i * 3 + 2];

        line_buffers[layer][i - omitted_segments][1][0] = arr[i * 3 + 3];
        line_buffers[layer][i - omitted_segments][1][1] = arr[i * 3 + 4];
        line_buffers[layer][i - omitted_segments][1][2] = arr[i * 3 + 5];

        // Save colors for each vertex
        if		(labels == nullptr)
        {
            line_color_buffers[layer][i - omitted_segments][0][0] = 0;
            line_color_buffers[layer][i - omitted_segments][0][1] = 0;
            line_color_buffers[layer][i - omitted_segments][0][2] = 0;
            line_color_buffers[layer][i - omitted_segments][0][3] = alpha_channels[lines][layer];

            line_color_buffers[layer][i - omitted_segments][1][0] = 0;
            line_color_buffers[layer][i - omitted_segments][1][1] = 0;
            line_color_buffers[layer][i - omitted_segments][1][2] = 0;
            line_color_buffers[layer][i - omitted_segments][1][3] = alpha_channels[lines][layer];
        }
        else if (array_type == categories)  // labels contains the category of each segment (including line jumps)
        {
            index = (int)labels[i] % palette_sizes[lines][layer];

            line_color_buffers[layer][i - omitted_segments][0][0] = palettes[lines][layer][index][0];
            line_color_buffers[layer][i - omitted_segments][0][1] = palettes[lines][layer][index][1];
            line_color_buffers[layer][i - omitted_segments][0][2] = palettes[lines][layer][index][2];
            line_color_buffers[layer][i - omitted_segments][0][3] = alpha_channels[lines][layer];

            line_color_buffers[layer][i - omitted_segments][1][0] = palettes[lines][layer][index][0];
            line_color_buffers[layer][i - omitted_segments][1][1] = palettes[lines][layer][index][1];
            line_color_buffers[layer][i - omitted_segments][1][2] = palettes[lines][layer][index][2];
            line_color_buffers[layer][i - omitted_segments][1][3] = alpha_channels[lines][layer];
        }
        else if	(array_type == colors)      // labels contains the color of each segment (including line jumps)
        {
            line_color_buffers[layer][i - omitted_segments][0][0] = labels[i * 3 + 0];
            line_color_buffers[layer][i - omitted_segments][0][1] = labels[i * 3 + 1];
            line_color_buffers[layer][i - omitted_segments][0][2] = labels[i * 3 + 2];
            line_color_buffers[layer][i - omitted_segments][0][3] = alpha_channels[lines][layer];

            line_color_buffers[layer][i - omitted_segments][1][0] = labels[i * 3 + 0];
            line_color_buffers[layer][i - omitted_segments][1][1] = labels[i * 3 + 1];
            line_color_buffers[layer][i - omitted_segments][1][2] = labels[i * 3 + 2];
            line_color_buffers[layer][i - omitted_segments][1][3] = alpha_channels[lines][layer];
        }
        else if (array_type == gradient)
        {
			if (labels[i] <= min) index = 0;
            else if (labels[i] >= max) index = palette_sizes[lines][layer] - 1;
			else {
                index = ((labels[i] - min) * (palette_sizes[lines][layer] - 1)) / siz;
			}

            line_color_buffers[layer][i - omitted_segments][0][0] = palettes[lines][layer][index][0];
            line_color_buffers[layer][i - omitted_segments][0][1] = palettes[lines][layer][index][1];
            line_color_buffers[layer][i - omitted_segments][0][2] = palettes[lines][layer][index][2];
            line_color_buffers[layer][i - omitted_segments][0][3] = alpha_channels[lines][layer];

            line_color_buffers[layer][i - omitted_segments][1][0] = palettes[lines][layer][index][0];
            line_color_buffers[layer][i - omitted_segments][1][1] = palettes[lines][layer][index][1];
            line_color_buffers[layer][i - omitted_segments][1][2] = palettes[lines][layer][index][2];
            line_color_buffers[layer][i - omitted_segments][1][3] = alpha_channels[lines][layer];
        }
    }
}

void visualizerClass::send_triangles(std::string layer_name, int number_triangles, const float *arr, const float *labels, data_buffer array_type, float min, float max) {

    // Look for the corresponding triangle layer and its parameters
    unsigned int layer, max_triangles;

    if (get_layer_data(layer_name, triangles, layer, max_triangles)) return;

    // Get the number of triangles the user wants to show on screen. Check whether the layer maximum size is being reached and, if it is, write to buffer only the maximum possible number of them.
    if (number_triangles > max_triangles) {
        std::cout << "Too many points for the buffer of the layer \"" << layer_name << "\" (" << number_triangles << " > " << max_triangles << ')' << std::endl;
        objects_to_print[triangles][layer] = max_triangles;
    }
    else objects_to_print[triangles][layer] = number_triangles;


    // Write data to buffers (points and colors)
    float siz = max - min;		// Used for gradients
    int index;

    std::lock_guard<std::mutex> lock_points(mut[triangles][layer]);

    for (int i = 0; i < objects_to_print[triangles][layer]; i++)			// Main loop for filling the corresponding points buffer
    {
        // Set triangle position in 3D;
        triangle_buffers[layer][i][0][0] = arr[i * 9 + 0];
        triangle_buffers[layer][i][0][1] = arr[i * 9 + 1];
        triangle_buffers[layer][i][0][2] = arr[i * 9 + 2];

        triangle_buffers[layer][i][1][0] = arr[i * 9 + 3];
        triangle_buffers[layer][i][1][1] = arr[i * 9 + 4];
        triangle_buffers[layer][i][1][2] = arr[i * 9 + 5];

        triangle_buffers[layer][i][2][0] = arr[i * 9 + 6];
        triangle_buffers[layer][i][2][1] = arr[i * 9 + 7];
        triangle_buffers[layer][i][2][2] = arr[i * 9 + 8];

        // Set color of the point
        if		(labels     == nullptr)
        {
            triangle_color_buffers[layer][i][0][0] = 0;
            triangle_color_buffers[layer][i][0][1] = 0;
            triangle_color_buffers[layer][i][0][2] = 0;
            triangle_color_buffers[layer][i][0][3] = alpha_channels[triangles][layer];

            triangle_color_buffers[layer][i][1][0] = 0;
            triangle_color_buffers[layer][i][1][1] = 0;
            triangle_color_buffers[layer][i][1][2] = 0;
            triangle_color_buffers[layer][i][1][3] = alpha_channels[triangles][layer];

            triangle_color_buffers[layer][i][2][0] = 0;
            triangle_color_buffers[layer][i][2][1] = 0;
            triangle_color_buffers[layer][i][2][2] = 0;
            triangle_color_buffers[layer][i][2][3] = alpha_channels[triangles][layer];
        }
        else if (array_type == categories)
        {
            index = (int)labels[i] % palette_sizes[triangles][layer];

            triangle_color_buffers[layer][i][0][0] = palettes[triangles][layer][index][0];
            triangle_color_buffers[layer][i][0][1] = palettes[triangles][layer][index][1];
            triangle_color_buffers[layer][i][0][2] = palettes[triangles][layer][index][2];
            triangle_color_buffers[layer][i][0][3] = alpha_channels[triangles][layer];

            triangle_color_buffers[layer][i][1][0] = palettes[triangles][layer][index][0];
            triangle_color_buffers[layer][i][1][1] = palettes[triangles][layer][index][1];
            triangle_color_buffers[layer][i][1][2] = palettes[triangles][layer][index][2];
            triangle_color_buffers[layer][i][1][3] = alpha_channels[triangles][layer];

            triangle_color_buffers[layer][i][2][0] = palettes[triangles][layer][index][0];
            triangle_color_buffers[layer][i][2][1] = palettes[triangles][layer][index][1];
            triangle_color_buffers[layer][i][2][2] = palettes[triangles][layer][index][2];
            triangle_color_buffers[layer][i][2][3] = alpha_channels[triangles][layer];
        }
        else if	(array_type == colors)
        {
            triangle_color_buffers[layer][i][0][0] = labels[i * 3 + 0];
            triangle_color_buffers[layer][i][0][1] = labels[i * 3 + 1];
            triangle_color_buffers[layer][i][0][2] = labels[i * 3 + 2];
            triangle_color_buffers[layer][i][0][3] = alpha_channels[triangles][layer];

            triangle_color_buffers[layer][i][1][0] = labels[i * 3 + 0];
            triangle_color_buffers[layer][i][1][1] = labels[i * 3 + 1];
            triangle_color_buffers[layer][i][1][2] = labels[i * 3 + 2];
            triangle_color_buffers[layer][i][1][3] = alpha_channels[triangles][layer];

            triangle_color_buffers[layer][i][2][0] = labels[i * 3 + 0];
            triangle_color_buffers[layer][i][2][1] = labels[i * 3 + 1];
            triangle_color_buffers[layer][i][2][2] = labels[i * 3 + 2];
            triangle_color_buffers[layer][i][2][3] = alpha_channels[triangles][layer];
        }
        else if (array_type == gradient)
        {
            if      (labels[i] <= min) index = 0;
            else if (labels[i] >= max) index = palette_sizes[triangles][layer] - 1;
            else {
                index = (int)((labels[i] - min) * (palette_sizes[triangles][layer] - 1)) / (int)siz;
            }

            triangle_color_buffers[layer][i][0][0] = palettes[triangles][layer][index][0];
            triangle_color_buffers[layer][i][0][1] = palettes[triangles][layer][index][1];
            triangle_color_buffers[layer][i][0][2] = palettes[triangles][layer][index][2];
            triangle_color_buffers[layer][i][0][3] = alpha_channels[triangles][layer];

            triangle_color_buffers[layer][i][1][0] = palettes[triangles][layer][index][0];
            triangle_color_buffers[layer][i][1][1] = palettes[triangles][layer][index][1];
            triangle_color_buffers[layer][i][1][2] = palettes[triangles][layer][index][2];
            triangle_color_buffers[layer][i][1][3] = alpha_channels[triangles][layer];

            triangle_color_buffers[layer][i][2][0] = palettes[triangles][layer][index][0];
            triangle_color_buffers[layer][i][2][1] = palettes[triangles][layer][index][1];
            triangle_color_buffers[layer][i][2][2] = palettes[triangles][layer][index][2];
            triangle_color_buffers[layer][i][2][3] = alpha_channels[triangles][layer];
        }
    }
}

void visualizerClass::send_cubes(std::string layer_name, int number_cubes, const cube3D *arr, const float *labels, data_buffer array_type, float min, float max) {

    // Look for the corresponding cube layer and its parameters
    unsigned int layer, max_cubes;

    if (get_layer_data(layer_name, cubes, layer, max_cubes)) return;

    // Get the number of cubes the user wants to show on screen. Check whether the layer maximum size is being reached and, if it is, write to buffer only the maximum possible number of them.
    if (number_cubes > max_cubes) {
        std::cout << "Too many cubes for the buffer of the layer \"" << layer_name << "\" (" << number_cubes << " > " << max_cubes << ')' << std::endl;
        objects_to_print[cubes][layer] = max_cubes;
    }
    else objects_to_print[cubes][layer] = number_cubes;

    // Write data to buffers (cubes and colors)
    float X, Y, Z, x, y, z, rot_H, rot_V;

    std::lock_guard<std::mutex> lock_points(mut[cubes][layer]);

    // Fill cubes_buffer                            >>> This draws triangles anti-clockwise
    for (size_t i = 0; i < objects_to_print[cubes][layer]; i++) {

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
        cube_buffers[layer][i][0][0] = x1;
        cube_buffers[layer][i][0][1] = y1;
        cube_buffers[layer][i][0][2] = z1;
        cube_buffers[layer][i][1][0] = x2;
        cube_buffers[layer][i][1][1] = y2;
        cube_buffers[layer][i][1][2] = z2;
        cube_buffers[layer][i][2][0] = x3;
        cube_buffers[layer][i][2][1] = y3;
        cube_buffers[layer][i][2][2] = z3;
        // 2-4-3
        cube_buffers[layer][i][3][0] = x2;
        cube_buffers[layer][i][3][1] = y2;
        cube_buffers[layer][i][3][2] = z2;
        cube_buffers[layer][i][4][0] = x4;
        cube_buffers[layer][i][4][1] = y4;
        cube_buffers[layer][i][4][2] = z4;
        cube_buffers[layer][i][5][0] = x3;
        cube_buffers[layer][i][5][1] = y3;
        cube_buffers[layer][i][5][2] = z3;
        // 3-4-5
        cube_buffers[layer][i][6][0] = x3;
        cube_buffers[layer][i][6][1] = y3;
        cube_buffers[layer][i][6][2] = z3;
        cube_buffers[layer][i][7][0] = x4;
        cube_buffers[layer][i][7][1] = y4;
        cube_buffers[layer][i][7][2] = z4;
        cube_buffers[layer][i][8][0] = x5;
        cube_buffers[layer][i][8][1] = y5;
        cube_buffers[layer][i][8][2] = z5;
        // 4-6-5
        cube_buffers[layer][i][9][0] = x4;
        cube_buffers[layer][i][9][1] = y4;
        cube_buffers[layer][i][9][2] = z4;
        cube_buffers[layer][i][10][0] = x6;
        cube_buffers[layer][i][10][1] = y6;
        cube_buffers[layer][i][10][2] = z6;
        cube_buffers[layer][i][11][0] = x5;
        cube_buffers[layer][i][11][1] = y5;
        cube_buffers[layer][i][11][2] = z5;
        // 5-6-7
        cube_buffers[layer][i][12][0] = x5;
        cube_buffers[layer][i][12][1] = y5;
        cube_buffers[layer][i][12][2] = z5;
        cube_buffers[layer][i][13][0] = x6;
        cube_buffers[layer][i][13][1] = y6;
        cube_buffers[layer][i][13][2] = z6;
        cube_buffers[layer][i][14][0] = x7;
        cube_buffers[layer][i][14][1] = y7;
        cube_buffers[layer][i][14][2] = z7;
        // 6-8-7
        cube_buffers[layer][i][15][0] = x6;
        cube_buffers[layer][i][15][1] = y6;
        cube_buffers[layer][i][15][2] = z6;
        cube_buffers[layer][i][16][0] = x8;
        cube_buffers[layer][i][16][1] = y8;
        cube_buffers[layer][i][16][2] = z8;
        cube_buffers[layer][i][17][0] = x7;
        cube_buffers[layer][i][17][1] = y7;
        cube_buffers[layer][i][17][2] = z7;
        // 7-8-1
        cube_buffers[layer][i][18][0] = x7;
        cube_buffers[layer][i][18][1] = y7;
        cube_buffers[layer][i][18][2] = z7;
        cube_buffers[layer][i][19][0] = x8;
        cube_buffers[layer][i][19][1] = y8;
        cube_buffers[layer][i][19][2] = z8;
        cube_buffers[layer][i][20][0] = x1;
        cube_buffers[layer][i][20][1] = y1;
        cube_buffers[layer][i][20][2] = z1;
        // 8-2-1
        cube_buffers[layer][i][21][0] = x8;
        cube_buffers[layer][i][21][1] = y8;
        cube_buffers[layer][i][21][2] = z8;
        cube_buffers[layer][i][22][0] = x2;
        cube_buffers[layer][i][22][1] = y2;
        cube_buffers[layer][i][22][2] = z2;
        cube_buffers[layer][i][23][0] = x1;
        cube_buffers[layer][i][23][1] = y1;
        cube_buffers[layer][i][23][2] = z1;
        // 1-3-7
        cube_buffers[layer][i][24][0] = x1;
        cube_buffers[layer][i][24][1] = y1;
        cube_buffers[layer][i][24][2] = z1;
        cube_buffers[layer][i][25][0] = x3;
        cube_buffers[layer][i][25][1] = y3;
        cube_buffers[layer][i][25][2] = z3;
        cube_buffers[layer][i][26][0] = x7;
        cube_buffers[layer][i][26][1] = y7;
        cube_buffers[layer][i][26][2] = z7;
        // 3-5-7
        cube_buffers[layer][i][27][0] = x3;
        cube_buffers[layer][i][27][1] = y3;
        cube_buffers[layer][i][27][2] = z3;
        cube_buffers[layer][i][28][0] = x5;
        cube_buffers[layer][i][28][1] = y5;
        cube_buffers[layer][i][28][2] = z5;
        cube_buffers[layer][i][29][0] = x7;
        cube_buffers[layer][i][29][1] = y7;
        cube_buffers[layer][i][29][2] = z7;
        // 2-8-4
        cube_buffers[layer][i][30][0] = x2;
        cube_buffers[layer][i][30][1] = y2;
        cube_buffers[layer][i][30][2] = z2;
        cube_buffers[layer][i][31][0] = x8;
        cube_buffers[layer][i][31][1] = y8;
        cube_buffers[layer][i][31][2] = z8;
        cube_buffers[layer][i][32][0] = x4;
        cube_buffers[layer][i][32][1] = y4;
        cube_buffers[layer][i][32][2] = z4;
        // 4-8-6
        cube_buffers[layer][i][33][0] = x4;
        cube_buffers[layer][i][33][1] = y4;
        cube_buffers[layer][i][33][2] = z4;
        cube_buffers[layer][i][34][0] = x8;
        cube_buffers[layer][i][34][1] = y8;
        cube_buffers[layer][i][34][2] = z8;
        cube_buffers[layer][i][35][0] = x6;
        cube_buffers[layer][i][35][1] = y6;
        cube_buffers[layer][i][35][2] = z6;
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
                cube_color_buffers[layer][j][k][0] = 0.1f;
                cube_color_buffers[layer][j][k][1] = 0.9f;
                cube_color_buffers[layer][j][k][2] = 0.1f;
                cube_color_buffers[layer][j][k][3] = alpha_channels[cubes][layer];
            }
        }
        else if(array_type == categories)
        {
            index = (int)labels[j] % palette_sizes[cubes][layer];

            for (int k = 0; k < 12 * 3; k++)
            {
                cube_color_buffers[layer][j][k][0] = palettes[cubes][layer][index][0];
                cube_color_buffers[layer][j][k][1] = palettes[cubes][layer][index][1];
                cube_color_buffers[layer][j][k][2] = palettes[cubes][layer][index][2];
                cube_color_buffers[layer][j][k][3] = alpha_channels[cubes][layer];
            }
        }
        else if(array_type == colors)
        {
            for(int k = 0; k < 12 * 3; k++)
            {
                cube_color_buffers[layer][j][k][0] = labels[j * 3 + 0];
                cube_color_buffers[layer][j][k][1] = labels[j * 3 + 1];
                cube_color_buffers[layer][j][k][2] = labels[j * 3 + 2];
                cube_color_buffers[layer][j][k][3] = alpha_channels[cubes][layer];
            }
        }
        else if(array_type == gradient)
        {
            if      (labels[j] <= min) index = 0;
            else if (labels[j] >= max) index = palette_sizes[cubes][layer] - 1;
            else index = (int)((labels[j] - min) * (palette_sizes[cubes][layer] - 1)) / (int)siz;

            for(int k = 0; k < 12 * 3; k++)
            {
                cube_color_buffers[layer][j][k][0] = palettes[cubes][layer][index][0];
                cube_color_buffers[layer][j][k][1] = palettes[cubes][layer][index][1];
                cube_color_buffers[layer][j][k][2] = palettes[cubes][layer][index][2];
                cube_color_buffers[layer][j][k][3] = alpha_channels[cubes][layer];
            }
        }
    }
}

// Auxiliary public members ----------------------------------------------

void visualizerClass::transform_coordinates(float *points_arr, int number_points) {

	float temp;

	for (int i = 0; i < number_points; i++)
	{
		temp = points_arr[i * 3 + 0];

		points_arr[i * 3 + 0] = - points_arr[i * 3 + 1];
		points_arr[i * 3 + 1] =   points_arr[i * 3 + 2];
		points_arr[i * 3 + 2] = - temp;
	}
}

void visualizerClass::send_palette_RGB_01(std::string layer_name, object_type obj, float *new_palette, int number_colors){

    unsigned int layer, max_elements;

    if(obj == points)
    {
		if (get_layer_data(layer_name, points, layer, max_elements)) {
            std::cout << "Cannot change points palette" << std::endl;
			return;
		}

        delete [] palettes[points][layer];

        palettes[points][layer] = new float[number_colors][3];

        for(int i = 0; i < number_colors; i++){
            palettes[points][layer][i][0] = new_palette[i * 3 + 0];
            palettes[points][layer][i][1] = new_palette[i * 3 + 1];
            palettes[points][layer][i][2] = new_palette[i * 3 + 2];
        }

        palette_sizes[points][layer] = number_colors;
    }
    else if(obj == lines)
    {
		if (get_layer_data(layer_name, lines, layer, max_elements)) {
            std::cout << "Cannot change lines palette" << std::endl;
			return;
		}

        delete [] palettes[lines][layer];
        palettes[lines][layer] = new float[number_colors][3];

        for(int i = 0; i < number_colors; i++){
            palettes[lines][layer][i][0] = new_palette[i * 3 + 0];
            palettes[lines][layer][i][1] = new_palette[i * 3 + 1];
            palettes[lines][layer][i][2] = new_palette[i * 3 + 2];
        }

        palette_sizes[lines][layer] = number_colors;
    }
    else if (obj == triangles)
    {
        if (get_layer_data(layer_name, triangles, layer, max_elements)) {
            std::cout << "Cannot change triangles palette" << std::endl;
            return;
        }

        delete [] palettes[triangles][layer];
        palettes[triangles][layer] = new float[number_colors][3];

        for(int i = 0; i < number_colors; i++){
            palettes[triangles][layer][i][0] = new_palette[i * 3 + 0];
            palettes[triangles][layer][i][1] = new_palette[i * 3 + 1];
            palettes[triangles][layer][i][2] = new_palette[i * 3 + 2];
        }

        palette_sizes[triangles][layer] = number_colors;
    }
    else if(obj == cubes)
    {
		if (get_layer_data(layer_name, cubes, layer, max_elements)) {
            std::cout << "Cannot change cubes palette" << std::endl;
			return;
		}

        delete [] palettes[cubes][layer];
        palettes[cubes][layer] = new float[number_colors][3];

        for(int i = 0; i < number_colors; i++){
            palettes[cubes][layer][i][0] = new_palette[i * 3 + 0];
            palettes[cubes][layer][i][1] = new_palette[i * 3 + 1];
            palettes[cubes][layer][i][2] = new_palette[i * 3 + 2];
        }

        palette_sizes[cubes][layer] = number_colors;
    }
}

void visualizerClass::send_palette_RGB(std::string layer_name, object_type obj, float *new_palette, int number_colors) {

	unsigned int layer, max_elements;

	if (obj == points)
	{
		if (get_layer_data(layer_name, points, layer, max_elements)) {
			std::cout << "Cannot change points palette" << std::endl;
			return;
		}

        delete[] palettes[points][layer];

        palettes[points][layer] = new float[number_colors][3];

		for (int i = 0; i < number_colors; i++) {
            palettes[points][layer][i][0] = new_palette[i * 3 + 0] / 255;
            palettes[points][layer][i][1] = new_palette[i * 3 + 1] / 255;
            palettes[points][layer][i][2] = new_palette[i * 3 + 2] / 255;
		}

        palette_sizes[points][layer] = number_colors;
	}
	else if (obj == lines)
	{
		if (get_layer_data(layer_name, lines, layer, max_elements)) {
			std::cout << "Cannot change lines palette" << std::endl;
			return;
		}

        delete[] palettes[lines][layer];
        palettes[lines][layer] = new float[number_colors][3];

		for (int i = 0; i < number_colors; i++) {
            palettes[lines][layer][i][0] = new_palette[i * 3 + 0] / 255;
            palettes[lines][layer][i][1] = new_palette[i * 3 + 1] / 255;
            palettes[lines][layer][i][2] = new_palette[i * 3 + 2] / 255;
		}

        palette_sizes[lines][layer] = number_colors;
	}
	else if (obj == triangles)
	{
		if (get_layer_data(layer_name, triangles, layer, max_elements)) {
			std::cout << "Cannot change triangles palette" << std::endl;
			return;
		}

        delete[] palettes[triangles][layer];
        palettes[triangles][layer] = new float[number_colors][3];

		for (int i = 0; i < number_colors; i++) {
            palettes[triangles][layer][i][0] = new_palette[i * 3 + 0] / 255;
            palettes[triangles][layer][i][1] = new_palette[i * 3 + 1] / 255;
            palettes[triangles][layer][i][2] = new_palette[i * 3 + 2] / 255;
		}

        palette_sizes[triangles][layer] = number_colors;
	}
	else if (obj == cubes)
	{
		if (get_layer_data(layer_name, cubes, layer, max_elements)) {
			std::cout << "Cannot change cubes palette" << std::endl;
			return;
		}

        delete[] palettes[cubes][layer];
        palettes[cubes][layer] = new float[number_colors][3];

		for (int i = 0; i < number_colors; i++) {
            palettes[cubes][layer][i][0] = new_palette[i * 3 + 0] / 255;
            palettes[cubes][layer][i][1] = new_palette[i * 3 + 1] / 255;
            palettes[cubes][layer][i][2] = new_palette[i * 3 + 2] / 255;
		}

        palette_sizes[cubes][layer] = number_colors;
	}
}

void visualizerClass::send_palette_HSV(std::string layer_name, object_type obj, float *new_palette, int number_colors) {

	unsigned int layer, max_elements;

	if (obj == points)
	{
		if (get_layer_data(layer_name, points, layer, max_elements)) {
			std::cout << "Cannot change points palette" << std::endl;
			return;
		}

        delete[] palettes[points][layer];

        palettes[points][layer] = new float[number_colors][3];

		for (int i = 0; i < number_colors; i++) { 

            HSVtoRGB(new_palette[i * 3 + 0], new_palette[i * 3 + 1], new_palette[i * 3 + 2], &palettes[points][layer][i][0]);

            palettes[points][layer][i][0] /= 255;
            palettes[points][layer][i][1] /= 255;
            palettes[points][layer][i][2] /= 255;
		}

        palette_sizes[points][layer] = number_colors;
	}
	else if (obj == lines)
	{
		if (get_layer_data(layer_name, lines, layer, max_elements)) {
			std::cout << "Cannot change lines palette" << std::endl;
			return;
		}

        delete[] palettes[lines][layer];
        palettes[lines][layer] = new float[number_colors][3];

		for (int i = 0; i < number_colors; i++) {

            HSVtoRGB(new_palette[i * 3 + 0], new_palette[i * 3 + 1], new_palette[i * 3 + 2], &palettes[lines][layer][i][0]);

            palettes[lines][layer][i][0] /= 360;
		}

        palette_sizes[lines][layer] = number_colors;
	}
	else if (obj == triangles)
	{
		if (get_layer_data(layer_name, triangles, layer, max_elements)) {
			std::cout << "Cannot change triangles palette" << std::endl;
			return;
		}

        delete[] palettes[triangles][layer];
        palettes[triangles][layer] = new float[number_colors][3];

		for (int i = 0; i < number_colors; i++) {

            HSVtoRGB(new_palette[i * 3 + 0], new_palette[i * 3 + 1], new_palette[i * 3 + 2], &palettes[triangles][layer][i][0]);

            palettes[triangles][layer][i][0] /= 360;
		}

        palette_sizes[triangles][layer] = number_colors;
	}
	else if (obj == cubes)
	{
		if (get_layer_data(layer_name, cubes, layer, max_elements)) {
			std::cout << "Cannot change cubes palette" << std::endl;
			return;
		}

        delete[] palettes[cubes][layer];
        palettes[cubes][layer] = new float[number_colors][3];

		for (int i = 0; i < number_colors; i++) {

            HSVtoRGB(new_palette[i * 3 + 0], new_palette[i * 3 + 1], new_palette[i * 3 + 2], &palettes[cubes][layer][i][0]);

            palettes[cubes][layer][i][0] /= 360;
		}

        palette_sizes[cubes][layer] = number_colors;
	}
}

void visualizerClass::convert_HSVtoRGB(float *colors, int num_colors) {

	float *coloroutput = new float[3];

	for (int i = 0; i < num_colors; i++) 
	{
		HSVtoRGB(colors[i * 3 + 0], colors[i * 3 + 0], colors[i * 3 + 0], coloroutput);

		colors[i * 3 + 0] = coloroutput[0];
		colors[i * 3 + 1] = coloroutput[1];
		colors[i * 3 + 2] = coloroutput[2];
	}

	delete[] coloroutput;
}

void visualizerClass::convert_RGB255toRGB(float *colors, int num_colors) {

	for (int i = 0; i < num_colors; i++)
	{
		colors[i * 3 + 0] /= 255;
		colors[i * 3 + 1] /= 255;
		colors[i * 3 + 2] /= 255;
	}
}

void visualizerClass::fill_data_window(const std::string *data_strings, int num_strings) {

    std::lock_guard<std::mutex> lock(mut_fill_data);

	if (data_window_size != num_strings)
	{
        delete [] data_window;
		data_window_size = num_strings;
		data_window = new std::string[num_strings];
	}

    for (int i = 0; i < num_strings; i++)
        data_window[i] = data_strings[i];
}

void visualizerClass::pol_3th_degree(float *results_array, float xmin, float xmax, float sample, float a, float b, float c, float d) {

    float step = (xmax - xmin) / sample;
    int counter = 0;
    for (int i = 0; i <= sample; i++)
    {
        results_array[i * 3 + 0] = xmin;
        results_array[i * 3 + 1] = a + b * xmin + c * xmin*xmin + d * xmin*xmin*xmin;
        results_array[i * 3 + 2] = 0;

        xmin += step;
    }
}

void visualizerClass::icosahedron(float side_length, float(*points)[3]) {

	float S = side_length;
	const float pi = 3.14159265359;
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

// Private members -------------------------------------------------------

int visualizerClass::run_thread() {

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, test, NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true;										// Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }



    //IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();		//ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);



    cam.adjustments(window);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    glClearColor( 0.5f, 0.5f, 0.5f, 0.0f );		// Background color

    glEnable(GL_DEPTH_TEST);					// Enable depth test
    glDepthFunc(GL_LESS);						// Accept fragment if it closer to the camera than the former one

    glEnable(GL_CULL_FACE);						// Cull triangles which normal is not towards the camera.    glDisable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_PROGRAM_POINT_SIZE);			// Enable GL_POINTS
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);		// Enable gl_PointSize in the vertex shader
    glEnable(GL_POINT_SMOOTH);					// For circular points (GPU implementation dependent)
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// For circular points (GPU implementation dependent)
    glPointSize(5.0);							// GL_POINT_SIZE_MAX is GPU implementation dependent

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(2.0);
    GLfloat lineWidthRange[2];                  // GPU implementation dependent
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);



    // Vertex array object (VAO)
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders				    system("pwd"): /home/hank/dev/OGL/Shaper/_BUILD
    //GLuint programID = LoadShaders(	"//home//hank//src//TransformVertexShader.vertexshader", "//home//hank//src//ColorFragmentShader.fragmentshader");
    GLuint programID = LoadShaders();

    // Get a handle for our "MVP" uniform and the camera position coordinates
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint Cam_position = glGetUniformLocation(programID, "Cam_pos");
    GLuint Pnt_size_ID = glGetUniformLocation(programID, "Pnt_siz");

    // GL_STATIC_DRAW example -------------------------------------------
    /*
        // Vertex buffer objects (VBO)
        static const GLfloat g_vertex_buffer_data[] = {
             0.0f,  0.0f,  0.0f,
             1.0f,  0.0f,  0.0f,
             2.0f,  0.0f,  0.0f,
        };
        static const GLfloat g_color_buffer_data[] = {
             1.0f,  0.0f,  0.0f,
             0.0f,  1.0f,  0.0f,
             0.0f,  0.0f,  1.0f,
        };
        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
        GLuint colorbuffer;
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(g_color_buffer_data), g_color_buffer_data, GL_STATIC_DRAW);
    */
    // ------------------------------------------------------------------

    GLuint *vertexbuffersIDs = new GLuint[layer_data.point_layers];
    glGenBuffers(layer_data.point_layers, vertexbuffersIDs);
	GLuint *colorbuffersIDs = new GLuint[layer_data.point_layers];
    glGenBuffers(layer_data.point_layers, colorbuffersIDs);

	GLuint *linebuffersIDs = new GLuint[layer_data.line_layers];
	glGenBuffers(layer_data.line_layers, linebuffersIDs);
	GLuint *linecolorsIDs = new GLuint[layer_data.line_layers];
	glGenBuffers(layer_data.line_layers, linecolorsIDs);

	GLuint *trianglebuffersIDs = new GLuint[layer_data.triangle_layers];
	glGenBuffers(layer_data.triangle_layers, trianglebuffersIDs);
	GLuint *trianglecolorsIDs = new GLuint[layer_data.triangle_layers];
	glGenBuffers(layer_data.triangle_layers, trianglecolorsIDs);

	GLuint *cubebuffersIDs = new GLuint[layer_data.cube_layers];
	glGenBuffers(layer_data.cube_layers, cubebuffersIDs);
	GLuint *cubecolorsIDs = new GLuint[layer_data.cube_layers];
	glGenBuffers(layer_data.cube_layers, cubecolorsIDs);

    // Main loop --------------------------------------------------------
    do {
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(backg_color[0], backg_color[1], backg_color[2], 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the screen and the depth test

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input
        {
            std::lock_guard<std::mutex> lock_controls(cam_mut);                 // computeMatricesFromInputs() passes 2 functions to GLFW as callbacks (mouseButtonCallback, scrollCallback). Both need an object of type "control" to make changes on it. Since it's not possible to pass that object because we can't modify its argument lists to do so (GLFW functions put the arguments), I decided to use a global control* (so the callback functions use this object) and a global mutex (to control accesses from differente visualizerClass objects to this pointer). Hence, multiple visualizer windows are possible.
            camera = &cam;
            if (!io.WantCaptureMouse) cam.computeMatricesFromInputs(window);    // io.WantCaptureMouse and io.WantCaptureKeyboard flags are true if dear imgui wants to use our inputs (i.e. cursor is hovering a window).
        }
        ProjectionMatrix = cam.getProjectionMatrix();
        ViewMatrix = cam.getViewMatrix();
        ModelMatrix = glm::mat4(1.0);                     // Identity matrix
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Send the position of the camera. Useful for adjusting the size of each point
		glUniform3fv(Cam_position, 1, &cam.position[0]);

        // Send the size of the points
        glUniform1fv(Pnt_size_ID, 1, &point_siz);



        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();



        load_points(vertexbuffersIDs, colorbuffersIDs);
        load_lines(linebuffersIDs, linecolorsIDs);
        load_triangles(trianglebuffersIDs, trianglecolorsIDs);
        load_cubes(cubebuffersIDs, cubecolorsIDs);



        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);



        // ImGui rendering
        create_windows();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());



        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);



    // ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup VBO and shader
    glDeleteBuffers(layer_data.point_layers, vertexbuffersIDs);
    glDeleteBuffers(layer_data.point_layers, colorbuffersIDs);
	glDeleteBuffers(layer_data.line_layers, linebuffersIDs);
	glDeleteBuffers(layer_data.line_layers, linecolorsIDs);
	glDeleteBuffers(layer_data.triangle_layers, trianglebuffersIDs);
	glDeleteBuffers(layer_data.triangle_layers, trianglecolorsIDs);
    glDeleteBuffers(layer_data.cube_layers, cubebuffersIDs);
    glDeleteBuffers(layer_data.cube_layers, cubecolorsIDs);

    glDeleteProgram(programID);
    glDeleteVertexArrays(1, &VertexArrayID);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    return 0;
}

void visualizerClass::create_windows() {

    //ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiCond_FirstUseEver);
    //ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    //ImGui::Begin("Toolbox");
    //ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Options")) {
                ImGui::MenuItem("Checkboxes", NULL, &show_checkboxes);
                ImGui::MenuItem("Data", NULL, &show_data);
                ImGui::MenuItem("Configuration", NULL, &show_options);
            ImGui::EndMenu();
        }
/*
        if (ImGui::BeginMenu("Camera")) {
            ImGui::MenuItem("Sphere", NULL, &cam_sphere);
            if (cam_sphere) {
                cam_FPS = 0;
                //camera = controls(SPHERE);
                //cam.camera_mode = SPHERE;
            }
            ImGui::MenuItem("FPS", NULL, &cam_FPS);
            if (cam_FPS) {
                cam_sphere = 0;
                //camera = controls(FPS);
                //cam.camera_mode = FPS;
            }
            ImGui::EndMenu();
        }
*/
        ImGui::EndMainMenuBar();
    }

    if(show_checkboxes)
    {
        ImGui::Begin("Checkboxes", &show_checkboxes);

			for(int i = 0; i < layer_data.point_layers; i++)
                ImGui::Checkbox((layer_data.point_layers_names[i]).c_str(), &checkboxes_values[points][i]);

			ImGui::Separator;

			for (int i = 0; i < layer_data.line_layers; i++)
				ImGui::Checkbox((layer_data.line_layers_names[i]).c_str(), &checkboxes_values[lines][i]);

			ImGui::Separator;

			for (int i = 0; i < layer_data.triangle_layers; i++)
				ImGui::Checkbox((layer_data.triangle_layers_names[i]).c_str(), &checkboxes_values[triangles][i]);

			ImGui::Separator;

			for (int i = 0; i < layer_data.cube_layers; i++)
				ImGui::Checkbox((layer_data.cube_layers_names[i]).c_str(), &checkboxes_values[cubes][i]);

        ImGui::End();
    }

    if (show_data)
    {
        std::lock_guard<std::mutex> lock(mut_fill_data);

        ImGui::Begin("Data window", &show_data);
            for (int i = 0; i < data_window_size; i++) {
                if(data_window[i] != "")
                    ImGui::Text(data_window[i].c_str());
            }
        ImGui::End();
    }

    // Background color, transparencies, show axis,
    if (show_options)
    {
        ImGui::Begin("Configuration", &show_options);

			ImGui::ColorEdit3("Background", backg_color);

            ImGui::SliderFloat("Point size", &point_siz, 1.0f, 500.0f);
			ImGui::Separator;
			for(int i = 0; i < layer_data.point_layers; i++)
                ImGui::SliderFloat(layer_data.point_layers_names[i].c_str(), &alpha_channels[points][i], 0.0f, 1.0f);
			ImGui::Separator;
			for (int i = 0; i < layer_data.line_layers; i++)
                ImGui::SliderFloat(layer_data.line_layers_names[i].c_str(), &alpha_channels[lines][i], 0.0f, 1.0f);
			ImGui::Separator;
			for (int i = 0; i < layer_data.triangle_layers; i++)
                ImGui::SliderFloat(layer_data.triangle_layers_names[i].c_str(), &alpha_channels[triangles][i], 0.0f, 1.0f);
			ImGui::Separator;
			for (int i = 0; i < layer_data.cube_layers; i++)
                ImGui::SliderFloat(layer_data.cube_layers_names[i].c_str(), &alpha_channels[cubes][i], 0.0f, 1.0f);
			ImGui::End();

			for (int i = 0; i < layer_data.point_layers; i++)		change_alpha_channel(points, layer_data.point_layers_names[i]);
			for (int i = 0; i < layer_data.line_layers; i++)		change_alpha_channel(lines, layer_data.line_layers_names[i]);
			for (int i = 0; i < layer_data.triangle_layers; i++)	change_alpha_channel(triangles, layer_data.triangle_layers_names[i]);
			for (int i = 0; i < layer_data.cube_layers; i++)		change_alpha_channel(cubes, layer_data.cube_layers_names[i]);
    }
}

void visualizerClass::create_demo_windows() {
/*
    static bool show_demo_window = true;
    static bool show_another_window = false;
    static ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    {
        static float slide = 0.0f;
        static int counter = 0;
        ImGui::Begin("Hello, world!");
            ImGui::Text("This is some useful text.");
            ImGui::Checkbox("Demo Window", &show_demo_window);
            ImGui::Checkbox("Another Window", &show_another_window);
            ImGui::SliderFloat("float", &slide, 0.0f, 1.0f);
            ImGui::ColorEdit3("clear color", (float*)&clear_color);
            if (ImGui::Button("Button")) counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me")) show_another_window = false;
        ImGui::End();
    }
*/
}

void visualizerClass::change_alpha_channel(object_type object, std::string layer_name) {

	unsigned int layer, max_elements;
	if ( get_layer_data(layer_name, object, layer, max_elements) ) return;

    if (object == points)
    {
        for (int i = 0; i < max_elements; i++)
            point_color_buffers[layer][i][3] = alpha_channels[points][layer];
    }
    if (object == lines)
    {
        for (int i = 0; i < max_elements; i++)
        {
            line_color_buffers[layer][i][0][3] = alpha_channels[lines][layer];
            line_color_buffers[layer][i][1][3] = alpha_channels[lines][layer];
        }
    }
	if (object == triangles)
	{
		for (int i = 0; i < max_elements; i++)
		{
            triangle_color_buffers[layer][i][0][3] = alpha_channels[triangles][layer];
            triangle_color_buffers[layer][i][1][3] = alpha_channels[triangles][layer];
            triangle_color_buffers[layer][i][2][3] = alpha_channels[triangles][layer];
		}
	}
    if (object == cubes)
    {
		for (int i = 0; i < max_elements; i++)
			for (int j = 0; j < 12 * 3; j++)
                cube_color_buffers[layer][i][j][3] = alpha_channels[cubes][layer];
    }
}

bool visualizerClass::get_layer_data(std::string layer_name, object_type obj_type, unsigned int &layer, unsigned int &max_elements) {

	unsigned int num_layers;
	std::string *names;
	unsigned int *max_primitives;

	// For the given object type, get the number of layers, layer names and maximum number of elements per layer.
	if (obj_type == points)
	{
		num_layers = layer_data.point_layers;
		names = &layer_data.point_layers_names[0];
		max_primitives = &layer_data.max_points[0];
	}
	else if (obj_type == lines)
	{
		num_layers = layer_data.line_layers;
		names = &layer_data.line_layers_names[0];
		max_primitives = &layer_data.max_lines[0];
	}
	else if (obj_type == triangles)
    {
        num_layers = layer_data.triangle_layers;
        names = &layer_data.triangle_layers_names[0];
        max_primitives = &layer_data.max_triangles[0];
	}
	else if (obj_type == cubes)
	{
		num_layers = layer_data.cube_layers;
		names = &layer_data.cube_layers_names[0];
		max_primitives = &layer_data.max_cubes[0];
	}

	// For the given layer name, get the number of the layer, and its maximum number of elements.
	for (unsigned int i = 0; i <= num_layers; i++)
	{
		if (i == num_layers)
		{
			std::cout << "The layer \"" << layer_name << "\" doesn't exist" << std::endl;
			return 1;
		}

		if (layer_name == names[i])
		{
			layer = i;
			max_elements = max_primitives[i];
			return 0;
		}
	}
}

void visualizerClass::rotation_H(float &x, float &y, float X, float Y, float rot) {

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

void visualizerClass::HSVtoRGB(int H, double S, double V, float coloroutput[3]) {

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

	coloroutput[0] = (Rs + m) * 255;
	coloroutput[1] = (Gs + m) * 255;
	coloroutput[2] = (Bs + m) * 255;
}

void visualizerClass::load_points(GLuint *vertexbuffIDs, GLuint *colorbuffIDs){

	for(int i = 0; i < layer_data.point_layers; i++)
	{
		{
            std::lock_guard<std::mutex> lock_points(mut[points][i]);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, objects_to_print[points][i] * 3 * sizeof(float), &point_buffers[i][0][0], GL_DYNAMIC_DRAW);				// GL_STATIC_DRAW

			glBindBuffer(GL_ARRAY_BUFFER, colorbuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, objects_to_print[points][i] * 4 * sizeof(float), &point_color_buffers[i][0][0], GL_DYNAMIC_DRAW);
		}

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffIDs[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, colorbuffIDs[i]);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

		//glDrawArrays(GL_TRIANGLES, 0, 12 * 3); // 12*3 indices starting at 0 -> 12 triangles
		if (checkboxes_values[points][i])
            glDrawArrays(GL_POINTS, 0, objects_to_print[points][i]);
		else
			glDrawArrays(GL_POINTS, 0, 0);
	}
}

void visualizerClass::load_lines(GLuint *linebuffIDs, GLuint *linecolorbuffIDs){

	for (int i = 0; i < layer_data.line_layers; i++)
	{
		{
            std::lock_guard<std::mutex> lock_points(mut[lines][i]);

			glBindBuffer(GL_ARRAY_BUFFER, linebuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, objects_to_print[lines][i] * 2 * 3 * sizeof(float), &line_buffers[i][0][0], GL_DYNAMIC_DRAW);

			glBindBuffer(GL_ARRAY_BUFFER, linecolorbuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, objects_to_print[lines][i] * 2 * 4 * sizeof(float), &line_color_buffers[i][0][0], GL_DYNAMIC_DRAW);
		}

        // 1st attribute buffer : lines
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, linebuffIDs[i]);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : colors
		glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, linecolorbuffIDs[i]);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        if (checkboxes_values[lines][i])
            glDrawArrays(GL_LINES, 0, objects_to_print[lines][i] * 2);
		else
			glDrawArrays(GL_LINES, 0, 0);
	}
}

void visualizerClass::load_triangles(GLuint *trianglebuffIDs, GLuint *trianglecolorbuffIDs) {

    for (int i = 0; i < layer_data.triangle_layers; i++)
    {
        {
            std::lock_guard<std::mutex> lock_points(mut[triangles][i]);

            glBindBuffer(GL_ARRAY_BUFFER, trianglebuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, objects_to_print[triangles][i] * 3 * 3 * sizeof(float), &triangle_buffers[i][0][0], GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, trianglecolorbuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, objects_to_print[triangles][i] * 3 * 4 * sizeof(float), &triangle_color_buffers[i][0][0], GL_DYNAMIC_DRAW);
        }

        // 1st attribute buffer : lines
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, trianglebuffIDs[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, trianglecolorbuffIDs[i]);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        if (checkboxes_values[triangles][i])
            glDrawArrays(GL_TRIANGLES, 0, objects_to_print[triangles][i] * 3);
        else
            glDrawArrays(GL_TRIANGLES, 0, 0);
    }
}

void visualizerClass::load_cubes(GLuint *cubebuffIDs, GLuint *cubecolorbuffIDs){

    for (int i = 0; i < layer_data.cube_layers; i++)
    {
        {
            std::lock_guard<std::mutex> lock_points(mut[cubes][i]);

            glBindBuffer(GL_ARRAY_BUFFER, cubebuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, objects_to_print[cubes][i] * 12 * 3 * 3 * sizeof(float), &cube_buffers[i][0][0], GL_DYNAMIC_DRAW);

            glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, objects_to_print[cubes][i] * 12 * 3 * 4 * sizeof(float), &cube_color_buffers[i][0][0], GL_DYNAMIC_DRAW);
        }

        // 1st attribute buffer : cubes
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cubebuffIDs[i]);
        glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cubecolorbuffIDs[i]);
        glVertexAttribPointer( 1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0 );

        if(checkboxes_values[cubes][i])
            glDrawArrays(GL_TRIANGLES, 0, objects_to_print[cubes][i] * 12 * 3);
        else
            glDrawArrays(GL_TRIANGLES, 0, 0);
    }
}
