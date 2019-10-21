// http://csharphelper.com/blog/2015/12/build-a-geodesic-sphere-with-wpf-and-c/

#include "visualizerClass.hpp"
#include "header.hpp"

buffers dat;

int main() {

    myclass obj;
    obj.visual.open_window();

    obj.send_data();

    obj.visual.add_layer(points, "New layer", 12);
    obj.visual.send_points("New layer", 12, &dat.pnts[0][0]);

    char abc;
    std::cin >> abc;
}
