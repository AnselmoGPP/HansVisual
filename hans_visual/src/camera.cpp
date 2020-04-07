#include "camera.hpp"

camera *camHandler;		// Pointer used by the callback functions (mouse buttons).
std::mutex cam_mut;

camera::camera(int mode) : window(nullptr) {

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
    else if (mode == 2)
    {
        camera_mode = sphere;
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

glm::mat4 camera::getViewMatrix() { return ViewMatrix; }
glm::mat4 camera::getProjectionMatrix() { return ProjectionMatrix; }

void camera::computeMatricesFromInputs(float aspect_ratio)
{
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
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		position += direction * deltaTime * speed;
	}
	// Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		position -= direction * deltaTime * speed;
	}
	// Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		position += right * deltaTime * speed;
	}
	// Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated, so here it's disabled instead.

	// >>> Projection matrix : 45° Field of View, 4:3 ratio, Display range (near/far clipping plane) : 0.1 unit <-> 100 units
    ProjectionMatrix = glm::perspective(glm::radians(FoV), aspect_ratio, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);
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
		Scroll: Get closer or further
		Scroll click: Normal translation
	*/

    // >>> Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	float FoV = initialFoV;    //float FoV = - 5 * glfwGetMouseWheel();
    ProjectionMatrix = glm::perspective(glm::radians(FoV), aspect_ratio, NEAR_CLIP_PLANE, FAR_CLIP_PLANE);


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

void camera::normalize_vec(glm::vec3 &vec) {

	float magnitude = sqrt( (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) );

	vec.x /= magnitude;
	vec.y /= magnitude;
	vec.z /= magnitude;
}

double camera::distance_sqr_vec(glm::vec3 &vec1, glm::vec3 &vec2) {

	return	(vec1.x - vec2.x) * (vec1.x - vec2.x) +
			(vec1.y - vec2.y) * (vec1.y - vec2.y) +
			(vec1.z - vec2.z) * (vec1.z - vec2.z);
}

// Callback functions ----------------------------

void camera::mouseButtonCallback(GLFWwindow *window, int button, int action, int mods) {

	if		(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
        camHandler->L_pressed = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
        camHandler->L_pressed = true;
        glfwGetCursorPos(window, &camHandler->xpos0, &camHandler->ypos0);
        camHandler->ypos = camHandler->ypos0;
        camHandler->xpos = camHandler->xpos0;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
	{
        camHandler->R_pressed = false;

        camHandler->R_just_released = true;
	}
	else if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
        camHandler->R_pressed = true;
        glfwGetCursorPos(window, &camHandler->sel_xpos0, &camHandler->sel_ypos0);
        camHandler->sel_ypos = camHandler->sel_ypos0;
        camHandler->sel_xpos = camHandler->sel_xpos0;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE)
	{
        camHandler->scroll_pressed = false;
	}
	else if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
	{
        camHandler->scroll_pressed = true;
        glfwGetCursorPos(window, &camHandler->xpos0, &camHandler->ypos0);
        camHandler->ypos = camHandler->ypos0;
        camHandler->xpos = camHandler->xpos0;
	}
}

void camera::scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    camHandler->radius -= yOffset * camHandler->scroll_speed;
    if (camHandler->radius < camHandler->minimum_radius) camHandler->radius = camHandler->minimum_radius;
}
