#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <GL/glew.h>

#include "layer.hpp"
#include "camera.hpp"
#include "controls.hpp"

// Struct used for point selection
class selection
{
    std::vector<layer> *layersSet;
    keys_controller *kc;
    float square[4][2][3];

    void check_ray(double xpos, double ypos, int display_height, int display_width);		// Send a ray from a certain pixel and set the points near to the ray true in selected_points[]

    // MVP matrices of the points to select
    double projmatrix[16];
    double mvmatrix[16];
    int viewport[4];

    double MinDistance;                      // Selection distance: Minimum distance between the unitary pixel ray and the unitary point ray (direction vectors)
    std::vector<glm::vec3> temp_selections;

    void normalize_vec(glm::vec3 &vec);                             // Get the unitary vector
    double distance_sqr_vec(glm::vec3 &vec1, glm::vec3 &vec2);      // Square distance between 2 points

    bool search_lay(std::string layer_name, layer *&result);

public:
    selection() = default;
    selection(std::vector<layer> *layers_set);
    selection(const selection &obj);
    selection & operator=(const selection &obj);

    //glm::mat4 *ModelMatrix, *ViewMatrix, *ProjectionMatrix;
    unsigned int *selectionSquareID, *selectionSquareColorID;

    void send_selection_square(int display_height, int display_width);
    void send_selected_points(glm::mat4 *model_mat, glm::mat4 *view_mat, glm::mat4 *proj_mat, int display_height, int display_width);    // Points selection search ( http://www.3dkingdoms.com/selection.html ). Looks for the selected points and print its data, if an array of strings was provided
};

#endif
