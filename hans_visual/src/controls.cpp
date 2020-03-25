#include "controls.hpp"

controls *camera;		// Pointer used by the callback functions (mouse buttons).
std::mutex cam_mut;

controls::controls(int mode) : camera_mode(mode) {

	if (camera_mode == FPS) {
        position = glm::vec3(0, 15, 0);     // Initial position : on +Z use (0, 0, 5)
		horizontalAngle = 3.14f;			// Initial horizontal angle : toward -Z use 3.14f
		verticalAngle = -3.14f / 2;			// Initial vertical angle : for none use 0
		initialFoV = 45.0f;					// Initial Field of View
		speed = 5.0f;						// 3 units / second
		mouseSpeed = 0.001f;
	}
	else if (camera_mode == SPHERE) {
		//position = glm::vec3( 0, 15, 0 );
		horizontalAngle = 3.14f;
		verticalAngle = 3.14f / 2;
		initialFoV = 45.0f;
		speed = 5.f;
		mouseSpeed = 0.008f;

		sphere_center = glm::vec3(0, 0, 0);
		radius = 15;
        L_pressed = false;  R_pressed = false;  scroll_pressed = false;
		scroll_speed = 1;
		minimum_radius = 1;
        right_speed = 0.001;
    }
}

glm::mat4 controls::getViewMatrix() { return ViewMatrix; }
glm::mat4 controls::getProjectionMatrix() { return ProjectionMatrix; }

void controls::computeMatricesFromInputs(GLFWwindow* window) {
    if      (camera_mode == FPS) computeMatricesFromInputs_FPS(window);
	else
        if  (camera_mode == SPHERE) computeMatricesFromInputs_spherical(window);
}

// FPS controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_DISABLED
void controls::computeMatricesFromInputs_FPS(GLFWwindow* window) {

	// glfwGetTime is called only once, the first time this function is called
	if (!lastTime) lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	currentTime = glfwGetTime();
	deltaTime = float(currentTime - lastTime);

	// Get mouse position
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(WINDOW_WIDTH / 2 - xpos);
	verticalAngle += mouseSpeed * float(WINDOW_HEIGHT / 2 - ypos);

	// >>> Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle),
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);

    // >>> Right vector (right from center, taking z as X, and x as Y) (y = 0)
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f / 2.0f),
		0,
		cos(horizontalAngle - 3.14f / 2.0f)
	);

	// >>> Up vector
	glm::vec3 up = glm::cross(right, direction);

	// Move forward
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated, so here it's disabled instead.

	// >>> Projection matrix : 45° Field of View, 4:3 ratio, Display range (near/far clipping plane) : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), ASPECT_RATIO, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);
	// >>> Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		position + direction, // and looks here : at the same position, plus "direction"
		up                  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

// Spherical controls - Reads the keyboard and mouse and computes the Projection and View matrices. Use GLFW_CURSOR_NORMAL
void controls::computeMatricesFromInputs_spherical(GLFWwindow* window) {
	/*
		Left click: Rotate around a sphere
		Scroll: Get closer or further
		Scroll click: Normal translation
	*/

    // >>> Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	float FoV = initialFoV;    //float FoV = - 5 * glfwGetMouseWheel();
	ProjectionMatrix = glm::perspective(glm::radians(FoV), ASPECT_RATIO, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);


	// Get current time and time difference
	//currentTime = glfwGetTime();
	//deltaTime = float(currentTime - lastTime);

	// Detect pressed and released button actions
	glfwSetMouseButtonCallback(window, mouseButtonCallback);        // The callback function is run only when the mouse left/middle/right-button is pressed or released

    // Get the cursor position only when right or left mouse buttons are pressed
    if (L_pressed)
    {
		glfwGetCursorPos(window, &xpos, &ypos);		// Another option using callback:	glfwSetCursorPosCallback(window, cursorPositionCallback);
	}

	// Compute angular movement over the sphere
    horizontalAngle -= mouseSpeed * float(xpos - xpos0);
    verticalAngle -= mouseSpeed * float(ypos - ypos0);

	glfwSetScrollCallback(window, scrollCallback);

	// Get the position over the sphere
	position = sphere_center + glm::vec3(radius * cos(verticalAngle) * sin(horizontalAngle),
		radius * sin(verticalAngle),
		radius * cos(verticalAngle) * cos(horizontalAngle));

	// Direction towards the center of the sphere
	glm::vec3 direction = (sphere_center - position) / radius;

    // Right vector (right from center, taking z as X, and x as Y) (y=0) (used for calculating the Up vector)
	glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f),
                                0,
                                cos(horizontalAngle - 3.14f / 2.0f));

	// Up vector
	glm::vec3 up = glm::cross(right, direction);

	if (R_pressed)
    {
        glfwGetCursorPos(window, &sel_xpos, &sel_ypos);

		// Front vector (y=0) (used for moving the sphere center)
        //glm::vec3 front = glm::vec3(    sin(horizontalAngle + 3.14f),
        //                                0,
        //                                cos(horizontalAngle + 3.14f) );

        //sphere_center += right_speed * radius * ( (-right * float(xpos - xpos0)) + (front * float(-ypos + ypos0)) );
	}

	if (scroll_pressed) 
	{
		glfwGetCursorPos(window, &xpos, &ypos);

		sphere_center += right_speed * radius * ((-right * float(xpos - xpos0)) + (up * float(ypos - ypos0)));
	}

	// >>> View matrix
		ViewMatrix = glm::lookAt(position, sphere_center, up);

	//lastTime = currentTime;
	xpos0 = xpos;
    ypos0 = ypos;
}

void controls::adjustments(GLFWwindow *window) {

	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);			// Ensure we can capture the escape key being pressed below

	if (camera_mode == FPS)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);	// Hide the mouse and enable unlimited movement. Use GLFW_CURSOR_DISABLED/HIDDEN/NORMAL.
	else
	if (camera_mode == SPHERE)
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
}

void controls::normalize_vec(glm::vec3 &vec) {

	float magnitude = sqrt( (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) );

	vec.x /= magnitude;
	vec.y /= magnitude;
	vec.z /= magnitude;
}

double controls::distance_sqr_vec(glm::vec3 &vec1, glm::vec3 &vec2) {

	return	(vec1.x - vec2.x) * (vec1.x - vec2.x) +
			(vec1.y - vec2.y) * (vec1.y - vec2.y) +
			(vec1.z - vec2.z) * (vec1.z - vec2.z);
}

// Callback functions ----------------------------

void controls::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {

	if		(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
        camera->L_pressed = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
        camera->L_pressed = true;
        glfwGetCursorPos(window, &camera->xpos0, &camera->ypos0);
        camera->ypos = camera->ypos0;
        camera->xpos = camera->xpos0;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
        camera->R_pressed = false;

        camera->R_just_released = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
        camera->R_pressed = true;
        glfwGetCursorPos(window, &camera->sel_xpos0, &camera->sel_ypos0);
        camera->sel_ypos = camera->sel_ypos0;
        camera->sel_xpos = camera->sel_xpos0;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
	{
        camera->scroll_pressed = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
        camera->scroll_pressed = true;
        glfwGetCursorPos(window, &camera->xpos0, &camera->ypos0);
        camera->ypos = camera->ypos0;
        camera->xpos = camera->xpos0;
	}
}

void controls::scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    camera->radius -= yOffset * camera->scroll_speed;
    if (camera->radius < camera->minimum_radius) camera->radius = camera->minimum_radius;
}
