#include "plotter.hpp"

// Main public members ---------------------------------------------------

plotter::plotter(std::vector<layer> *layers_set, std::mutex *layers_set_mutex) : layersSet(layers_set), layersSet_mut(layers_set_mutex)
{
    // Create the first layer (Selections)
    layersSet->push_back(layer("Selections", points, 0));

    //window = nullptr;
    //window_open = false;

    // GUI state
    show_checkboxes = 1;
    show_data = 0;
    show_options = 0;
	data_window_size = 1;
    data_window = new std::string[data_window_size];
    backg_color[0] = BACKG_R;  backg_color[1] = BACKG_G;  backg_color[2] = BACKG_B;
    point_siz = PNT_SIZE;

    // Points selection
    MinDistance = 0.01;
    selection_color[0] = SEL_R;  selection_color[1] = SEL_G;  selection_color[2] = SEL_B;  selection_color[3] = SEL_A;
    selection_square = layer("Sel. square", points, 0);

    // OGL buffers filling for being able to call delete_buffer() at any moment
    vertexbuffersIDs = nullptr;
    colorbuffersIDs = nullptr;
}

plotter::plotter(const plotter &obj) : layersSet(obj.layersSet)
{
    layersSet = obj.layersSet;

    //cam = obj.cam;

    win.window_open = false;

    // GUI state
    show_checkboxes = obj.show_checkboxes;
    show_data = obj.show_data;
    show_options = obj.show_options;
    data_window_size = obj.data_window_size;
    data_window = new std::string[data_window_size];        // No need to delete[] now, it haven't been allocated yet
    for(int i = 0; i < data_window_size; i++) data_window[i] = obj.data_window[i];

    for(int i = 0; i < 3; ++i) backg_color[i] = obj.backg_color[i];
    point_siz = obj.point_siz;

    // Points selection
    MinDistance = obj.MinDistance;
    for(int i = 0; i < 4; ++i) selection_color[i] = obj.selection_color[i];
    temp_selections = obj.temp_selections;
}

plotter& plotter::operator=(const plotter &obj)
{
    layersSet = obj.layersSet;

    //cam = obj.cam;

    //window = obj.window;
    display_w = obj.display_w;  display_h = obj.display_h;

    // GUI state
    show_checkboxes = obj.show_checkboxes;
    show_data = obj.show_data;
    show_options = obj.show_options;
    data_window_size = obj.data_window_size;
    delete[] data_window;
    data_window = new std::string[data_window_size];
    for(int i = 0; i < data_window_size; i++) data_window[i] = obj.data_window[i];
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
    delete[] data_window;
    // delete window;
}

int  plotter::open_window()
{
    win.init_GLFW();

    std::thread running(&plotter::main_loop_thread, this);
    running.detach();

    return 0;
}

void plotter::fill_data_window(const std::string *data_strings, int num_strings) {

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

void plotter::resize_buffer_set(size_t new_size)
{
    if(vertexbuffersIDs != nullptr)
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

// Private members -------------------------------------------------------

int  plotter::main_loop_thread()
{
    win.open_window();
std::cout << "XXX" << std::endl;
    // Initialize GLEW
    glewExperimental = true;										// Needed for core profile
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    std::cout << "XXX" << std::endl;
    // ------------------------------------

    // GUI
    std::cout << "ImGui version: " << IMGUI_CHECKVERSION() << std::endl;
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();		//ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(win.window, true);
    const char* glsl_version = VERSION_GLFW_for_IMGUI;
    ImGui_ImplOpenGL3_Init(glsl_version);
    std::cout << "XXX" << std::endl;
    // ------------------------------------

    cam.adjustments(win.window);

    // Set the mouse at the center of the screen
    glfwPollEvents();
    glfwSetCursorPos(win.window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

    glClearColor(backg_color[0], backg_color[1], backg_color[2], 0.0f);     // Background color

    glEnable(GL_DEPTH_TEST);					// Enable depth test
    glDepthFunc(GL_LESS);						// Accept fragment if it's closer to the camera than the former one

    // Cull triangles which normal is not towards the camera.
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
    std::cout << "XXX" << std::endl;
    // ------------------------------------

    // Vertex array object (VAO)
    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders				    system("pwd"): /home/hank/dev/OGL/Shaper/_BUILD
    //GLuint programID = LoadShaders(	"//home//hank//src//TransformVertexShader.vertexshader", "//home//hank//src//ColorFragmentShader.fragmentshader");
    GLuint programID = LoadShaders(VertexShaderCode, FragmentShaderCode);
    GLuint programID_selection = LoadShaders(VertexShaderCode2D, FragmentShaderCode);

    // Get a handle for our uniforms: MVP, camera position and coordinates
    GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint Cam_position = glGetUniformLocation(programID, "Cam_pos");
    GLuint Pnt_size_ID = glGetUniformLocation(programID, "Pnt_siz");

    // GL_STATIC_DRAW example -------------
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
    // ------------------------------------

    {
        std::lock_guard<std::mutex> lock(*layersSet_mut);

        vertexbuffersIDs = new GLuint[layersSet->size()];    // The first buffer is for the selected points
        colorbuffersIDs = new GLuint[layersSet->size()];
        selectionSquareID = new GLuint;
        selectionSquareColorID = new GLuint;

        glGenBuffers(layersSet->size(), vertexbuffersIDs);
        glGenBuffers(layersSet->size(), colorbuffersIDs);
        glGenBuffers(1, selectionSquareID);
        glGenBuffers(1, selectionSquareColorID);
    }

    // ------------------------------------
    do
    {
        time_1 = std::chrono::high_resolution_clock::now();
        if(!win.window_open) break;

        win.GetFramebufferSize(&display_w, &display_h);
        glViewport(0, 0, display_w, display_h);                 // Arguments: Lower left
        glClearColor(backg_color[0], backg_color[1], backg_color[2], 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the screen and the depth test

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input. Also check whether a selection was made.
        {
            std::lock_guard<std::mutex> lock_controls(cam_mut);                 // computeMatricesFromInputs() passes 2 functions to GLFW as callbacks (mouseButtonCallback, scrollCallback). Both need an object of type "control" to make changes on it. Since it's not possible to pass that object because we can't modify its argument lists to do so (GLFW functions puts the arguments), I decided to use a global control* (so the callback functions use this object) and a global mutex (to control accesses from different visualizerClass objects to this pointer). Hence, multiple visualizer windows are possible.
            camera = &cam;
            if (!io.WantCaptureMouse) cam.computeMatricesFromInputs(win.window);    // io.WantCaptureMouse and io.WantCaptureKeyboard flags are true if dear imgui wants to use our inputs (i.e. cursor is hovering a window).
        
			ProjectionMatrix = cam.getProjectionMatrix();
			ViewMatrix = cam.getViewMatrix();
			ModelMatrix = glm::mat4(1.0);                     // Identity matrix
			MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

            check_selections();
		}

        // Send our transformation to the currently bound shader, in the "MVP" uniform
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
		// Send the position of the camera. Useful for adjusting the size of each point
		glUniform3fv(Cam_position, 1, &cam.position[0]);
        // Send the size of the points
        glUniform1fv(Pnt_size_ID, 1, &point_siz);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        std::lock_guard<std::mutex> lock(*layersSet_mut);
        load_buffers(vertexbuffersIDs, colorbuffersIDs);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        glUseProgram(programID_selection);
        load_selectionSquare(selectionSquareID, selectionSquareColorID);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

        // ImGui rendering
        create_windows();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        win.SwapBuffers_PollEvents();

        for(layer &lay : *layersSet) lay.state = open;

        time_2 = std::chrono::high_resolution_clock::now();
        fps_control(DESIRED_FPS);
    }
    // Check if the ESC key was pressed or the window was closed
    while ((glfwGetKey(win.window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(win.window) == 0) && win.window_open);

    // ------------------------------------

    // ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup VBO and shader
    glDeleteBuffers(layersSet->size(), vertexbuffersIDs);       delete[] vertexbuffersIDs;    vertexbuffersIDs = nullptr;
    glDeleteBuffers(layersSet->size(), colorbuffersIDs);        delete[] colorbuffersIDs;     colorbuffersIDs  = nullptr;
    glDeleteBuffers(1, selectionSquareID);
    glDeleteBuffers(1, selectionSquareColorID);

    glDeleteProgram(programID);
    glDeleteProgram(programID_selection);

    glDeleteVertexArrays(1, &VertexArrayID);

    win.Terminate();

    return 0;
}

void plotter::create_windows() {

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

            for(size_t i = 0; i < layersSet->size(); i++)
                ImGui::Checkbox((layersSet->operator[](i).layer_name).c_str(), &layersSet->operator[](i).checkbox_value);

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

        ImGui::SliderFloat("Point size", &point_siz, 1.0f, MAX_PNT_SIZE);

        ImGui::Separator;

        float alpha_value;
        for(size_t i = 0; i < layersSet->size(); i++)
        {
            alpha_value = layersSet->operator[](i).alpha_channel;
            ImGui::SliderFloat((layersSet->operator[](i).layer_name).c_str(), &alpha_value, 0.0f, 1.0f);
            layersSet->operator[](i).set_alpha_channel(alpha_value);
        }

        ImGui::End();
    }
}

void plotter::create_demo_windows() {
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

void plotter::check_selections(){

    if (cam.R_just_released)
    {
    // >>> Fill the MVP arrays for the square selection (projmatrix[], mvmatrix[], viewport[]):

        glGetIntegerv(GL_VIEWPORT, viewport);
        // glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);			// See: glMatrixMode, glPushMatrix, glGet
        // glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);

        glm::mat4 modelviewmatrix = ViewMatrix * ModelMatrix;

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                projmatrix[i * 4 + j] = ProjectionMatrix[i][j];
                mvmatrix[i * 4 + j] = modelviewmatrix[i][j];
            }

    // >>> Forbid new buffers editions in this main-loop iteration. If any buffer is being edited now, there is a lock_guard ahead to avoid conflicts.
        for (size_t i = 0; i < layersSet->size(); i++) layersSet->operator[](i).state = closed;
        // Clear the vector (size = 0, but keeps previous capacity) that will pass the selected points to the layer
        temp_selections.clear();

    // >>> Point selection search

        //check_ray(cam.sel_xpos0, cam.sel_ypos0);      // One point selection way

        // Check the direction to where you draw the selection square
        signed int xstep, ystep;
        if(cam.sel_xpos0 <= cam.sel_xpos) xstep = 1;
        else xstep = -1;
        if(cam.sel_ypos0 <= cam.sel_ypos) ystep = 1;
        else ystep = -1;

        // Send a ray for each pixel inside the selection square
        for(double column = cam.sel_xpos0; ; column += xstep)
        {
            if(xstep == 1 && column > cam.sel_xpos) break;
            else if(xstep == -1 && column < cam.sel_xpos) break;

            for(double row = cam.sel_ypos0; ; row += ystep)
            {
                if(ystep == 1 && row > cam.sel_ypos) break;
                else if(ystep == -1 && row < cam.sel_ypos) break;

                check_ray(column, row);
            }
        }

        // Copy all the selected points to a layer (Selections)
        layersSet->operator[](0) = layer("Selections", points, temp_selections.size());
        layersSet->operator[](0).objs_to_print = temp_selections.size();
        for(size_t i = 0; i < temp_selections.size(); i++)
        {
            layersSet->operator[](0).points_buffer[i][0] = temp_selections[i][0];
            layersSet->operator[](0).points_buffer[i][1] = temp_selections[i][1];
            layersSet->operator[](0).points_buffer[i][2] = temp_selections[i][2];

            layersSet->operator[](0).points_color_buffer[i][0] = selection_color[0];
            layersSet->operator[](0).points_color_buffer[i][1] = selection_color[1];
            layersSet->operator[](0).points_color_buffer[i][2] = selection_color[2];
            layersSet->operator[](0).points_color_buffer[i][3] = selection_color[3];
        }

        std::cout << "\n----- Selections -----" << std::endl;
        for(size_t i = 0; i < layersSet->size(); i++)
        {
            if(layersSet->operator[](i).layer_type == points)
                for(size_t j = 0; j < layersSet->operator[](i).objs_to_print; j++)
                    if(layersSet->operator[](i).points_strings[j] != "") std::cout << layersSet->operator[](i).points_strings[j] << std::endl;
        }

        cam.R_just_released = false;
    }
}

void plotter::check_ray(double xpos, double ypos) {

    double dX, dY, dZ;                      // Temporary variables
    double dClickY, dClickX;                // OGL screen coordinates of the pixel
    glm::vec3 ClickRayP1, ClickRayP2;       // OGL world coordinates of the pixel ray ends (in the near and far clip plane).
    glm::vec3 pointRayP1, pointRayP2;       // OGL world coordinates of the point ray ends (in the near clip plane and the point).
    glm::vec3 ClickSlope, pointSlope;       // Direction vectors: Directions (unitary vector) of the pixel ray and the point's ray
    double sqrDist, minSqrDist = MinDistance * MinDistance;

    // From the screen coordinates, get the 2 ray ends in world-space (they are in the near and far clip plane)
    dClickX = xpos;
    dClickY = double(display_h - ypos);         // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

    gluUnProject(dClickX, dClickY, 0.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);		// 0.0: Near clip plane
	ClickRayP1 = glm::vec3(dX, dY, dZ);
	gluUnProject(dClickX, dClickY, 1.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);		// 1.0: Far clip plane
	ClickRayP2 = glm::vec3(dX, dY, dZ);

    // Get the direction vector of the pixel ray
    ClickSlope = ClickRayP2 - ClickRayP1;           // Get the direction vector
    cam.normalize_vec(ClickSlope);                  // Get unitary direction vector

    // Find the closest points by testing which points' rays are close to the clickRay (checks all the points)
    for (size_t i = 0; i < layersSet->size(); i++)
	{
        if(layersSet->operator[](i).layer_type != points || !layersSet->operator[](i).checkbox_value) continue;

        std::lock_guard<std::mutex> lock(*layersSet->operator[](i).mut);       // This second filther (after setting buffer_closed to closed) avoid conflicts with buffers being currently edited (can only happen during the first iteration of this loop for each layer)

        for (size_t j = 0; j < layersSet->operator[](i).objs_to_print; j++)
		{
            // TODO <<<<<<<<<< If this point was already selected (by a different ClickRay), don't check again whether it is selected

            // Point's ray is formed by the vector between the selectable point and the origin point of the clickRay
			pointRayP1 = ClickRayP1;
            pointRayP2 = glm::vec3(	layersSet->operator[](i).points_buffer[j][0],
                                    layersSet->operator[](i).points_buffer[j][1],
                                    layersSet->operator[](i).points_buffer[j][2] );

            // Get the direction vector of the point ray
            pointSlope = pointRayP2 - pointRayP1;   // Get the direction vector
            cam.normalize_vec(pointSlope);          // Get unitary direction vector

            // Check distance between the ends the direction vector of the pixel ray and the point ray
            sqrDist = cam.distance_sqr_vec(pointSlope, ClickSlope);
            if (sqrDist < minSqrDist)
                temp_selections.push_back(glm::vec3(layersSet->operator[](i).points_buffer[j][0],
                                                    layersSet->operator[](i).points_buffer[j][1],
                                                    layersSet->operator[](i).points_buffer[j][2]));
        }
	}
}

void plotter::load_buffers(GLuint *vertexbuffIDs, GLuint *colorbuffIDs)
{
    for(size_t i = 0; i < layersSet->size(); i++)
    {
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

        if (layersSet->operator[](i).checkbox_value)
            glDrawArrays(GL_OBJS, 0, layersSet->operator[](i).objs_to_print * vertex_per_obj);
        else
            glDrawArrays(GL_OBJS, 0, 0);
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
