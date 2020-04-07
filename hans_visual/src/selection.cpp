
#include "selection.hpp"

void selection::check_selections(){

    if (cam.R_just_released)
    {
    // >>> Fill the MVP arrays for the square selection (projmatrix[], mvmatrix[], viewport[]):

        glGetIntegerv(GL_VIEWPORT, viewport);
        // glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);			// See: glMatrixMode, glPushMatrix, glGet
        // glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);

        glm::mat4 modelviewmatrix = ViewMatrix * ModelMatrix;

        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                projmatrix[i * 4 + j] = ProjectionMatrix[i][j];
                mvmatrix[i * 4 + j] = modelviewmatrix[i][j];
            }

    // >>> Forbid new buffers editions in this main-loop iteration. If any buffer is being edited now, there is a lock_guard ahead to avoid conflicts.
        for (size_t i = 0; i < layersSet->size(); i++) layersSet->operator[](i).state = closed;
        // Clear the vector (size = 0, but keeps previous capacity) that will pass the selected points to the layer
        temp_selections.clear();

    // >>> Point selection search

        //check_ray(cam.sel_xpos0, cam.sel_ypos0);      // One point selection way

        // Check the direction to where you draw the selection square
        signed int xstep, ystep;
        if(cam.sel_xpos0 <= cam.sel_xpos) xstep = 1;
        else xstep = -1;
        if(cam.sel_ypos0 <= cam.sel_ypos) ystep = 1;
        else ystep = -1;

        // Send a ray for each pixel inside the selection square
        for(double column = cam.sel_xpos0; ; column += xstep)
        {
            if(xstep == 1 && column > cam.sel_xpos) break;
            else if(xstep == -1 && column < cam.sel_xpos) break;

            for(double row = cam.sel_ypos0; ; row += ystep)
            {
                if(ystep == 1 && row > cam.sel_ypos) break;
                else if(ystep == -1 && row < cam.sel_ypos) break;

                check_ray(column, row);
            }
        }

        // Copy all the selected points to a layer (Selections)
        layersSet->operator[](0) = layer("Selections", points, temp_selections.size());
        layersSet->operator[](0).objs_to_print = temp_selections.size();
        for(size_t i = 0; i < temp_selections.size(); i++)
        {
            layersSet->operator[](0).points_buffer[i][0] = temp_selections[i][0];
            layersSet->operator[](0).points_buffer[i][1] = temp_selections[i][1];
            layersSet->operator[](0).points_buffer[i][2] = temp_selections[i][2];

            layersSet->operator[](0).points_color_buffer[i][0] = selection_color[0];
            layersSet->operator[](0).points_color_buffer[i][1] = selection_color[1];
            layersSet->operator[](0).points_color_buffer[i][2] = selection_color[2];
            layersSet->operator[](0).points_color_buffer[i][3] = selection_color[3];
        }

        std::cout << "\n----- Selections -----" << std::endl;
        for(size_t i = 0; i < layersSet->size(); i++)
        {
            if(layersSet->operator[](i).layer_type == points)
                for(size_t j = 0; j < layersSet->operator[](i).objs_to_print; j++)
                    if(layersSet->operator[](i).points_strings[j] != "") std::cout << layersSet->operator[](i).points_strings[j] << std::endl;
        }

        cam.R_just_released = false;
    }
}

void selection::check_ray(double xpos, double ypos) {

    double dX, dY, dZ;                      // Temporary variables
    double dClickY, dClickX;                // OGL screen coordinates of the pixel
    glm::vec3 ClickRayP1, ClickRayP2;       // OGL world coordinates of the pixel ray ends (in the near and far clip plane).
    glm::vec3 pointRayP1, pointRayP2;       // OGL world coordinates of the point ray ends (in the near clip plane and the point).
    glm::vec3 ClickSlope, pointSlope;       // Direction vectors: Directions (unitary vector) of the pixel ray and the point's ray
    double sqrDist, minSqrDist = MinDistance * MinDistance;

    // From the screen coordinates, get the 2 ray ends in world-space (they are in the near and far clip plane)
    dClickX = xpos;
    dClickY = double(display_h - ypos);         // OpenGL renders with (0,0) on bottom, mouse reports with (0,0) on top

    gluUnProject(dClickX, dClickY, 0.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);		// 0.0: Near clip plane
    ClickRayP1 = glm::vec3(dX, dY, dZ);
    gluUnProject(dClickX, dClickY, 1.0, mvmatrix, projmatrix, viewport, &dX, &dY, &dZ);		// 1.0: Far clip plane
    ClickRayP2 = glm::vec3(dX, dY, dZ);

    // Get the direction vector of the pixel ray
    ClickSlope = ClickRayP2 - ClickRayP1;           // Get the direction vector
    cam.normalize_vec(ClickSlope);                  // Get unitary direction vector

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
            cam.normalize_vec(pointSlope);          // Get unitary direction vector

            // Check distance between the ends the direction vector of the pixel ray and the point ray
            sqrDist = cam.distance_sqr_vec(pointSlope, ClickSlope);
            if (sqrDist < minSqrDist)
                temp_selections.push_back(glm::vec3(layersSet->operator[](i).points_buffer[j][0],
                                                    layersSet->operator[](i).points_buffer[j][1],
                                                    layersSet->operator[](i).points_buffer[j][2]));
        }
    }
}
