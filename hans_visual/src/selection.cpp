#include "selection.hpp"

selection::selection(std::vector<layer> *layers_set) :
    layersSet(layers_set), kc(keys_controller::get_instance()), MinDistance(MIN_DISTANCE)
{
    //selectionSquareID and selectionSquareColorID are not initialized here. They are initialized by plotter class
}

selection::selection(const selection &obj)
{
    layersSet = obj.layersSet;

    for(int i = 0; i < 16; ++i) projmatrix[i] = obj.projmatrix[i];
    for(int i = 0; i < 16; ++i) mvmatrix[i] = obj.mvmatrix[i];
    for(int i = 0; i < 4;  ++i) viewport[i] = obj.viewport[i];

    MinDistance = obj.MinDistance;

    selectionSquareID = obj.selectionSquareID;
    selectionSquareColorID = obj.selectionSquareColorID;
}

selection & selection::operator=(const selection &obj)
{
    layersSet = obj.layersSet;

    for(int i = 0; i < 16; ++i) projmatrix[i] = obj.projmatrix[i];
    for(int i = 0; i < 16; ++i) mvmatrix[i] = obj.mvmatrix[i];
    for(int i = 0; i < 4;  ++i) viewport[i] = obj.viewport[i];

    MinDistance = obj.MinDistance;

    selectionSquareID = obj.selectionSquareID;
    selectionSquareColorID = obj.selectionSquareColorID;

    return *this;
}

void selection::send_selection_square(int display_height, int display_width)
{
    if(kc->R_pressed)
    {
        layer *sqr;
        if(search_lay(SELECTION_SQUARE_LAYER_NAME, sqr))
        {
            // Set positions
            float x, y, x0, y0;
            x =     (kc->xpos - display_width/2)/(display_width/2);
            x0 =    (kc->sel_xpos0 - display_width/2)/(display_width/2);
            y =    -(kc->ypos - display_height/2)/(display_height/2);
            y0 =   -(kc->sel_ypos0 - display_height/2)/(display_height/2);

            square[0][0][0] = x0;
            square[0][0][1] = y0;
            square[0][0][2] = 0.;
            square[0][1][0] = x;
            square[0][1][1] = y0;
            square[0][1][2] = 0.;

            square[1][0][0] = x;
            square[1][0][1] = y0;
            square[1][0][2] = 0.;
            square[1][1][0] = x;
            square[1][1][1] = y;
            square[1][1][2] = 0.;

            square[2][0][0] = x;
            square[2][0][1] = y;
            square[2][0][2] = 0.;
            square[2][1][0] = x0;
            square[2][1][1] = y;
            square[2][1][2] = 0.;

            square[3][0][0] = x0;
            square[3][0][1] = y;
            square[3][0][2] = 0.;
            square[3][1][0] = x0;
            square[3][1][1] = y0;
            square[3][1][2] = 0.;

            sqr->send_lines(4, square, SEL_R, SEL_G, SEL_B);
            sqr->alpha_channel = SEL_A;
        }
    }

    if(kc->R_just_released)
    {
        std::cout << ". ";
        layer *sqr;
        if(search_lay(SELECTION_SQUARE_LAYER_NAME, sqr))
        {
            sqr->objs_to_print = 0;
        }
    }
}

void selection::send_selected_points(glm::mat4 *ModelMatrix, glm::mat4 *ViewMatrix, glm::mat4 *ProjectionMatrix, int display_height, int display_width)
{
    /*
    if (kc->R_just_released)
    {
        layer *sel;
        if(search_lay(SELECTED_POINTS_LAYER_NAME, sel))
        {
            // >>> Fill the MVP arrays for the square selection (projmatrix[], mvmatrix[], viewport[]):

                glGetIntegerv(GL_VIEWPORT, viewport);
                // glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);			// See: glMatrixMode, glPushMatrix, glGet
                // glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);

                glm::mat4 modelviewmatrix = (*ViewMatrix) * (*ModelMatrix);

                for (int i = 0; i < 4; i++)
                    for (int j = 0; j < 4; j++)
                    {
                        projmatrix[i * 4 + j] = (*ProjectionMatrix)[i][j];
                        mvmatrix[i * 4 + j] = modelviewmatrix[i][j];
                    }

            // >>> Forbid new buffers editions in this main-loop iteration. If any buffer is being edited now, there is a lock_guard ahead to avoid conflicts.
                for (size_t i = 0; i < layersSet->size(); i++)
                    layersSet->operator[](i).state = closed;

                // Clear the vector (size = 0, but keeps previous capacity) that will pass the selected points to the layer
                temp_selections.clear();

            // >>> Point selection search

                //check_ray(cam.sel_xpos0, cam.sel_ypos0);      // One point selection way

                // Check the direction to where you draw the selection square
                signed int xstep, ystep;
                if(kc->sel_xpos0 <= kc->xpos) xstep = 1;
                else xstep = -1;
                if(kc->sel_ypos0 <= kc->ypos) ystep = 1;
                else ystep = -1;

                // Send a ray for each pixel inside the selection square
                for(double column = kc->sel_xpos0; ; column += xstep)
                {
                    if(xstep == 1 && column > kc->xpos) break;
                    else if(xstep == -1 && column < kc->xpos) break;

                    for(double row = kc->sel_ypos0; ; row += ystep)
                    {
                        if(ystep == 1 && row > kc->ypos) break;
                        else if(ystep == -1 && row < kc->ypos) break;

                        check_ray(column, row, display_height, display_width);
                    }
                }

                // Copy all the selected points to a layer (Selections)
                (*sel) = layer(SELECTED_POINTS_LAYER_NAME, points, temp_selections.size());

                float (*pnts)[3] = new float[temp_selections.size()][3];
                for (int i = 0; i < temp_selections.size(); ++i)
                {
                    pnts[i][0] = temp_selections[i][0];
                    pnts[i][1] = temp_selections[i][1];
                    pnts[i][2] = temp_selections[i][2];
                }

                sel->send_points(temp_selections.size(), pnts, SEL_R, SEL_G, SEL_B);
                sel->alpha_channel = SEL_A;
                delete[] pnts;

                std::cout << "\n----- Selections -----" << std::endl;
                for(size_t i = 0; i < layersSet->size(); i++)
                {
                    //if(layersSet->operator[](i).layer_type == points)
                    //    for(size_t j = 0; j < layersSet->operator[](i).objs_to_print; j++)
                    //        if(layersSet->operator[](i).points_strings[j] != "") std::cout << layersSet->operator[](i).points_strings[j] << std::endl;
                }
        }
    }
    */
}

// Private members -----------------------------------------

void selection::check_ray(double xpos, double ypos, int display_height, int display_width) {

    double dX, dY, dZ;                      // Temporary variables
    double dClickY, dClickX;                // OGL screen coordinates of the pixel
    glm::vec3 ClickRayP1, ClickRayP2;       // OGL world coordinates of the pixel ray ends (in the near and far clip plane).
    glm::vec3 pointRayP1, pointRayP2;       // OGL world coordinates of the point ray ends (in the near clip plane and the point).
    glm::vec3 ClickSlope, pointSlope;       // Direction vectors: Directions (unitary vector) of the pixel ray and the point's ray
    double sqrDist, minSqrDist = MinDistance * MinDistance;

    // From the screen coordinates, get the 2 ray ends in world-space (they are in the near and far clip plane)
    dClickX = xpos;
    dClickY = double(display_height - ypos);         // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

    gluUnProject(dClickX, dClickY, 0.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);		// 0.0: Near clip plane
    ClickRayP1 = glm::vec3(dX, dY, dZ);
    gluUnProject(dClickX, dClickY, 1.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);		// 1.0: Far clip plane
    ClickRayP2 = glm::vec3(dX, dY, dZ);

    // Get the direction vector of the pixel ray
    ClickSlope = ClickRayP2 - ClickRayP1;           // Get the direction vector
    normalize_vec(ClickSlope);                      // Get unitary direction vector

    // Find the closest points by testing which points' rays are close to the clickRay (checks all the points)
    for (size_t i = 0; i < layersSet->size(); i++)
    {
        if(layersSet->operator[](i).layer_type != points || !layersSet->operator[](i).checkbox_value) continue;

        std::lock_guard<std::mutex> lock(*layersSet->operator[](i).mut);       // This second filther (after setting buffer_closed to closed) avoid conflicts with buffers being currently edited (can only happen during the first iteration of this loop for each layer)

        for (size_t j = 0; j < layersSet->operator[](i).objs_to_print; j++)
        {
            // TODO <<<<<<<<<< If this point was already selected (by a different ClickRay), don't check again whether it is selected

            // Point's ray is formed by the vector between the selectable point and the origin point of the clickRay
            pointRayP1 = ClickRayP1;
            pointRayP2 = glm::vec3(	layersSet->operator[](i).points_buffer[j][0],
                                    layersSet->operator[](i).points_buffer[j][1],
                                    layersSet->operator[](i).points_buffer[j][2] );

            // Get the direction vector of the point ray
            pointSlope = pointRayP2 - pointRayP1;   // Get the direction vector
            normalize_vec(pointSlope);          // Get unitary direction vector

            // Check distance between the ends the direction vector of the pixel ray and the point ray
            sqrDist = distance_sqr_vec(pointSlope, ClickSlope);
            if (sqrDist < minSqrDist)
                temp_selections.push_back(glm::vec3(layersSet->operator[](i).points_buffer[j][0],
                                                    layersSet->operator[](i).points_buffer[j][1],
                                                    layersSet->operator[](i).points_buffer[j][2]));
        }
    }
}

void selection::normalize_vec(glm::vec3 &vec)
{
    float magnitude = sqrt( (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) );

    vec.x /= magnitude;
    vec.y /= magnitude;
    vec.z /= magnitude;
}

double selection::distance_sqr_vec(glm::vec3 &vec1, glm::vec3 &vec2)
{
    return	(vec1.x - vec2.x) * (vec1.x - vec2.x) +
            (vec1.y - vec2.y) * (vec1.y - vec2.y) +
            (vec1.z - vec2.z) * (vec1.z - vec2.z);
}

bool selection::search_lay(std::string layer_name, layer *&result)
{
    for(size_t i = 0; i < layersSet->size(); ++i)
        if(layersSet[0][i].layer_name == layer_name)
        {
            result = &layersSet[0][i];
            return true;
        }

    std::cout << "Layer \"" << layer_name << "\" was not found" << std::endl;
    return false;
}
