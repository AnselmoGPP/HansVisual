#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

#include "layer.hpp"

class selection_square
{
public:
    void send_selection_square(std::vector<layer> &layers_set, int display_height, int display_width);

private:
    bool search_lay(std::string layer_name, std::vector<layer> &layersSet, layer *&result);

    // Temporary variables
    float square[4][2][3];
};


class points_selection
{
public:
    float MinDistance = MIN_DISTANCE;                      // Selection distance: Minimum distance between the unitary pixel ray and the unitary point ray (direction vectors)
    bool strings_extraction = STRINGS_EXTRACTION;
    int pixel_step = 1;

    // >>> Points selection search. Looks for the selected points and print its data, if an array of strings was provided
    // Using gluProject()
    void send_selected_points(std::vector<layer> &layersSet, const glm::mat4 &ModelMatrix, const glm::mat4 &ViewMatrix, const glm::mat4 &ProjectionMatrix);
    // Using gluUnProjec() and ideas from http://www.3dkingdoms.com/selection.html
    void send_selected_points_2(std::vector<layer> &layersSet, const glm::mat4 &ModelMatrix, const glm::mat4 &ViewMatrix, const glm::mat4 &ProjectionMatrix);

private:
    bool search_lay(std::string layer_name, std::vector<layer> &layersSet, layer *&result);		// Send a ray from a certain pixel and set the points near to the ray true in selected_points[]
    void set_transf_matrices(const glm::mat4 &ModelMatrix, const glm::mat4 &ViewMatrix, const glm::mat4 &ProjectionMatrix);   // Fill the MVP arrays for the square selection
    void set_square_corners(int display_height);                    // Get the coordinates of the corners of the selection square
    void pass_points_to_selection_layer(layer *sel);                // Copy all the selected points to the selections layer
    bool is_inside_selection_square(float (*point)[3]);             // Send a ray for each pixel inside the selection square
    bool check_ray(float (*point)[3], float xpos, float ypos);
    void pass_strings_to_window();
    void pass_strings_to_console();

    void normalize_vec(glm::vec3 &vec);                             // Get the unitary vector
    float distance_sqr_vec(glm::vec3 &vec1, glm::vec3 &vec2);       // Square distance between 2 points

    // Temporary variables
    int x0, y0, x, y;
    int viewport[4];
    double projmatrix[16];
    double mvmatrix[16];
    std::vector<glm::vec3> selections_repo;
    std::vector<std::string> strings_repo;
};

#endif
