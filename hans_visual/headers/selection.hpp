#ifndef SELECTION_HPP
#define SELECTION_HPP

#include <GL/glew.h>

#include "layer.hpp"
#include "camera.hpp"

// Struct used for point selection
class selection
{
    std::vector<layer> *layersSet;
    //camera *cam;
    int display_h;

    void check_ray(double xpos, double ypos);		// Send a ray from a certain pixel and set the points near to the ray true in selected_points[]

    // MVP matrices of the points to select
    double projmatrix[16];
    double mvmatrix[16];
    int viewport[4];

    double MinDistance;                      // Selection distance: Minimum distance between the unitary pixel ray and the unitary point ray (direction vectors)
    std::vector<glm::vec3> temp_selections;

public:
    selection() = default;
    selection(std::vector<layer> *layers_set, int display_hor);     // <<<<< Where is display_vert??
    selection(const selection &obj);
    selection & operator=(const selection &obj);

    //glm::mat4 *ModelMatrix, *ViewMatrix, *ProjectionMatrix;
    unsigned int *selectionSquareID, *selectionSquareColorID;

    void draw_selection_square();
    void check_selections(glm::mat4 *model_mat, glm::mat4 *view_mat, glm::mat4 *proj_mat);    // Points selection search ( http://www.3dkingdoms.com/selection.html ). Looks for the selected points and print its data, if an array of strings was provided

    float selection_color[4];
    layer selection_square;     // <<<<<<<<<<<<<<<
};

#endif
