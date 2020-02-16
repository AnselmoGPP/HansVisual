#include "visualizerClass.hpp"

// Main public members ---------------------------------------------------

visualizerClass::visualizerClass()
{
     char test2[21] = { 0x3c, 0x3c, 0x3c, 0x20, 0x20, 0x48, 0x61,
                        0x6e, 0x73, 0x56, 0x69, 0x73, 0x75, 0x61,
                        0x6c, 0x20, 0x20, 0x3e, 0x3e, 0x3e, 0x00 };
     for(int i = 0; i < 21; i++) test[i] = test2[i];

    layersSet.push_back(layer("Selections", points, 0));

    // Data window
	data_window_size = 1;
    data_window = new std::string[data_window_size];

    // Rainbow palette
    for (int i = 0; i < 256; i++) {
        if (i <= 155) {
            modified_rainbow[255-i][0] = i * 60./155.;
            modified_rainbow[255-i][1] = 1.;
            modified_rainbow[255 - i][2] = 1.;
        }
        if (i > 155) {
            modified_rainbow[255-i][0] = 60. + (i-155) * (240.-60.)/(255.-155.);
            modified_rainbow[255-i][1] = 1.;
            modified_rainbow[255-i][2] = 1.;
        }
    }
    layer temp;
    temp.convert_HSVtoRGB(&modified_rainbow[0][0], 256);
}

visualizerClass::visualizerClass(const visualizerClass &obj)
{

}

visualizerClass& visualizerClass::operator=(const visualizerClass &obj) {

    layersSet = obj.layersSet;

    for (int i = 0; i < 3; i++) backg_color[i] = obj.backg_color[i];

    // GUI state -----
    show_checkboxes = obj.show_checkboxes;
    show_data = obj.show_data;
    show_options = obj.show_options;

    data_window_size = obj.data_window_size;
    delete[] data_window;
    data_window = new std::string[data_window_size];
    for(int i = 0; i < data_window_size; i++) data_window[i] = obj.data_window[i];

    return *this;
}

visualizerClass::~visualizerClass()
{
    delete[] data_window;
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

void visualizerClass::add_layer(const char *name, object_type type, unsigned int capacity)
{
    layersSet.push_back(layer(name, type, capacity));
}

void visualizerClass::send_points(std::string layer_name, unsigned int number_points, const float *arr, const float *labels,  std::string *points_data, data_buffer array_type, float min, float max)
{
    bool layer_founded = false;

    for(size_t i = 0; i < layersSet.size(); i++)
        if(layersSet[i].layer_name == layer_name)
        {
            layersSet[i].state = half_closed;
            layersSet[i].save_points(number_points, arr, labels, points_data, array_type, min, max);
            layer_founded = true;
        }

    if(!layer_founded) std::cout << "Layer \"" << layer_name << "\" not founded" << std::endl;
}

void visualizerClass::send_lines(std::string layer_name, unsigned int number_points, const float *arr, const float *labels, data_buffer array_type, float min, float max)
{
    bool layer_founded = false;

    for(size_t i = 0; i < layersSet.size(); i++)
        if(layersSet[i].layer_name == layer_name)
        {
            layersSet[i].state = half_closed;
            layersSet[i].save_lines(number_points, arr, labels, array_type, min, max);
            layer_founded = true;
        }

    if(!layer_founded) std::cout << "Layer \"" << layer_name << "\" not founded" << std::endl;
}

void visualizerClass::send_triangles(std::string layer_name, unsigned int number_triangles, const float *arr, const float *labels, data_buffer array_type, float min, float max)
{
    bool layer_founded = false;

    for(size_t i = 0; i < layersSet.size(); i++)
        if(layersSet[i].layer_name == layer_name)
        {
            layersSet[i].state = half_closed;
            layersSet[i].save_triangles(number_triangles, arr, labels, array_type, min, max);
            layer_founded = true;
        }

    if(!layer_founded) std::cout << "Layer \"" << layer_name << "\" not founded" << std::endl;
}

void visualizerClass::send_cubes(std::string layer_name, unsigned int number_cubes, const cube3D *arr, const float *labels, data_buffer array_type, float min, float max)
{
    bool layer_founded = false;

    for(size_t i = 0; i < layersSet.size(); i++)
        if(layersSet[i].layer_name == layer_name)
        {
            layersSet[i].state = half_closed;
            layersSet[i].save_cubes(number_cubes, arr, labels, array_type, min, max);
            layer_founded = true;
        }

    if(!layer_founded) std::cout << "Layer \"" << layer_name << "\" not founded" << std::endl;
}

// Auxiliary public members ----------------------------------------------

void visualizerClass::send_palette_RGB_01(std::string layer_name, float *new_palette, int number_colors)
{
    bool layer_founded = false;

    for(size_t i = 0; i < layersSet.size(); i++)
        if(layersSet[i].layer_name == layer_name)
        {
            layersSet[i].save_palette_RGB_01(new_palette, number_colors);
            layer_founded = true;
        }

    if(!layer_founded) std::cout << "Layer \"" << layer_name << "\" not founded" << std::endl;
}

void visualizerClass::send_palette_RGB(std::string layer_name, float *new_palette, int number_colors)
{
    bool layer_founded = false;

    for(size_t i = 0; i < layersSet.size(); i++)
        if(layersSet[i].layer_name == layer_name)
        {
            layersSet[i].save_palette_RGB(new_palette, number_colors);
            layer_founded = true;
        }

    if(!layer_founded) std::cout << "Layer \"" << layer_name << "\" not founded" << std::endl;
}

void visualizerClass::send_palette_HSV(std::string layer_name, float *new_palette, int number_colors)
{
    bool layer_founded = false;

    for(size_t i = 0; i < layersSet.size(); i++)
        if(layersSet[i].layer_name == layer_name)
        {
            layersSet[i].save_palette_HSV(new_palette, number_colors);
            layer_founded = true;
        }

    if(!layer_founded) std::cout << "Layer \"" << layer_name << "\" not founded" << std::endl;
}


void visualizerClass::convert_HSVtoRGB(float *colors, int num_colors)
{
    layer obj;
    obj.convert_HSVtoRGB(colors, num_colors);
}

void visualizerClass::convert_RGB255toRGB(float *colors, int num_colors)
{
    layer obj;
    obj.convert_RGB255toRGB(colors, num_colors);
}

void visualizerClass::transform_coordinates(float *points_arr, int number_points)
{
    layer obj;
    obj.transform_coordinates(points_arr, number_points);
}

void visualizerClass::polynomial_graph(float (*result)[3], float min_x, float max_x, int sample_size, float *coefficients, float number_of_coefficients)
{
    layer obj;
    obj.polynomial_graph(result, min_x, max_x, sample_size, coefficients, number_of_coefficients);
}

void visualizerClass::icosahedron(float side_length, float(*points)[3])
{
    layer obj;
    obj.icosahedron(side_length, points);
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

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);    // Show only the borders of the triangles
    glLineWidth(2.0);
    GLfloat lineWidthRange[2];                  // GPU implementation dependent
    glGetFloatv(GL_ALIASED_LINE_WIDTH_RANGE, lineWidthRange);



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

    GLuint *vertexbuffersIDs = new GLuint[layersSet.size()];    // The first buffer is for the selected points
    glGenBuffers(layersSet.size(), vertexbuffersIDs);
    GLuint *colorbuffersIDs = new GLuint[layersSet.size()];
    glGenBuffers(layersSet.size(), colorbuffersIDs);

    GLuint *selectionSquareID = new GLuint;
    glGenBuffers(1, selectionSquareID);
    GLuint *selectionSquareColorID = new GLuint;
    glGenBuffers(1, selectionSquareColorID);

    // Main loop --------------------------------------------------------
    do
    {
        time_1 = std::chrono::high_resolution_clock::now();

        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);	// Arguments: Lower left 
        glClearColor(backg_color[0], backg_color[1], backg_color[2], 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);     // Clear the screen and the depth test

        // Use our shader
        glUseProgram(programID);

        // Compute the MVP matrix from keyboard and mouse input. Also check whether a selection was made.
        {
            std::lock_guard<std::mutex> lock_controls(cam_mut);                 // computeMatricesFromInputs() passes 2 functions to GLFW as callbacks (mouseButtonCallback, scrollCallback). Both need an object of type "control" to make changes on it. Since it's not possible to pass that object because we can't modify its argument lists to do so (GLFW functions puts the arguments), I decided to use a global control* (so the callback functions use this object) and a global mutex (to control accesses from different visualizerClass objects to this pointer). Hence, multiple visualizer windows are possible.
            camera = &cam;
            if (!io.WantCaptureMouse) cam.computeMatricesFromInputs(window);    // io.WantCaptureMouse and io.WantCaptureKeyboard flags are true if dear imgui wants to use our inputs (i.e. cursor is hovering a window).
        
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
        glfwSwapBuffers(window);
        glfwPollEvents();

        for(layer &lay : layersSet) lay.state = open;

        time_2 = std::chrono::high_resolution_clock::now();
        fps_control(60);
    }
    // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && glfwWindowShouldClose(window) == 0);

    // ImGui cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    // Cleanup VBO and shader
    glDeleteBuffers(layersSet.size(), vertexbuffersIDs);
    glDeleteBuffers(layersSet.size(), colorbuffersIDs);
    glDeleteBuffers(1, selectionSquareID);
    glDeleteBuffers(1, selectionSquareColorID);

    glDeleteProgram(programID);
    glDeleteProgram(programID_selection);
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

            for(size_t i = 0; i < layersSet.size(); i++)
                ImGui::Checkbox((layersSet[i].layer_name).c_str(), &layersSet[i].checkbox_value);

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

        float alpha_value;
        for(size_t i = 0; i < layersSet.size(); i++)
        {
            alpha_value = layersSet[i].alpha_channel;
            ImGui::SliderFloat((layersSet[i].layer_name).c_str(), &alpha_value, 0.0f, 1.0f);
            layersSet[i].set_alpha_channel(alpha_value);
        }

        ImGui::End();
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

void visualizerClass::check_selections(){

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
        for (size_t i = 0; i < layersSet.size(); i++) layersSet[i].state = closed;
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
        layersSet[0] = layer("Selections", points, temp_selections.size());
        layersSet[0].objs_to_print = temp_selections.size();
        for(size_t i = 0; i < temp_selections.size(); i++)
        {
            layersSet[0].points_buffer[i][0] = temp_selections[i][0];
            layersSet[0].points_buffer[i][1] = temp_selections[i][1];
            layersSet[0].points_buffer[i][2] = temp_selections[i][2];

            layersSet[0].points_color_buffer[i][0] = selection_color[0];
            layersSet[0].points_color_buffer[i][1] = selection_color[1];
            layersSet[0].points_color_buffer[i][2] = selection_color[2];
            layersSet[0].points_color_buffer[i][3] = selection_color[3];
        }

        std::cout << "\n----- Selections -----" << std::endl;
        for(size_t i = 0; i < layersSet.size(); i++)
        {
            if(layersSet[i].layer_type == points)
                for(size_t j = 0; j < layersSet[i].objs_to_print; j++)
                    if(layersSet[i].points_strings[j] != "") std::cout << layersSet[i].points_strings[j] << std::endl;
        }

        cam.R_just_released = false;
    }
}

void visualizerClass::check_ray(double xpos, double ypos) {

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
    for (size_t i = 0; i < layersSet.size(); i++)
	{
        if(layersSet[i].layer_type != points || !layersSet[i].checkbox_value) continue;

        std::lock_guard<std::mutex> lock(*layersSet[i].mut);       // This second filther (after setting buffer_closed to closed) avoid conflicts with buffers being currently edited (can only happen during the first iteration of this loop for each layer)

        for (size_t j = 0; j < layersSet[i].objs_to_print; j++)
		{
            // TODO <<<<<<<<<< If this point was already selected (by a different ClickRay), don't check again whether it is selected

            // Point's ray is formed by the vector between the selectable point and the origin point of the clickRay
			pointRayP1 = ClickRayP1;
            pointRayP2 = glm::vec3(	layersSet[i].points_buffer[j][0],
                                    layersSet[i].points_buffer[j][1],
                                    layersSet[i].points_buffer[j][2] );

            // Get the direction vector of the point ray
            pointSlope = pointRayP2 - pointRayP1;   // Get the direction vector
            cam.normalize_vec(pointSlope);          // Get unitary direction vector

            // Check distance between the ends the direction vector of the pixel ray and the point ray
            sqrDist = cam.distance_sqr_vec(pointSlope, ClickSlope);
            if (sqrDist < minSqrDist)
                temp_selections.push_back(glm::vec3(layersSet[i].points_buffer[j][0],
                                                    layersSet[i].points_buffer[j][1],
                                                    layersSet[i].points_buffer[j][2]));
        }
	}
}

void visualizerClass::load_buffers(GLuint *vertexbuffIDs, GLuint *colorbuffIDs)
{
    for(size_t i = 0; i < layersSet.size(); i++)
    {
        unsigned long vertex_per_obj;
        GLenum GL_OBJS;

        if     (layersSet[i].layer_type == points)
        {
            vertex_per_obj = 1;
            GL_OBJS = GL_POINTS;
        }
        else if(layersSet[i].layer_type == lines)
        {
            vertex_per_obj = 2;
            GL_OBJS = GL_LINES;
        }
        else if(layersSet[i].layer_type == triangles)
        {
            vertex_per_obj = 3;
            GL_OBJS = GL_TRIANGLES;
        }
        else if(layersSet[i].layer_type == cubes)
        {
            vertex_per_obj = 3*12;
            GL_OBJS = GL_TRIANGLES;
        }

        {
            std::lock_guard<std::mutex> lock(*layersSet[i].mut);
            glBindBuffer(GL_ARRAY_BUFFER, vertexbuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, layersSet[i].objs_to_print * vertex_per_obj * 3 * sizeof(float), layersSet[i].get_vertex_ptr(), GL_DYNAMIC_DRAW);				// GL_STATIC_DRAW

            glBindBuffer(GL_ARRAY_BUFFER, colorbuffIDs[i]);
            glBufferData(GL_ARRAY_BUFFER, layersSet[i].objs_to_print * vertex_per_obj * 4 * sizeof(float), layersSet[i].get_colors_ptr(), GL_DYNAMIC_DRAW);
        }

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffIDs[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffIDs[i]);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

        if (layersSet[i].checkbox_value)
            glDrawArrays(GL_OBJS, 0, layersSet[i].objs_to_print * vertex_per_obj);
        else
            glDrawArrays(GL_OBJS, 0, 0);
    }
}

void visualizerClass::load_selectionSquare(GLuint *selectionSquareID, GLuint *selectionColorID)
{
    if(cam.is_R_pressed())
    {
        // Set positions
        float square[5][3];
        double x, y, x0, y0;
        x =     (cam.sel_xpos - display_w/2)/(display_w/2);
        x0 =    (cam.sel_xpos0 - display_w/2)/(display_w/2);
        y =    -(cam.sel_ypos - display_h/2)/(display_h/2);
        y0 =   -(cam.sel_ypos0 - display_h/2)/(display_h/2);

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

        // Set colors
        float square_colors[5][4];
        for(int i = 0; i < 5; i++)
        {
            square_colors[i][0] = selection_color[0];
            square_colors[i][1] = selection_color[1];
            square_colors[i][2] = selection_color[2];
            square_colors[i][3] = selection_square.alpha_channel;
        }

        selection_square.save_points(5, &square[0][0], &square_colors[0][0], nullptr, colors);


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

void visualizerClass::fps_control(unsigned int frequency)
{
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(time_2 - time_1).count();
    long desired_time = 1000000/frequency;
    if(duration < desired_time) std::this_thread::sleep_for(std::chrono::microseconds(desired_time - duration));

    duration = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - time_1).count();
    //std::cout << "FPS: " << 1000000/duration << '\r';
}
