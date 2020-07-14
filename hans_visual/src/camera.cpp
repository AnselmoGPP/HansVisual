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

// Camera factory class ----------------------------------------------

cam_factory::cam_factory()
{
    cam = nullptr;
    //cam = new sphere(glm::vec3(0, 0, 0), RADIUS, MIN_RADIUS, 3.14f/2, 3.14f/2, 45.0f);
}

cam_factory::cam_factory(const cam_factory &obj)
{
    // if(cam) delete cam;

    if     (obj.cam->get_type() == "sphere" )
    {
        cam = new sphere(glm::vec3(0,0,0), 0, 0, 0, 0, 0);
        *cam = *obj.cam;
    }
    else if(obj.cam->get_type() == "fp" )
    {
        cam = new firstPerson(glm::vec3(0,0,0), 0, 0, 0);
        *cam = *obj.cam;
    }
}

cam_factory & cam_factory::operator=(const cam_factory &obj)
{
    if(cam) delete cam;

    if     (obj.cam->get_type() == "sphere" )
    {
        cam = new sphere(glm::vec3(0,0,0), 0, 0, 0, 0, 0);
        *cam = *obj.cam;
    }
    else if(obj.cam->get_type() == "fp" )
    {
        cam = new firstPerson(glm::vec3(0,0,0), 0, 0, 0);
        *cam = *obj.cam;
    }
}

cam_factory::~cam_factory()
{
    if(cam) delete cam;
}

void cam_factory::camera_change()
{
    keys_controller *kc = keys_controller::get_instance();

    if(kc->p_press)
    {
        camera *new_cam;

        if      (cam->get_type() == "sphere")   new_cam = new firstPerson(glm::vec3(0,0,0), 0, 0);
        else if (cam->get_type() == "fp")       new_cam = new sphere(glm::vec3(0,0,0), 10, 1, 0, 0);

        *new_cam = *cam;
        new_cam->set_derived_from_base(*cam);
        delete cam;
        cam = new_cam;
    }
}


// Camera class ------------------------------------------------------

camera::camera(float FV)
    : window(nullptr),
      kc(keys_controller::get_instance()),
      FoV(FV),
      pi(3.1415926536f) { }

camera::camera(glm::vec3 cam_pos, glm::vec3 cam_dir, glm::vec3 up_vec, float FV)
    : window(nullptr),
      kc(keys_controller::get_instance()),
      cam_position(cam_pos),
      cam_direction(cam_dir),
      up_vector(up_vec),
      FoV(FV),
      pi(3.1415926536f) { }

camera::camera(const camera &obj)
    : window(obj.window),
      kc(keys_controller::get_instance()),
      cam_position(obj.cam_position),
      cam_direction(obj.cam_direction),
      up_vector(obj.up_vector),
      FoV(obj.FoV),
      pi(3.1415926536f) { }

camera & camera::operator=(const camera &obj)
{
    window = obj.window;
    kc = keys_controller::get_instance();
    cam_position = obj.cam_position;
    cam_direction = obj.cam_direction;
    up_vector = obj.up_vector;
    FoV = obj.FoV;
    pi = 3.1415926536f;

    return *this;
}

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

/*
void camera::hide_cursor(bool hide)
{
    // GLFW_CURSOR_DISABLED, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_NORMAL
    if(hide) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);     //GLFW_CURSOR_DISABLED: Hide the mouse and enable unlimited movement
    else     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}
*/

// Sphere class ------------------------------------------------------

sphere::sphere(glm::vec3 center, float rad, float min_rad, float latitude, float longitude, float FV)
    : camera(FV)
{
    sphere_center = center;
    radius = rad;
    minimum_radius = min_rad;
    lat = latitude; lon = longitude;        // Initial position   <<<<<<<<<<<<< Test and take notes about how orientation works

    mouse_speed = 0.008f;
    scroll_speed = 1;
    right_speed = 0.001f;

    set_base_from_derived();         // Set base class (camera) members values (dependent of derived class members values)
}

sphere::sphere(const sphere &obj)
    : camera(obj)
{
    sphere_center = obj.sphere_center;
    radius = obj.radius;
    minimum_radius = obj.minimum_radius;
    lat = obj.lat; lon = obj.lon;

    mouse_speed = obj.mouse_speed;
    scroll_speed = obj.scroll_speed;
    right_speed = obj.right_speed;
}

sphere & sphere::operator=(const sphere &obj)
{
    camera::operator=(obj);

    sphere_center = obj.sphere_center;
    radius = obj.radius;
    minimum_radius = obj.minimum_radius;
    lat = obj.lat; lon = obj.lon;

    mouse_speed = obj.mouse_speed;
    scroll_speed = obj.scroll_speed;
    right_speed = obj.right_speed;
}

std::string sphere::get_type() { return "sphere"; }

void sphere::computeMatricesFromInputs(float aspect_ratio)
{
// Spherical controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_NORMAL

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
            lon += mouse_speed * xIncrem;
            lat -= mouse_speed * yIncrem;
        }
        else
        {
            lon -= mouse_speed * xIncrem;
            lat -= mouse_speed * yIncrem;
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
                cos((lon+pi) - pi/2),
                sin((lon+pi) - pi/2),
                0 );

    // Up vector
    up_vector = glm::cross(right, cam_direction);

    // >>> View matrix <<<

    ViewMatrix = glm::lookAt(cam_position, sphere_center, up_vector);

    // >>> Scroll pressed <<<

    if (kc->scroll_pressed)
        sphere_center += right_speed * radius * ((-right * xIncrem) + (-up_vector * yIncrem));
}

void sphere::set_mouse_position_and_visibility() {

    // Mouse visibility
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);      // GLFW_CURSOR_DISABLED/HIDDEN/NORMAL (DISABLED: Hide the mouse and enable unlimited movement)

    // Set the mouse at the center of the screen
    //glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    glfwPollEvents();
}

void sphere::get_cursor_pos_increment()
{
    xIncrem = kc->xpos - kc->xpos0;
    yIncrem = kc->ypos0 - kc->ypos;         // Correction done so Y go upwards (not downwards)
}

void sphere::set_base_from_derived()
{
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

}

void sphere::set_derived_from_base(const camera & cam)
{
    sphere_center = cam.cam_position + cam.cam_direction * radius;

    glm::vec3 pos(
                cam.cam_position.x - sphere_center.x,
                cam.cam_position.y - sphere_center.y,
                cam.cam_position.z - sphere_center.z );     // Position referred to the sphere's center

    lat = atan(pos.z / sqrt(pos.x*pos.x + pos.y*pos.y));
    lon = atan(pos.y / pos.x);

    if(pos.x < 0)
        if(pos.y < 0) lon -= pi;
        else lon += pi;             // Cuadrants check

    set_mouse_position_and_visibility();
}


// FirstPerson class ------------------------------------------------------

firstPerson::firstPerson(glm::vec3 cam_pos, float latitude, float longitude, float FV)
    : camera(cam_pos, glm::vec3(0,0,0), glm::vec3(0,0,0), FV)
{
    lat = latitude;             // For camera rotation
    lon = longitude;

    walk_speed = 1.f;           // X units / second
    mouse_speed = 0.001f;

    two_first_calls = 2;

    set_base_from_derived();
}

firstPerson::firstPerson(const firstPerson &obj)
    : camera(obj)
{
    camera::operator=(obj);

    lon = obj.lon;
    lat = obj.lat;

    walk_speed = obj.walk_speed;
    mouse_speed = obj.mouse_speed;

    two_first_calls = obj.two_first_calls;
}

firstPerson & firstPerson::operator=(const firstPerson &obj)
{
    camera::operator=(obj);

    lon = obj.lon;
    lat = obj.lat;

    walk_speed = obj.walk_speed;
    mouse_speed = obj.mouse_speed;

    two_first_calls = obj.two_first_calls;
}

std::string firstPerson::get_type() { return "fp"; }

void firstPerson::computeMatricesFromInputs(float aspect_ratio)
{
// FP controls: Reads the keyboard and mouse and computes the Projection and View matrices. Uses GLFW_CURSOR_DISABLED

    ProjectionMatrix = glm::perspective(glm::radians(FoV), aspect_ratio, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

    // Directional keys: Move camera forward, backwards, to the right, to the left
    // Mouse movement: Orientate camera

    timer.get_delta_time();

    // Reset mouse position for next frame
    kc->setCursorPosInNextKeyUpdate(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);   // <<<<<<<< Window size will be variable with C_PRESS camera change

    // >>> Mouse movement <<<

    if(two_first_calls) { kc->xpos = WINDOW_WIDTH /2; kc->ypos = WINDOW_HEIGHT/2; --two_first_calls; }

    // Compute new orientation
    lon +=  mouse_speed * float(WINDOW_WIDTH /2 - kc->xpos);
    lat +=  mouse_speed * float(WINDOW_HEIGHT/2 - kc->ypos);
    adjust_moves_to_range_2pi(lat, lon);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    cam_direction = glm::vec3(
                                cos(lat) * cos(lon),
                                sin(lon) * cos(lat),
                                sin(lat) );

    // Right vector (y = 0)
    glm::vec3 right = glm::vec3(
                cos(lon - pi/2),
                sin(lon - pi/2),
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

void firstPerson::set_mouse_position_and_visibility() {

    // Mouse visibility
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);      // GLFW_CURSOR_DISABLED/HIDDEN/NORMAL (DISABLED: Hide the mouse and enable unlimited movement)

    // Set the mouse at the center of the screen
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    glfwPollEvents();
}

void firstPerson::set_base_from_derived()
{
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

void firstPerson::set_derived_from_base(const camera & cam)
{
    lat = atan(cam.cam_direction.z / sqrt(cam.cam_direction.x * cam.cam_direction.x + cam.cam_direction.y * cam.cam_direction.y));
    lon = atan(cam.cam_direction.y / cam.cam_direction.x);

    if(cam.cam_direction.x < 0)
        if(cam.cam_direction.y < 0) lon -= pi;
        else lon += pi;                         // Cuadrants check

    set_mouse_position_and_visibility();
    two_first_calls = 2;
}











