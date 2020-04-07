#include "plotter.hpp"

// Main public members ---------------------------------------------------

plotter::plotter(std::vector<layer> *layers_set, std::mutex *layers_set_mutex) : layersSet(layers_set), layersSet_mut(layers_set_mutex)
{
    // Create the first layer (Selections)
    layersSet->push_back(layer("Selections", points, 0));

    backg_color[0] = BACKG_R;  backg_color[1] = BACKG_G;  backg_color[2] = BACKG_B;
    point_siz = PNT_SIZE;

    // Points selection
    MinDistance = 0.01;
    selection_color[0] = SEL_R;  selection_color[1] = SEL_G;  selection_color[2] = SEL_B;  selection_color[3] = SEL_A;
    selection_square = layer("Sel. square", points, 0);

    // OGL buffers filling for being able to call delete_buffer() at any moment
    VertexArraysID = nullptr;
    vertexbuffersIDs = nullptr;
    colorbuffersIDs = nullptr;
}

plotter::plotter(const plotter &obj) : layersSet(obj.layersSet)
{
    layersSet = obj.layersSet;

    //cam = obj.cam;
    gui = obj.gui;
    win.window_open = false;

    for(int i = 0; i < 3; ++i) backg_color[i] = obj.backg_color[i];
    point_siz = obj.point_siz;
    for(int i = 0; i < 4; ++i) selection_color[i] = obj.selection_color[i];
    temp_selections = obj.temp_selections;

    // Points selection
    MinDistance = obj.MinDistance;
}

plotter& plotter::operator=(const plotter &obj)
{
    layersSet = obj.layersSet;

    //cam = obj.cam;
    gui = obj.gui;
    //window = obj.window;
    display_w = obj.display_w;  display_h = obj.display_h;

    for(int i = 0; i < 3; ++i) backg_color[i] = obj.backg_color[i];
    point_siz = obj.point_siz;

    // Points selection
    for(int i = 0; i < 16; ++i) { projmatrix[i] = obj.projmatrix[i];  mvmatrix[i] = obj.mvmatrix[i]; }
    for(int i = 0; i < 4; ++i) viewport[i] = obj.viewport[i];
    MinDistance = obj.MinDistance;
    for(int i = 0; i < 4; ++i) selection_color[i] = obj.selection_color[i];
    temp_selections = obj.temp_selections;

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
        glDeleteBuffers(layersSet->size(), vertexbuffersIDs);
        glDeleteBuffers(layersSet->size(), colorbuffersIDs);

        delete[] vertexbuffersIDs;
        delete[] colorbuffersIDs;

        vertexbuffersIDs = new GLuint[new_size];
        colorbuffersIDs  = new GLuint[new_size];

        glGenBuffers(new_size, vertexbuffersIDs);
        glGenBuffers(new_size, colorbuffersIDs);
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
    GLuint programID = LoadShaders(VertexShaderCode, FragmentShaderCode);
    GLuint programID_selection = LoadShaders(VertexShaderCode2D, FragmentShaderCode);

    create_uniforms(programID);

    //gl_static_draw_example();

    create_VBOs(layersSet->size(), layersSet_mut);

    selectionSquareID = new GLuint;
    selectionSquareColorID = new GLuint;
    glGenBuffers(1, selectionSquareID);
    glGenBuffers(1, selectionSquareColorID);

    // ------------------------------------
    do
    {
        time_1 = std::chrono::high_resolution_clock::now();
        //if(!win.window_open) break;

        clear_and_set_background();

        glUseProgram(programID);        // Use our shader

        // Compute the MVP matrix from keyboard and mouse input. Also check whether a selection was made.
        {
            std::lock_guard<std::mutex> lock_controls(cam_mut);              // computeMatricesFromInputs() passes 2 functions to GLFW as callbacks (mouseButtonCallback, scrollCallback). Both need an object of type "control" to make changes on it. Since it's not possible to pass that object because we can't modify its argument lists to do so (GLFW functions puts the arguments), I decided to use a global control* (so the callback functions use this object) and a global mutex (to control accesses from different visualizerClass objects to this pointer). Hence, multiple visualizer windows are possible.
            camHandler = &cam;
            if (!gui.WantCaptureMouse()) cam.computeMatricesFromInputs((float)display_w / display_h);
        
            ProjectionMatrix = cam.getProjectionMatrix();
            ViewMatrix = cam.getViewMatrix();
            ModelMatrix = glm::mat4(1.0);                     // Identity matrix
            MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            check_selections();
		}

        send_uniforms();

        gui.new_frame();

        std::lock_guard<std::mutex> lock(*layersSet_mut);
        load_buffers(vertexbuffersIDs, colorbuffersIDs);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glUseProgram(programID_selection);
        load_selectionSquare(selectionSquareID, selectionSquareColorID);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        gui.create_gui_1(layersSet, backg_color, &point_siz);
        gui.render_gui();

        // Swap buffers
        win.SwapBuffers_PollEvents();

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
    glDeleteBuffers(1, selectionSquareID);
    glDeleteBuffers(1, selectionSquareColorID);

    glDeleteProgram(programID);
    glDeleteProgram(programID_selection);

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
    glEnable(GL_CULL_FACE);
    glDisable(GL_CULL_FACE);

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
}

void plotter::load_buffers(GLuint *vertexbuffIDs, GLuint *colorbuffIDs)
{
    for(size_t i = 0; i < layersSet->size(); i++)
    {
        if (!layersSet->operator[](i).checkbox_value) continue;

        unsigned long vertex_per_obj;
        GLenum GL_OBJS;

        if     (layersSet->operator[](i).layer_type == points)
        {
            vertex_per_obj = 1;
            GL_OBJS = GL_POINTS;
        }
        else if(layersSet->operator[](i).layer_type == lines)
        {
            vertex_per_obj = 2;
            GL_OBJS = GL_LINES;
        }
        else if(layersSet->operator[](i).layer_type == triangles)
        {
            vertex_per_obj = 3;
            GL_OBJS = GL_TRIANGLES;
        }
        else if(layersSet->operator[](i).layer_type == cubes)
        {
            vertex_per_obj = 3*12;
            GL_OBJS = GL_TRIANGLES;
        }

        {
            std::lock_guard<std::mutex> lock(*layersSet->operator[](i).mut);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, layersSet->operator[](i).objs_to_print * vertex_per_obj * 3 * sizeof(float), layersSet->operator[](i).get_vertex_ptr(), GL_DYNAMIC_DRAW);				// GL_STATIC_DRAW

            glBindBuffer(GL_ARRAY_BUFFER, colorbuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, layersSet->operator[](i).objs_to_print * vertex_per_obj * 4 * sizeof(float), layersSet->operator[](i).get_colors_ptr(), GL_DYNAMIC_DRAW);
        }

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffIDs[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffIDs[i]);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_OBJS, 0, layersSet->operator[](i).objs_to_print * vertex_per_obj);
    }
}

void plotter::load_selectionSquare(GLuint *selectionSquareID, GLuint *selectionColorID)
{
    if(cam.is_R_pressed())
    {
        // Set positions
        float square[4][2][3];
        double x, y, x0, y0;
        x =     (cam.sel_xpos - display_w/2)/(display_w/2);
        x0 =    (cam.sel_xpos0 - display_w/2)/(display_w/2);
        y =    -(cam.sel_ypos - display_h/2)/(display_h/2);
        y0 =   -(cam.sel_ypos0 - display_h/2)/(display_h/2);
/*
        square[0][0] = x0;
        square[0][1] = y0;
        square[0][2] = 0.;
        square[1][0] = x;
        square[1][1] = y0;
        square[1][2] = 0.;
        square[2][0] = x;
        square[2][1] = y;
        square[2][2] = 0.;
        square[3][0] = x0;
        square[3][1] = y;
        square[3][2] = 0.;
        square[4][0] = x0;
        square[4][1] = y0;
        square[4][2] = 0.;
*/
        // Set colors
        float square_colors[5][4];
        for(int i = 0; i < 5; i++)
        {
            square_colors[i][0] = selection_color[0];
            square_colors[i][1] = selection_color[1];
            square_colors[i][2] = selection_color[2];
            square_colors[i][3] = selection_square.alpha_channel;
        }

        selection_square.send_lines(5, square, selection_color[0], selection_color[1], selection_color[2]);


        glBindBuffer(GL_ARRAY_BUFFER, *selectionSquareID);
        glBufferData(GL_ARRAY_BUFFER, selection_square.objs_to_print * 3 * sizeof(float), selection_square.get_vertex_ptr(), GL_DYNAMIC_DRAW);				// GL_STATIC_DRAW

        glBindBuffer(GL_ARRAY_BUFFER, *selectionColorID);
        glBufferData(GL_ARRAY_BUFFER, selection_square.objs_to_print * 4 * sizeof(float), selection_square.get_colors_ptr(), GL_DYNAMIC_DRAW);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, *selectionSquareID);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, *selectionColorID);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        glDrawArrays(GL_LINE_STRIP, 0, selection_square.objs_to_print);
    }
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

void plotter::clear_and_set_background()
{
    win.GetFramebufferSize(&display_w, &display_h);
    glViewport(0, 0, display_w, display_h);                 // Arguments: Lower left
    glClearColor(backg_color[0], backg_color[1], backg_color[2], 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the screen and the depth test
}
