#include "camera.hpp"
#include <cmath>

void controls_timer::get_delta_time()
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

    // First person
    if (camera_mode == fp)
    {
        cam_position = glm::vec3(0, 15, 0); // Initial position : on +Z use (0, 0, 5)
        horMove = 3.14f;			// Initial horizontal angle : toward -Z use 3.14f
        vertMove = -3.14f / 2;			// Initial vertical angle : for none use 0
		initialFoV = 45.0f;					// Initial Field of View
        speed = 15.0f;						// 3 units / second
        mouseSpeed = 0.001f;
	}
    // Sphere
    else if (camera_mode == sphere)
    {
		//position = glm::vec3( 0, 15, 0 );
        horMove = 3.14f / 2;        // Initial pos
        vertMove = 3.14f / 2;          // Initial pos
		initialFoV = 45.0f;
		speed = 5.f;
		mouseSpeed = 0.008f;

		sphere_center = glm::vec3(0, 0, 0);
		radius = 15;
        scroll_speed = 1;
		minimum_radius = 1;
        right_speed = 0.001;
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
//#include <thread>
// FP controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_DISABLED
void camera::computeMatricesFromInputs_FP(float aspect_ratio)
{
    timer.get_delta_time();

    if(kc->L_pressed) get_cursor_displacement();

    // Reset mouse position for next frame
    kc->setCursorPosInNextKeyUpdate(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);   // <<<<<<<< Window size will be variable with C_PRESS camera change

	// Compute new orientation
    horMove +=  mouseSpeed * float(WINDOW_WIDTH / 2 - kc->xpos);
    vertMove +=    mouseSpeed * float(WINDOW_HEIGHT / 2 - kc->ypos);

	// >>> Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
        cos(vertMove) * sin(horMove),  // <<<<<<<<<<<<<<<<<
        sin(vertMove),
        cos(vertMove) * cos(horMove)
	);

    // >>> Right vector (y = 0)
	glm::vec3 right = glm::vec3(
        sin(horMove - 3.14f / 2.0f),
        0,
        cos(horMove - 3.14f / 2.0f)
	);

	// >>> Up vector
	glm::vec3 up = glm::cross(right, direction);

    // Movement
    if (kc->up_press    || kc->w_press) cam_position += direction   * timer.deltaTime * speed;
    if (kc->down_press  || kc->s_press) cam_position -= direction   * timer.deltaTime * speed;
    if (kc->right_press || kc->d_press) cam_position += right       * timer.deltaTime * speed;
    if (kc->left_press  || kc->a_press) cam_position -= right       * timer.deltaTime * speed;

	// >>> Camera matrix
	ViewMatrix = glm::lookAt(
        cam_position,               // Camera is here
        cam_position + direction,   // and looks here : at the same position, plus "direction"
        up                          // Head is up (set to 0,-1,0 to look upside-down)
	);
}

// Spherical controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_NORMAL
void camera::computeMatricesFromInputs_spherical(float aspect_ratio) {
	/*
		Left click: Rotate around a sphere
        Scroll roll: Get closer or further
		Scroll click: Normal translation
	*/

	// Get current time and time difference
	//currentTime = glfwGetTime();
	//deltaTime = float(currentTime - lastTime);

    // >>> Scroll roll <<<

    // Set sphere radius
    if(ENABLE_SCROLL_SPEEDUP)   radius -= kc->scroll_yOffset * scroll_speed * (SCROLL_SPEEDUP * radius);
    else                        radius -= kc->scroll_yOffset * scroll_speed;

    if (radius < minimum_radius) radius = minimum_radius;

    // >>> Left mouse button <<<

    //Get angular position over the sphere (latitude & longitude)
    if(kc->L_pressed) get_cursor_displacement();

    // Get the position over the sphere
    cam_position = sphere_center + glm::vec3(
                radius * cos(vertMove) * cos(horMove),
                radius * cos(vertMove) * sin(horMove),
                radius * sin(vertMove) );

    // Vector director: Direction towards the sphere's center
    glm::vec3 direction = (sphere_center - cam_position) / radius;

    // Right vector (z=0)
    glm::vec3 right = glm::vec3(
                cos(horMove - 3.14f / 2.0f),
                sin(horMove - 3.14f / 2.0f),
                0 );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // >>> Scroll pressed <<<

    if (kc->scroll_pressed)
    {
        sphere_center += right_speed * radius * ((-right * float(kc->xpos - kc->xpos0)) + (up * float(kc->ypos - kc->ypos0)));
    }

    // >>> View matrix <<<

    ViewMatrix = glm::lookAt(cam_position, sphere_center, up);
}

void camera::get_cursor_displacement()
{
    //std::cout << "H: " << horizontalAngle << ", V: " << verticalAngle << '\r' << std::flush;

    // Get angular position
    vertMove   -= mouseSpeed * float(kc->ypos - kc->ypos0);
    horMove -= mouseSpeed * float(kc->xpos - kc->xpos0);
    //std::cout << kc->ypos - kc->ypos0 << ", " << kc->xpos - kc->xpos0 << std::endl;

    // Reformat it so the range is (0, 2*pi]
    if      (horMove > 2*pi)    horMove -= 2*pi;
    else if (horMove <= 0)      horMove += 2*pi;
    if      (vertMove > 2*pi)   vertMove -= 2*pi;
    else if (vertMove <= 0)     vertMove += 2*pi;

    // Reverse horizontal control when camera is inverted (so, control when right and when reversed is the same)
    if((vertMove < pi/2 && vertMove >= 0) ||
       (vertMove > 3*pi/2 && vertMove <= 2*pi))
    {
        horMove += 2*mouseSpeed * float(kc->xpos - kc->xpos0);
        if      (horMove > 2*pi)    horMove -= 2*pi;
        else if (horMove <= 0)      horMove += 2*pi;
    }
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

