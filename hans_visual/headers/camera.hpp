#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "_options.hpp"
#include "controls.hpp"

#include <string>

class glfw_timer
{
    float lastTime = 0.0;
    float currentTime;
public:
    float deltaTime;
    void get_delta_time();  // Get time increment between two consecutives calls to this function
};

//enum cam_system{fp, sphere, plane, fps, zenith, sphere_z};

class camera
{
public:
    //camera(glm::vec3 cam_pos, glm::vec3 cam_dir, glm::vec3 up_vec, float FV);
    camera(float FV = 45.0f);
    camera(glm::vec3 cam_pos, glm::vec3 cam_dir, glm::vec3 up_vec, float FV = 45.0f);
    camera(const camera &obj);
    camera & operator=(const camera &obj);
    virtual ~camera() = default;

    glm::mat4 getViewMatrix();
    glm::mat4 getProjectionMatrix();

    virtual std::string get_type() = 0;                                 // Types: "sphere", "fp"
    virtual void computeMatricesFromInputs(float aspect_ratio) = 0;
    virtual void set_mouse_position_and_visibility() = 0;
    virtual void set_base_from_derived() = 0;
    virtual void set_derived_from_base(const camera & cam) = 0;

    //void hide_cursor(bool hide);
    void associate_window(GLFWwindow *window_in);

    glm::vec3 cam_position;
    glm::vec3 cam_direction;
    glm::vec3 up_vector;
    float FoV;

protected:
    GLFWwindow *window;
    keys_controller * kc;

    glm::mat4 ViewMatrix;
    glm::mat4 ProjectionMatrix;

    glfw_timer timer;
    float pi;

    void adjust_moves_to_range_2pi(float &lat, float &lon);
};

// Do have standard initial values for exclusive parameters. When changing camera mode, just change the common parameters (position, orientation)

class sphere : public camera
{
public:
    sphere(glm::vec3 center, float rad, float min_rad, float latitude, float longitude, float FV = 45.0f);
    //sphere(const sphere &obj);
    sphere(const sphere &obj);
    sphere & operator=(const sphere &obj);
    ~sphere() override = default;

    std::string get_type() override;
    void computeMatricesFromInputs(float aspect_ratio) override;
    void set_mouse_position_and_visibility() override;
    void set_base_from_derived() override;
    void set_derived_from_base(const camera & cam) override;
    void get_cursor_pos_increment();

    glm::vec3 sphere_center;
    float radius;
    float minimum_radius;
    float lat, lon;

    float mouse_speed;
    float scroll_speed;
    float right_speed;

    float xIncrem, yIncrem;
};

class firstPerson : public camera
{
public:
    firstPerson(glm::vec3 cam_pos, float latitude, float longitude, float FV = 45.0f);
    firstPerson(const firstPerson &obj);
    firstPerson & operator=(const firstPerson &obj);
    ~firstPerson() override = default;

    std::string get_type() override;
    void computeMatricesFromInputs(float aspect_ratio) override;
    void set_mouse_position_and_visibility() override;
    void set_base_from_derived() override;
    void set_derived_from_base(const camera & cam) override;

    float lon;
    float lat;
    float walk_speed;                     // X units / second
    float mouse_speed;
    int two_first_calls;
};

class cam_factory
{
public:
    cam_factory();
    cam_factory(const cam_factory &obj);
    cam_factory &operator=(const cam_factory &obj);
    ~cam_factory();

    camera *cam;
    void camera_change();
};

#endif
