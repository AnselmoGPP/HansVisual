
#include "gui.hpp"

#include <iostream>

my_gui::my_gui()
{
    glsl_version = VERSION_GLFW_for_IMGUI;

    show_checkboxes = SHOW_CHECKBOXES;
    show_data = SHOW_DATA;
    show_options = SHOW_OPTIONS;

    data_window_size = 1;
    data_window = new std::string[data_window_size];
}

my_gui::my_gui(const my_gui &obj)
{
    glsl_version = obj.glsl_version;

    show_checkboxes = obj.show_checkboxes;
    show_data = obj.show_data;
    show_options = obj.show_options;

    data_window_size = obj.data_window_size;
    data_window = new std::string[obj.data_window_size];        // No need to delete[] now, it haven't been allocated yet
    for(int i = 0; i < data_window_size; i++) data_window[i] = obj.data_window[i];
}

my_gui & my_gui::operator=(const my_gui &obj)
{
    glsl_version = obj.glsl_version;

    show_checkboxes = obj.show_checkboxes;
    show_data = obj.show_data;
    show_options = obj.show_options;

    data_window_size = obj.data_window_size;
    delete[] data_window;
    data_window = new std::string[obj.data_window_size];
    for(int i = 0; i < data_window_size; i++) data_window[i] = obj.data_window[i];

    return *this;
}

void my_gui::init_gui()
{
    std::cout << "ImGui version: " << IMGUI_CHECKVERSION() << std::endl;

    ImGui::CreateContext();
    io = &ImGui::GetIO();           // ImGuiIO& io = ImGui::GetIO();   ---> This function returns a reference
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    ImGui::StyleColorsDark();		//ImGui::StyleColorsClassic();

    // Setup Platform/Renderer bindings (call these 2 lines after just after calling init_gui())
    //ImGui_ImplGlfw_InitForOpenGL(win, true);
    //ImGui_ImplOpenGL3_Init(glsl_version);
}

void my_gui::new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void my_gui::create_gui_1(std::vector<layer> *layersSet, float *backg_color, float *point_siz)
{
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
                //camera = camera(SPHERE);
                //cam.camera_mode = SPHERE;
            }
            ImGui::MenuItem("FPS", NULL, &cam_FPS);
            if (cam_FPS) {
                cam_sphere = 0;
                //camera = camera(FPS);
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
                if(layersSet->operator[](i).checkbox_visible)
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

        ImGui::SliderFloat("Point size", point_siz, 1.0f, MAX_PNT_SIZE);

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

void my_gui::render_gui()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void my_gui::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void my_gui::create_demo_windows() {
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

void my_gui::fill_data_window(const std::string *data_strings, int num_strings) {

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

bool my_gui::WantCaptureMouse()
{
    // io.WantCaptureMouse and io.WantCaptureKeyboard flags are true if dear imgui wants to use our inputs (i.e. cursor is hovering a window)
    return io->WantCaptureMouse;
}
