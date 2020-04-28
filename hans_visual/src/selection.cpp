#include "selection.hpp"

// selection_square class ----------------------------------

bool selection_square::search_lay(std::string layer_name, std::vector<layer> &layers_set, layer *&result)
{
    for(size_t i = 0; i < layers_set.size(); ++i)
        if(layers_set[i].layer_name == layer_name)
        {
            result = &layers_set[i];
            return true;
        }

    std::cout << "Layer \"" << layer_name << "\" was not found" << std::endl;
    return false;
}

void selection_square::send_selection_square(std::vector<layer> &layers_set, int display_height, int display_width)
{
    keys_controller *kc = keys_controller::get_instance();

    if(kc->R_pressed)
    {
        layer *sqr;
        if(search_lay(SELECTION_SQUARE_LAYER_NAME, layers_set, sqr))
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
        layer *sqr;
        if(search_lay(SELECTION_SQUARE_LAYER_NAME, layers_set, sqr))
        {
            sqr->objs_to_print = 0;
        }
    }
}

// points_selection class ----------------------------------

void points_selection::send_selected_points(std::vector<layer> &layersSet, const glm::mat4 &ModelMatrix, const glm::mat4 &ViewMatrix, const glm::mat4 &ProjectionMatrix)
{
    keys_controller *kc = keys_controller::get_instance();

    if (kc->R_just_released)
    {
        double winX, winY, winZ;
        layer *sel;
        if(search_lay(SELECTED_POINTS_LAYER_NAME, layersSet, sel))
        {
            // Fill the MVP arrays and get the Viewport dimensions
            set_transf_matrices(ModelMatrix, ViewMatrix, ProjectionMatrix);
            glGetIntegerv(GL_VIEWPORT, viewport);

            // First filther. Forbid new buffers editions in this main-loop iteration. If any buffer is being edited now, there is a lock_guard ahead to avoid conflicts.
            for (size_t i = 0; i < layersSet.size(); i++)
                layersSet.operator[](i).state = closed;

            // Clear the temporary vectors (size = 0, but keeps previous capacity)
            selections_repo.clear();
            if(strings_extraction) strings_repo.clear();

            set_square_corners(viewport[3]);

            // For every vertex, check whether it is behind any pixel inside the selection square
            for(int i = 0; i < layersSet.size(); ++i)
                if( layersSet[i].layer_type == points &&
                    layersSet[i].checkbox_value &&
                    layersSet[i].layer_name != SELECTED_POINTS_LAYER_NAME)
                {
                    std::lock_guard<std::mutex> lock(*layersSet[i].mut);       // Second filther (after setting buffer_closed to closed). It avoids conflicts with buffers being currently edited (can only happen during the first iteration of this loop for each layer)

                    for(int j = 0; j < layersSet[i].objs_to_print; ++j)
                    {
                        // https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/gluProject.xml
                        if( gluProject( layersSet[i].points_buffer[j][0],
                                        layersSet[i].points_buffer[j][1],
                                        layersSet[i].points_buffer[j][2],
                                        mvmatrix,
                                        projmatrix,
                                        viewport,
                                        &winX,
                                        &winY,
                                        &winZ))
                        {
                            if(x0 < winX && winX < x && y0 < winY && winY < y)
                            {
                                selections_repo.push_back(glm::vec3(
                                        layersSet[i].points_buffer[j][0],
                                        layersSet[i].points_buffer[j][1],
                                        layersSet[i].points_buffer[j][2]));

                                if(strings_extraction)
                                    strings_repo.push_back(layersSet[i].points_strings[j]);
                            }
                        }

                        /*
                        if(is_inside_selection_square(&layersSet[i].points_buffer[j]))
                        {
                            selections_repo.push_back(glm::vec3(
                                    layersSet[i].points_buffer[j][0],
                                    layersSet[i].points_buffer[j][1],
                                    layersSet[i].points_buffer[j][2]));

                            if(strings_extraction)
                                strings_repo.push_back(layersSet[i].points_strings[j]);
                        }
                        */
                    }
                }

            pass_points_to_selection_layer(sel);

            if(strings_extraction)
                //pass_strings_to_window();
                pass_strings_to_console();
        }
    }
}

void points_selection::send_selected_points_2(std::vector<layer> &layersSet, const glm::mat4 &ModelMatrix, const glm::mat4 &ViewMatrix, const glm::mat4 &ProjectionMatrix)
{
    /*
     * Go through each pixel checking which points are behind each pixel:
     * 1. Fill transformation matrices
     * 2. Forbid new buffer editions
     * 3. Send a ray for each pixel inside the selection square and call check_ray()
     * 4. Copy all the selected points to a layer
     */

    /*
     * Go through each point checking whether it is behind, at least, one pixel
     */

    keys_controller *kc = keys_controller::get_instance();

    if (kc->R_just_released)
    {
        layer *sel;
        if(search_lay(SELECTED_POINTS_LAYER_NAME, layersSet, sel))
        {
            // Fill the MVP arrays for the square selection
            set_transf_matrices(ModelMatrix, ViewMatrix, ProjectionMatrix);

            // First filther. Forbid new buffers editions in this main-loop iteration. If any buffer is being edited now, there is a lock_guard ahead to avoid conflicts.
            for (size_t i = 0; i < layersSet.size(); i++)
                layersSet.operator[](i).state = closed;

            // Clear the temporary vectors (size = 0, but keeps previous capacity)
            selections_repo.clear();
            if(strings_extraction) strings_repo.clear();
            //check_ray(cam.sel_xpos0, cam.sel_ypos0);      // One point selection way

            set_square_corners(viewport[3]);

            // For every vertex, check whether it is behind any pixel inside the selection square
            for(int i = 0; i < layersSet.size(); ++i)
                if( layersSet[i].layer_type == points &&
                    layersSet[i].checkbox_value &&
                    layersSet[i].layer_name != SELECTED_POINTS_LAYER_NAME)
                {
                    std::lock_guard<std::mutex> lock(*layersSet[i].mut);       // Second filther (after setting buffer_closed to closed). It avoids conflicts with buffers being currently edited (can only happen during the first iteration of this loop for each layer)

                    for(int j = 0; j < layersSet[i].objs_to_print; ++j)
                    {
                        if(is_inside_selection_square(&layersSet[i].points_buffer[j]))
                        {
                            selections_repo.push_back(glm::vec3(
                                    layersSet[i].points_buffer[j][0],
                                    layersSet[i].points_buffer[j][1],
                                    layersSet[i].points_buffer[j][2]));

                            if(strings_extraction)
                                strings_repo.push_back(layersSet[i].points_strings[j]);
                        }
                    }
                }

            pass_points_to_selection_layer(sel);

            if(strings_extraction)
                //pass_strings_to_window();
                pass_strings_to_console();
        }
    }
}

void points_selection::set_square_corners(int display_height)
{
    keys_controller *kc = keys_controller::get_instance();

    if(kc->sel_xpos0 <= kc->xpos)
    {
        x0 = kc->sel_xpos0;
        x = kc->xpos;
    }
    else
    {
        x0 = kc->xpos;
        x = kc->sel_xpos0;
    }

    // OpenGL renders with (0,0) on bottom-left, mouse reports with (0,0) on top-left
    // So, we convert Y mouse coordinates to Y OpenGL coordinates using the display height (viewport[3])
    if( (display_height - kc->sel_ypos0) <= (display_height - kc->ypos))
    {
        y0 = display_height - kc->sel_ypos0;
        y = display_height - kc->ypos;
    }
    else
    {
        y0 = display_height - kc->ypos;
        y = display_height - kc->sel_ypos0;
    }
}

bool points_selection::is_inside_selection_square(float (*point)[3])
{
    for(int column = x0; column <= x; column += pixel_step)
        for(int row = y0; row <= y; row += pixel_step)
            if(check_ray(point, column, row)) return true;

    return false;
}

bool points_selection::check_ray(float (*point)[3], double xray, double yray)
{
    /*
     * 1. From the screen coordinates, get the 2 ray ends in world-space (they are in the near and far clip plane)
     * 2. Get the direction vector of the pixel ray
     * 3. Find the closest points by testing which points' rays are close to the clickRay (checks all the points)
     */

    double dX, dY, dZ;                      // Temporary variables
    double dClickY, dClickX;                // OGL screen coordinates of the pixel
    glm::vec3 ClickRayP1, ClickRayP2;       // OGL world coordinates of the pixel ray ends (in the near and far clip plane).
    glm::vec3 pointRayP1, pointRayP2;       // OGL world coordinates of the point ray ends (in the near clip plane and the point).
    glm::vec3 ClickSlope, pointSlope;       // Direction vectors: Directions (unitary vector) of the pixel ray and the point's ray
    double sqrDist, minSqrDist = MinDistance * MinDistance;

    // Get the display minimum and maximum coordinates (pixels)
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    // From the screen coordinates, get the 2 ray ends in world-space (they are in the near and far clip plane)
    dClickX = xray;
    dClickY = double(viewport[3] - yray);         // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top (viewport[3]: display height)

    gluUnProject(dClickX, dClickY, 0.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);		// 0.0: Near clip plane
    ClickRayP1 = glm::vec3(dX, dY, dZ);
    gluUnProject(dClickX, dClickY, 1.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);		// 1.0: Far clip plane
    ClickRayP2 = glm::vec3(dX, dY, dZ);

    // Get the direction vector of the pixel ray
    ClickSlope = ClickRayP2 - ClickRayP1;           // Get the direction vector
    normalize_vec(ClickSlope);                      // Get unitary direction vector

    // Find the closest points by testing which points' rays are close to the clickRay (checks all the points)
    // TODO <<<<<<<<<< If this point was already selected (by a different ClickRay), don't check again whether it is selected

    // Point's ray is formed by the vector between the selectable point and the origin point of the clickRay
    pointRayP1 = ClickRayP1;
    pointRayP2 = glm::vec3(	point[0][0], point[0][1], point[0][2] );

    // Get the direction vector of the point ray
    pointSlope = pointRayP2 - pointRayP1;   // Get the direction vector
    normalize_vec(pointSlope);              // Get unitary direction vector

    // Check distance between the ends the direction vector of the pixel ray and the point ray
    sqrDist = distance_sqr_vec(pointSlope, ClickSlope);
    if (sqrDist < minSqrDist) return true;
    else return false;
}

void points_selection::pass_points_to_selection_layer(layer *sel)
{
    float (*pnts)[3] = new float[selections_repo.size()][3];
    for (int i = 0; i < selections_repo.size(); ++i)
    {
        pnts[i][0] = selections_repo[i][0];
        pnts[i][1] = selections_repo[i][1];
        pnts[i][2] = selections_repo[i][2];
    }

    *sel = layer(SELECTED_POINTS_LAYER_NAME, points, selections_repo.size());
    sel->send_points(selections_repo.size(), pnts, SEL_R, SEL_G, SEL_B);
    sel->alpha_channel = SEL_A;
    delete[] pnts;
}

void points_selection::normalize_vec(glm::vec3 &vec)
{
    float magnitude = sqrt( (vec.x * vec.x) + (vec.y * vec.y) + (vec.z * vec.z) );

    vec.x /= magnitude;
    vec.y /= magnitude;
    vec.z /= magnitude;
}

double points_selection::distance_sqr_vec(glm::vec3 &vec1, glm::vec3 &vec2)
{
    return	(vec1.x - vec2.x) * (vec1.x - vec2.x) +
            (vec1.y - vec2.y) * (vec1.y - vec2.y) +
            (vec1.z - vec2.z) * (vec1.z - vec2.z);
}

bool points_selection::search_lay(std::string layer_name, std::vector<layer> &layersSet, layer *&result)
{
    for(size_t i = 0; i < layersSet.size(); ++i)
        if(layersSet[i].layer_name == layer_name)
        {
            result = &layersSet[i];
            return true;
        }

    std::cout << "Layer \"" << layer_name << "\" was not found" << std::endl;
    return false;
}

void points_selection::set_transf_matrices(const glm::mat4 &ModelMatrix, const glm::mat4 &ViewMatrix, const glm::mat4 &ProjectionMatrix)
{
    // glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);			// See: glMatrixMode, glPushMatrix, glGet
    // glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
    glm::mat4 temp_mvwmatrix = ViewMatrix * ModelMatrix;
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            projmatrix[i * 4 + j]   = ProjectionMatrix[i][j];
            mvmatrix[i * 4 + j]     = temp_mvwmatrix[i][j];
        }
}

void points_selection::pass_strings_to_window()
{

}

void points_selection::pass_strings_to_console()
{
    std::cout << "\n----- " << selections_repo.size() << " selections -----" << std::endl;
    for(size_t i = 0; i < strings_repo.size(); i++)
        if(strings_repo[i] != "") std::cout << strings_repo[i] << std::endl;
}
