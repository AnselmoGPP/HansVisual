#include "camera.hpp"
#include <cmath>

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

camera::camera(cam_system mode) : window(nullptr), kc(keys_controller::get_instance())
{
    camera_mode = mode;

    initialFoV = 45.0f;					// Initial Field of View

    // First person
    if (camera_mode == fp)
    {
        // up_vec
        // cam_direction
        cam_position = glm::vec3(0, 15, 0); // Initial position : on +Z use (0, 0, 5)
        horMove = 3.14f;                    // Initial horizontal angle : toward -Z use 3.14f
        vertMove = -3.14f / 2;              // Initial vertical angle : for none use 0
        speed = 15.0f;						// 3 units / second
        mouseSpeed = 0.001f;
	}
    // Sphere
    else if (camera_mode == sphere)
    {
        horMove = 3.14f / 2;                // Initial pos      <<<<<<<<<<<<<<<< Test and take notes about how orientation works
        vertMove = 3.14f / 2;               // Initial pos
		speed = 5.f;
		mouseSpeed = 0.008f;

		sphere_center = glm::vec3(0, 0, 0);
		radius = 15;
        scroll_speed = 1;
		minimum_radius = 1;
        right_speed = 0.001f;
    }
}

glm::mat4 camera::getViewMatrix() { return ViewMatrix; }
glm::mat4 camera::getProjectionMatrix() { return ProjectionMatrix; }

void camera::computeMatricesFromInputs(float aspect_ratio)
{
    ProjectionMatrix = glm::perspective(glm::radians(initialFoV), aspect_ratio, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);

    if       (camera_mode == fp) computeMatricesFromInputs_FP(aspect_ratio);
    else if  (camera_mode == sphere) computeMatricesFromInputs_spherical(aspect_ratio);
}

// FP controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_DISABLED
void camera::computeMatricesFromInputs_FP(float aspect_ratio)
{
    // Directional keys: Move camera forward, backwards, to the right, to the left
    // Mouse movement: Orientate camera

    timer.get_delta_time();

    // Reset mouse position for next frame
    kc->setCursorPosInNextKeyUpdate(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);   // <<<<<<<< Window size will be variable with C_PRESS camera change

    // >>> Directional keys <<<

	// Compute new orientation
    horMove     +=  mouseSpeed * float(WINDOW_WIDTH  / 2 - kc->xpos);
    vertMove    +=  mouseSpeed * float(WINDOW_HEIGHT / 2 - kc->ypos);

    adjust_moves_to_range_2pi();

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    cam_direction = glm::vec3(
                                cos(vertMove) * cos(horMove),
                                sin(horMove) * cos(vertMove),
                                sin(vertMove) );

    // Right vector (y = 0)
	glm::vec3 right = glm::vec3(
                cos(horMove - 3.14f / 2.0f),
                sin(horMove - 3.14f / 2.0f),
                0 );

    // Up vector
    up_vector = glm::cross(right, cam_direction);

    // >>> Movement <<<

    if (kc->up_press    || kc->w_press) cam_position += cam_direction   * timer.deltaTime * speed;
    if (kc->down_press  || kc->s_press) cam_position -= cam_direction   * timer.deltaTime * speed;
    if (kc->right_press || kc->d_press) cam_position += right           * timer.deltaTime * speed;
    if (kc->left_press  || kc->a_press) cam_position -= right           * timer.deltaTime * speed;

    // >>> Camera matrix <<<

    ViewMatrix = glm::lookAt( cam_position, cam_position + cam_direction, up_vector );                    // Head is up (set to 0,-1,0 to look upside-down)
}

// Spherical controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_NORMAL
void camera::computeMatricesFromInputs_spherical(float aspect_ratio)
{
    // Left click: Rotate around a sphere
    // Scroll roll: Get closer or further
    // Scroll click: Normal translation

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
        if(vertMove > (pi/2.) && vertMove < (3*pi/2.))      // Range (pi/2, 3pi/2)
        {
            horMove     -= mouseSpeed * xIncrem;
            vertMove    += mouseSpeed * yIncrem;
        }
        else
        {
            horMove     += mouseSpeed * xIncrem;
            vertMove    += mouseSpeed * yIncrem;
        }

        adjust_moves_to_range_2pi();
    }

    // Get the position over the sphere   
        cam_position = sphere_center + glm::vec3(
                     radius * cos(vertMove) * cos(horMove),
                     radius * cos(vertMove) * sin(horMove),
                     radius * sin(vertMove) );

    // Vector director: Direction towards the sphere's center
    cam_direction = (sphere_center - cam_position) / radius;

    // Right vector (z=0)
    glm::vec3 right = glm::vec3(
                cos(horMove - 3.14f / 2.0f),
                sin(horMove - 3.14f / 2.0f),
                0 );

    // Up vector
    up_vector = glm::cross(right, cam_direction);

    // >>> View matrix <<<

    ViewMatrix = glm::lookAt(cam_position, sphere_center, up_vector);

    // >>> Scroll pressed <<<

    if (kc->scroll_pressed)
        sphere_center += right_speed * radius * ((-right * xIncrem) + (-up_vector * yIncrem));
}

void camera::get_cursor_pos_increment()
{
    xIncrem = kc->xpos - kc->xpos0;
    yIncrem = kc->ypos0 - kc->ypos;         // Correction done so Y go upwards (not downwards)
}

void camera::adjust_moves_to_range_2pi()
{
    if(horMove >= 2*pi) horMove -= 2*pi;
    else if(horMove < 0) horMove += 2*pi;

    if(vertMove >= 2*pi) vertMove -= 2*pi;
    else if(vertMove < 0) vertMove += 2*pi;
}

void camera::associate_window(GLFWwindow *window_in) { window = window_in; }

void camera::sticky_keys(bool sticky)
{
    if(sticky)
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);        // Sticky keys: Make sure that any pressed key is captured (such as the escape)
    else
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_FALSE);
}

void camera::set_mouse_position_visibility() {

	// Mouse visibility
    if  (camera_mode == fp)
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// GLFW_CURSOR_DISABLED: Hide the mouse and enable unlimited movement
    else if
        (camera_mode == sphere)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);      // GLFW_CURSOR_DISABLED/HIDDEN/NORMAL

    // Set the mouse at the center of the screen
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
    glfwPollEvents();
}

void camera::hide_cursor(bool hide)
{
    // GLFW_CURSOR_DISABLED, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_NORMAL
    if(hide) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);     //GLFW_CURSOR_DISABLED: Hide the mouse and enable unlimited movement
    else     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

