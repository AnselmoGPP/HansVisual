#include "camera.hpp"

//camera *camHandler;		// Pointer used by the callback functions (mouse buttons).
//std::mutex cam_mut;

camera::camera(int mode) : window(nullptr), kc(keys_controller::get_instance())
{
    // First person
    if (mode == 1)
    {
        camera_mode = fp;
        position = glm::vec3(0, 15, 0);     // Initial position : on +Z use (0, 0, 5)
		horizontalAngle = 3.14f;			// Initial horizontal angle : toward -Z use 3.14f
		verticalAngle = -3.14f / 2;			// Initial vertical angle : for none use 0
		initialFoV = 45.0f;					// Initial Field of View
        speed = 15.0f;						// 3 units / second
		mouseSpeed = 0.001f;
	}
    // Sphere
    else if (mode == 2)
    {
        camera_mode = sphere;
		//position = glm::vec3( 0, 15, 0 );
        horizontalAngle = 3.14f / 2;        // Initial pos
        verticalAngle = 3.14f / 2;          // Initial pos
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

// FPS controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_DISABLED
void camera::computeMatricesFromInputs_FP(float aspect_ratio)
{
	// glfwGetTime is called only once, the first time this function is called
	if (!lastTime) lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	// Compute new orientation
    horizontalAngle += mouseSpeed * float(WINDOW_WIDTH / 2 - kc->xpos);
    verticalAngle += mouseSpeed * float(WINDOW_HEIGHT / 2 - kc->ypos);

	// >>> Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

    // >>> Right vector (y = 0)
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// >>> Up vector
	glm::vec3 up = glm::cross(right, direction);

    // Movement
    if (kc->up_press || kc->w_press)    position += direction * deltaTime * speed;
    if (kc->down_press || kc->s_press)  position -= direction * deltaTime * speed;
    if (kc->right_press || kc->d_press) position += right * deltaTime * speed;
    if (kc->left_press || kc->a_press)  position -= right * deltaTime * speed;

	// >>> Camera matrix
	ViewMatrix = glm::lookAt(
        position,               // Camera is here
        position + direction,   // and looks here : at the same position, plus "direction"
        up                      // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
    lastTime = currentTime;
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

    // >>>>> Left mouse button <<<<<
    // Get the cursor position only when right or left mouse buttons are pressed
    ///if (L_pressed) glfwGetCursorPos(window, &xpos, &ypos);

    double pi = 3.1415926536;

	// Compute angular movement over the sphere
    if(kc->L_pressed)
    {
        horizontalAngle -= mouseSpeed * float(kc->xpos - kc->xpos0);
        verticalAngle -= mouseSpeed * float(kc->ypos - kc->ypos0);

        if      (horizontalAngle > 2*pi)  horizontalAngle -= 2*pi;
        else if (horizontalAngle < 0) horizontalAngle += 2*pi;
        if      (verticalAngle > 2*pi)    verticalAngle -= 2*pi;
        else if (verticalAngle < 0)   verticalAngle += 2*pi;

        //if(verticalAngle > 3.1416/2 && verticalAngle < 3.1416+(3/2)) horizontalAngle = - horizontalAngle;
        std::cout << horizontalAngle << ", " << verticalAngle << '\r' << std::flush;
    }


    // Set sphere radius
    radius -= kc->scroll_yOffset * scroll_speed;
    if (radius < minimum_radius) radius = minimum_radius;
    //std::cout << "OUT: " << keys_controller::get_instance()->scroll_yOffset << std::endl;

    // Get the position over the sphere
    position = sphere_center + glm::vec3(
                radius * cos(verticalAngle) * cos(horizontalAngle),
                radius * cos(verticalAngle) * sin(horizontalAngle),
                radius * sin(verticalAngle) );
    /*
    if(verticalAngle >= pi/2 && verticalAngle <= (3./2)*pi)
    {
        position = sphere_center + glm::vec3(
                    radius * cos(verticalAngle) * cos(horizontalAngle),
                    radius * cos(verticalAngle) * sin(horizontalAngle),
                    radius * sin(verticalAngle) );
    }

    if(verticalAngle < pi/2 || verticalAngle > (3./2)*pi)
    {
        position = sphere_center + glm::vec3(
                    radius * cos(-verticalAngle) * cos(-horizontalAngle),
                    radius * cos(-verticalAngle) * sin(-horizontalAngle),
                    radius * sin(verticalAngle) );
    }

    if(verticalAngle > (3./2)*pi)
    {

    }
*/

    // Direction towards the sphere's center
    glm::vec3 direction = (sphere_center - position) / radius;

    // Right vector (vector director) (z=0)
    glm::vec3 right = glm::vec3(
                cos(horizontalAngle - 3.14f / 2.0f),
                sin(horizontalAngle - 3.14f / 2.0f),
                0 );

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // >>>>> Right mouse button <<<<<
    //if (R_pressed) {
		// Front vector (y=0) (used for moving the sphere center)
        //glm::vec3 front = glm::vec3(    sin(horizontalAngle + 3.14f),
        //                                0,
        //                                cos(horizontalAngle + 3.14f) );

        //sphere_center += right_speed * radius * ( (-right * float(xpos - xpos0)) + (front * float(-ypos + ypos0)) );
    //}

    // >>>>> Scroll is pressed <<<<<

    if (kc->scroll_pressed)
	{
        //std::cout << kc->xpos - kc->xpos0 << ", " << kc->ypos - kc->ypos0 << std::endl;
        sphere_center += right_speed * radius * ((-right * float(kc->xpos - kc->xpos0)) + (up * float(kc->ypos - kc->ypos0)));
	}

	// >>> View matrix
		ViewMatrix = glm::lookAt(position, sphere_center, up);
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
    glfwPollEvents();
    glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);
}

void camera::hide_cursor(bool hide)
{
    // GLFW_CURSOR_DISABLED, GLFW_CURSOR_HIDDEN, GLFW_CURSOR_NORMAL
    if(hide) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);     //GLFW_CURSOR_DISABLED: Hide the mouse and enable unlimited movement
    else     glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

