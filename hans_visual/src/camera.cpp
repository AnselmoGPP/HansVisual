#include <iostream>
#include <cmath>
//#include <iomanip>

#include "_options.hpp"
#include "camera.hpp"

void glfw_timer::get_delta_time()
{
    // glfwGetTime is called only once, the first time this function is called
    if (!lastTime) lastTime = glfwGetTime();

    // Compute time difference between current and last frame
    currentTime = glfwGetTime();
    deltaTime = float(currentTime - lastTime);

    // For the next frame, the "last time" will be "now"
    lastTime = currentTime;
}

// Camera class ------------------------------------------------------

camera::camera(glm::vec3 cam_pos, glm::vec3 cam_dir, glm::vec3 up_vec, float FV)
    : window(nullptr),
      kc(keys_controller::get_instance()),
      cam_position(cam_pos),
      cam_direction(cam_dir),
      up_vector(up_vec),
      FoV(FV),
      pi(3.1415926536f) { }

glm::mat4 camera::getViewMatrix() { return ViewMatrix; }
glm::mat4 camera::getProjectionMatrix() { return ProjectionMatrix; }

void camera::adjust_moves_to_range_2pi(float &lat, float &lon)
{
    if(lon >= 2*pi)  lon -= 2*pi;
    else if(lon < 0) lon += 2*pi;

    if(lat >= 2*pi)  lat -= 2*pi;
    else if(lat < 0) lat += 2*pi;
}

void camera::associate_window(GLFWwindow *window_in) { window = window_in; }

void camera::hide_cursor(bool hide)
{
    // GLFW_CURSOR_DISABLED, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_NORMAL
    if(hide) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);     //GLFW_CURSOR_DISABLED: Hide the mouse and enable unlimited movement
    else     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

// Sphere class ------------------------------------------------------

sphere::sphere() : camera(glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), 45.0f)
{
    sphere_center = glm::vec3(0, 0, 0);
    radius = 15;
    minimum_radius = 1;
    lat = 3.14f/2; lon = 3.14f/2;           // Initial position   <<<<<<<<<<<<< Test and take notes about how orientation works

    mouse_speed = 0.008f;
    scroll_speed = 1;
    right_speed = 0.001f;
}

// Spherical controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_NORMAL
void sphere::computeMatricesFromInputs(float aspect_ratio)
{
    // Left click: Rotate around a sphere
    // Scroll roll: Get closer or further
    // Scroll click: Normal translation

    ProjectionMatrix = glm::perspective(glm::radians(FoV), aspect_ratio, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

    // >>> Scroll roll <<<

    // Set sphere radius
    if(ENABLE_SCROLL_SPEEDUP)   radius -= kc->scroll_yOffset * scroll_speed * (SCROLL_SPEEDUP * radius);
    else                        radius -= kc->scroll_yOffset * scroll_speed;

    if (radius < minimum_radius) radius = minimum_radius;

    // >>> Left mouse button <<<

    get_cursor_pos_increment();

    //Get angular position over the sphere (latitude & longitude)
    if(kc->L_pressed)
    {
        if(lat > (pi/2.f) && lat < (3*pi/2.f))      // Range (pi/2, 3pi/2)
        {
            lon -= mouse_speed * xIncrem;
            lat += mouse_speed * yIncrem;
        }
        else
        {
            lon += mouse_speed * xIncrem;
            lat += mouse_speed * yIncrem;
        }

        adjust_moves_to_range_2pi(lat, lon);
    }

    // Get the position over the sphere
    cam_position =  sphere_center + glm::vec3(
                                                radius * cos(lat) * cos(lon),
                                                radius * cos(lat) * sin(lon),
                                                radius * sin(lat) );

    // Vector director: Direction towards the sphere's center
    cam_direction = (sphere_center - cam_position) / radius;

    // Right vector (z=0)
    glm::vec3 right = glm::vec3(
                cos(lon - 3.14f / 2.0f),
                sin(lon - 3.14f / 2.0f),
                0 );

    // Up vector
    up_vector = glm::cross(right, cam_direction);

    // >>> View matrix <<<

    ViewMatrix = glm::lookAt(cam_position, sphere_center, up_vector);

    // >>> Scroll pressed <<<

    if (kc->scroll_pressed)
        sphere_center += right_speed * radius * ((-right * xIncrem) + (-up_vector * yIncrem));
}

void sphere::set_mouse_position_visibility() {

    // Mouse visibility
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);      // GLFW_CURSOR_DISABLED/HIDDEN/NORMAL (DISABLED: Hide the mouse and enable unlimited movement)

    // Set the mouse at the center of the screen
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    glfwPollEvents();
}

void sphere::get_cursor_pos_increment()
{
    xIncrem = kc->xpos - kc->xpos0;
    yIncrem = kc->ypos0 - kc->ypos;         // Correction done so Y go upwards (not downwards)
}

// FirstPerson class ------------------------------------------------------

firstPerson::firstPerson() : camera(glm::vec3(0, -15, 15), glm::vec3(0, 15, -15), glm::vec3(0, -15, 15), 45.0f)
{
    lon = 0;
    lat = 0;

    walk_speed = 1.f;           // X units / second
    mouse_speed = 0.001f;
}

// FP controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_DISABLED
void firstPerson::computeMatricesFromInputs(float aspect_ratio)
{
    ProjectionMatrix = glm::perspective(glm::radians(FoV), aspect_ratio, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

    // Directional keys: Move camera forward, backwards, to the right, to the left
    // Mouse movement: Orientate camera

    timer.get_delta_time();

    // Reset mouse position for next frame
    kc->setCursorPosInNextKeyUpdate(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);   // <<<<<<<< Window size will be variable with C_PRESS camera change

    // >>> Mouse movement <<<

    // Compute new orientation
    lon +=  mouse_speed * float(WINDOW_WIDTH  / 2 - kc->xpos);
    lat +=  mouse_speed * float(WINDOW_HEIGHT / 2 - kc->ypos);

    adjust_moves_to_range_2pi(lat, lon);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    cam_direction = glm::vec3(
                                cos(lat) * cos(lon),
                                sin(lon) * cos(lat),
                                sin(lat) );

    // Right vector (y = 0)
    glm::vec3 right = glm::vec3(
                cos(lon - 3.14f / 2.0f),
                sin(lon - 3.14f / 2.0f),
                0 );

    // Up vector
    up_vector = glm::cross(right, cam_direction);

    // >>> Directional keys <<<

    if (kc->up_press    || kc->w_press) cam_position += cam_direction * timer.deltaTime * walk_speed;
    if (kc->down_press  || kc->s_press) cam_position -= cam_direction * timer.deltaTime * walk_speed;
    if (kc->right_press || kc->d_press) cam_position += right         * timer.deltaTime * walk_speed;
    if (kc->left_press  || kc->a_press) cam_position -= right         * timer.deltaTime * walk_speed;

    // >>> Camera matrix <<<

    ViewMatrix = glm::lookAt( cam_position, cam_position + cam_direction, up_vector );      // Head is up (set to 0,-1,0 to look upside-down)
}

void firstPerson::set_mouse_position_visibility() {

    // Mouse visibility
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);      // GLFW_CURSOR_DISABLED/HIDDEN/NORMAL (DISABLED: Hide the mouse and enable unlimited movement)

    // Set the mouse at the center of the screen
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    glfwPollEvents();
}
