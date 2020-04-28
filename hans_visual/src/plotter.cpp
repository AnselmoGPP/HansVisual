#include "plotter.hpp"

// Main public members ---------------------------------------------------

plotter::plotter(std::vector<layer> *layers_set, std::mutex *layers_set_mutex) : layersSet(layers_set), layersSet_mut(layers_set_mutex), kc(keys_controller::get_instance())
{
    backg_color[0] = BACKG_R;  backg_color[1] = BACKG_G;  backg_color[2] = BACKG_B;
    point_siz = PNT_SIZE;

    // OGL buffers filling for being able to call delete_buffer() at any moment
    VertexArraysID = nullptr;
    vertexbuffersIDs = nullptr;
    colorbuffersIDs = nullptr;
}

plotter::plotter(const plotter &obj) : layersSet(obj.layersSet)
{
    layersSet = obj.layersSet;
    layersSet_mut = obj.layersSet_mut;

    //cam = obj.cam;
    gui = obj.gui;
    win.window_open = false;

    for(int i = 0; i < 3; ++i) backg_color[i] = obj.backg_color[i];
    point_siz = obj.point_siz;
}

plotter& plotter::operator=(const plotter &obj)
{
    layersSet = obj.layersSet;
    layersSet_mut = obj.layersSet_mut;

    //cam = obj.cam;
    gui = obj.gui;
    //window = obj.window;

    display_w = obj.display_w;  display_h = obj.display_h;

    for(int i = 0; i < 3; ++i) backg_color[i] = obj.backg_color[i];
    point_siz = obj.point_siz;

    return *this;
}

plotter::~plotter()
{
    // delete window;
}

int  plotter::open_window()
{
    win.open_GLFW_window();
    cam.associate_window(win.window);

    std::thread running(&plotter::main_loop_thread, this);
    running.detach();

    return 0;
}

void plotter::resize_buffer_set(size_t new_size)
{

    if(vertexbuffersIDs != nullptr)         // Executed only if VBOs have already been created
    {
        size_t prev_size = layersSet->size();

        glDeleteBuffers(prev_size, vertexbuffersIDs);
        glDeleteBuffers(prev_size, colorbuffersIDs);

        delete[] vertexbuffersIDs;
        delete[] colorbuffersIDs;

        vertexbuffersIDs = new GLuint[new_size];
        colorbuffersIDs  = new GLuint[new_size];

        glGenBuffers(new_size, vertexbuffersIDs);
        glGenBuffers(new_size, colorbuffersIDs);

/*
        size_t prev_size = layersSet->size();

        if(prev_size < new_size)
        {
            GLuint *temp_v = new GLuint[prev_size];
            GLuint *temp_c = new GLuint[prev_size];
            for(int i = 0; i < prev_size; ++i)
            {
                temp_v[i] = vertexbuffersIDs[i];
                temp_c[i] = colorbuffersIDs[i];
            }

            delete[] vertexbuffersIDs;
            delete[] colorbuffersIDs;
            vertexbuffersIDs = new GLuint[new_size];
            colorbuffersIDs  = new GLuint[new_size];

            size_t j = 0;
            for(j; j < prev_size; ++j)
            {
                vertexbuffersIDs[j] = temp_v[j];
                colorbuffersIDs[j] = temp_c[j];
            }

            for(j; j < new_size; ++j)
            {
                glGenBuffers(1, vertexbuffersIDs + j);
                glGenBuffers(1, colorbuffersIDs + j);
            }

            delete[] temp_v;
            delete[] temp_c;
        }
        else if(prev_size > new_size)
        {

        }
        else return;
*/
    }
}

bool plotter::window_is_open() { return win.window_open; }

void plotter::close_window() { win.window_open = false; }

void plotter::fill_data_window(const std::string *data_strings, int num_strings)
{
    gui.fill_data_window(data_strings, num_strings);
}


// Private members -------------------------------------------------------

int  plotter::main_loop_thread()
{
    win.MakeContextCurrent();

    // Create the OpenGL context for the GLFW window
    glewExperimental = true;        // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        win.Terminate();
        return -1;
    }

    cam.sticky_keys(true);
    cam.set_mouse_position_visibility();

    gui.init_gui();
    ImGui_ImplGlfw_InitForOpenGL(win.window, true);
    ImGui_ImplOpenGL3_Init(gui.glsl_version);

    set_gl_options();

    create_VAOs(1);
    glBindVertexArray(VertexArraysID[0]);

    // Create and compile our GLSL program from the shaders				    system("pwd"): /home/hank/dev/OGL/Shaper/_BUILD
    //GLuint programID = LoadShaders(	"//home//hank//src//TransformVertexShader.vertexshader", "//home//hank//src//ColorFragmentShader.fragmentshader");
    std::map<char*, unsigned int> program;
    program["3D"] = LoadShaders(VertexShaderCode, FragmentShaderCode);
    program["2D"] = LoadShaders(VertexShaderCode2D, FragmentShaderCode);

    create_uniforms(program["3D"]);

    //gl_static_draw_example();

    create_VBOs(layersSet->size(), layersSet_mut);

    /*
    sel.selectionSquareID = new GLuint;
    sel.selectionSquareColorID = new GLuint;
    glGenBuffers(1, sel.selectionSquareID);
    glGenBuffers(1, sel.selectionSquareColorID);
    */

    // ------------------------------------

    do
    {
        //std::cout << "\rFrame " << frame_count;
        //std::cout << "---------- " << frame_count << std::endl;

        time_1 = std::chrono::high_resolution_clock::now();
        //if(!win.window_open) break;

        set_viewport_and_background();

        gui.new_frame();

        glUseProgram(program["3D"]);        // Use our shader

        // Compute the MVP matrix from keyboard and mouse input
        if (!gui.WantCaptureMouse())
            cam.computeMatricesFromInputs((float)display_w / display_h);

        ProjectionMatrix = cam.getProjectionMatrix();
        ViewMatrix = cam.getViewMatrix();
        ModelMatrix = glm::mat4(1.0);                     // Identity matrix
        MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

        // Individual layers modifiers
        sqr_sel.send_selection_square(*layersSet, display_h, display_w);
        pnt_sel.send_selected_points(*layersSet, ModelMatrix, ViewMatrix, ProjectionMatrix);

        //points_selection pnt_sel;
        //sel.send_selection_square(display_h, display_w);
        //sel.send_selected_points(&ProjectionMatrix, &ViewMatrix, &ModelMatrix, display_h, display_w);    // Points selection search ( http://www.3dkingdoms.com/selection.html ). Looks for the selected points and print its data, if an array of strings was provided

        send_uniforms();

        std::lock_guard<std::mutex> lock(*layersSet_mut);

        load_buffers(program);

        /*
        load_buffers(vertexbuffersIDs, colorbuffersIDs);

        glUseProgram(programID_3D);
        load_selectionSquare(sel.selectionSquareID, sel.selectionSquareColorID);
        */

        gui.create_gui_1(layersSet, backg_color, &point_siz);
        gui.render_gui();

        keys_controller::get_instance()->update_key_states(win.window);
        win.SwapBuffers_PollEvents();

        ++frame_count;

        for(layer &lay : *layersSet) lay.state = open;

        time_2 = std::chrono::high_resolution_clock::now();
        fps_control(DESIRED_FPS);
    }
    // Check if the ESC key was pressed or the window was closed
    while (win.scape_conditions());

    // ------------------------------------

    gui.cleanup();

    // Cleanup VBO and shader
    glDeleteBuffers(layersSet->size(), vertexbuffersIDs);       delete[] vertexbuffersIDs;    vertexbuffersIDs = nullptr;
    glDeleteBuffers(layersSet->size(), colorbuffersIDs);        delete[] colorbuffersIDs;     colorbuffersIDs  = nullptr;

    glDeleteProgram(program["3D"]);
    glDeleteProgram(program["2D"]);

    glDeleteVertexArrays(1, VertexArraysID);

    win.Terminate();

    return 0;
}

void plotter::set_gl_options()
{
    glClearColor(backg_color[0], backg_color[1], backg_color[2], 0.0f);     // Background color

    glEnable(GL_DEPTH_TEST);					// Enable depth test
    glDepthFunc(GL_LESS);						// Accept fragment if it's closer to the camera than the former one

    // Cull triangles which normal is not towards the camera
    if(CULL_FACES)  glEnable(GL_CULL_FACE);
    else            glDisable(GL_CULL_FACE);

    // Enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Point parameters
    glEnable(GL_PROGRAM_POINT_SIZE);			// Enable GL_POINTS
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);		// Enable gl_PointSize in the vertex shader
    glEnable(GL_POINT_SMOOTH);					// For circular points (GPU implementation dependent)
    glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);	// For circular points (GPU implementation dependent)
    glPointSize(5.0);							// GL_POINT_SIZE_MAX is GPU implementation dependent

    // Lines parameters
    if(POLYGON_MODE) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // Show only the borders of the triangles
    glLineWidth(2.0);
    GLfloat lineWidthRange[2];                  // GPU implementation dependent
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);
    //std::cout << "Line width range: " << lineWidthRange[1] << ", " << lineWidthRange[2] << std::endl;
}

void plotter::load_buffers(std::map<char*, unsigned int> program)
{
    for(int i = 0; i < layersSet->size(); ++i)
    {
        if(layersSet->operator[](i).dimensions == 3)
            load_buffer_3D(&layersSet->operator[](i), vertexbuffersIDs[i], colorbuffersIDs[i], program["3D"]);
        else
        if(layersSet->operator[](i).dimensions == 2)
            load_buffer_2D(&layersSet->operator[](i), vertexbuffersIDs[i], colorbuffersIDs[i], program["2D"]);
    }
}

void plotter::load_buffer_3D(layer *lay, GLuint vertexbuffID, GLuint colorbuffID, unsigned int program)
{
    if (!lay->checkbox_value || lay->objs_to_print == 0) return;

    glUseProgram(program);

    unsigned long vertex_per_obj;
    GLenum GL_OBJS;

    if     (lay->layer_type == points)
    {
        vertex_per_obj = 1;
        GL_OBJS = GL_POINTS;
    }
    else if(lay->layer_type == lines)
    {
        vertex_per_obj = 2;
        GL_OBJS = GL_LINES;
    }
    else if(lay->layer_type == triangles)
    {
        vertex_per_obj = 3;
        GL_OBJS = GL_TRIANGLES;
    }
    else if(lay->layer_type == cubes)
    {
        vertex_per_obj = 3*12;
        GL_OBJS = GL_TRIANGLES;
    }

    {
        std::lock_guard<std::mutex> lock(*lay->mut);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffID);
        glBufferData(GL_ARRAY_BUFFER, lay->objs_to_print * vertex_per_obj * 3 * sizeof(float), lay->get_vertex_ptr(), GL_DYNAMIC_DRAW);				// GL_STATIC_DRAW

        glBindBuffer(GL_ARRAY_BUFFER, colorbuffID);
        glBufferData(GL_ARRAY_BUFFER, lay->objs_to_print * vertex_per_obj * 4 * sizeof(float), lay->get_colors_ptr(), GL_DYNAMIC_DRAW);
    }

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffID);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_OBJS, 0, lay->objs_to_print * vertex_per_obj);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void plotter::load_buffer_2D(layer *lay, GLuint vertexbuffID, GLuint colorbuffID, unsigned int program)
{
    if (!lay->checkbox_value || lay->objs_to_print == 0) return;

    glUseProgram(program);

    unsigned long vertex_per_obj;
    GLenum GL_OBJS;

    if     (lay->layer_type == points)
    {
        vertex_per_obj = 1;
        GL_OBJS = GL_POINTS;
    }
    else if(lay->layer_type == lines)
    {
        vertex_per_obj = 2;
        GL_OBJS = GL_LINES;
    }
    else if(lay->layer_type == triangles)
    {
        vertex_per_obj = 3;
        GL_OBJS = GL_TRIANGLES;
    }

    {
    std::lock_guard<std::mutex> lock(*lay->mut);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffID);
    glBufferData(GL_ARRAY_BUFFER, lay->objs_to_print * vertex_per_obj * 3 * sizeof(float), lay->get_vertex_ptr(), GL_DYNAMIC_DRAW);				// GL_STATIC_DRAW

    glBindBuffer(GL_ARRAY_BUFFER, colorbuffID);
    glBufferData(GL_ARRAY_BUFFER, lay->objs_to_print * vertex_per_obj * 4 * sizeof(float), lay->get_colors_ptr(), GL_DYNAMIC_DRAW);
    }

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexbuffID);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorbuffID);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_OBJS, 0, lay->objs_to_print * vertex_per_obj);
/*
    std::cout << lay->layer_name << ", " << lay->layer_type << ", " << lay->objs_to_print << std::endl;
    for(int i = 0; i < lay->objs_to_print; i++)
    {
        std::cout << "     " << lay->lines_buffer[i][0][0] << ", " << lay->lines_buffer[i][0][1] << ", " << lay->lines_buffer[i][0][2] << ", " << lay->lines_buffer[i][1][0] << ", " << lay->lines_buffer[i][1][1] << ", " << lay->lines_buffer[i][1][2] << std::endl;
    }
*/
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void plotter::fps_control(unsigned int frequency)
{
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_2 - time_1).count();
    long desired_time = 1000000/frequency;
    if(duration < desired_time) std::this_thread::sleep_for(std::chrono::microseconds(desired_time - duration));

    duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - time_1).count();
    //std::cout << "FPS: " << 1000000/duration << '\r';
}

void plotter::gl_static_draw_example()
{
    // Vertex buffer objects (VBO)
    static const GLfloat g_vertex_buffer_data[] = {
            1.0f,  1.0f,  1.0f,
            2.0f,  2.0f,  2.0f,
            3.0f,  3.0f,  3.0f,
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
}

void plotter::create_VAOs(int amount)
{
    VertexArraysID = new GLuint[amount];
    glGenVertexArrays(amount, VertexArraysID);          // Vertex array object (VAO)
}

void plotter::create_uniforms(GLuint programID)
{
    // Get a handle for our uniforms: MVP, camera position and coordinates
    unif["MVP"] = glGetUniformLocation(programID, "MVP");
    unif["Cam_pos"] = glGetUniformLocation(programID, "Cam_pos");
    unif["Pnt_size"] = glGetUniformLocation(programID, "Pnt_siz");
}

void plotter::create_VBOs(int amount, std::mutex *mut)
{
    std::lock_guard<std::mutex> lock(*mut);

    vertexbuffersIDs = new GLuint[amount];    // The first buffer is for the selected points
    colorbuffersIDs = new GLuint[amount];

    glGenBuffers(amount, vertexbuffersIDs);
    glGenBuffers(amount, colorbuffersIDs);
}

void plotter::send_uniforms()
{
    // Send our transformation to the currently bound shader, in the "MVP" uniform
    glUniformMatrix4fv(unif["MVP"], 1, GL_FALSE, &MVP[0][0]);
    // Send the position of the camera. Useful for adjusting the size of each point
    glUniform3fv(unif["Cam_pos"], 1, &cam.position[0]);
    // Send the size of the points
    glUniform1fv(unif["Pnt_size"], 1, &point_siz);
}

void plotter::set_viewport_and_background()
{
    win.GetFramebufferSize(&display_w, &display_h);     // Get frame buffer size (window pixels)
    glViewport(0, 0, display_w, display_h);             // Set viewport size: Lower left corner, top right corner
    //glGetIntegerv(GL_VIEWPORT, viewport);             // Get viewport size: int viewport[4]

    glClearColor(backg_color[0], backg_color[1], backg_color[2], 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the screen and the depth test
}

