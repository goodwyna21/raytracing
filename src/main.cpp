#include <iostream>
#include <fstream>

#include "loadfile.cpp"
#include "triangulate.cpp"
#include "render.cpp"

int main(int argc, char** argv){
    const int IMG_WIDTH = 100;
    const int IMG_HEIGHT = 100;
    const int IMG_MAXCOLOR = 255;

    std::string fname = "../Objects/cube_with_face_colors.obj";
    //std::string fname = "../Objects/pentagon_face.obj";
    //std::string fname = "../Objects/skew_face.obj";
    std::shared_ptr<Mesh> m = std::make_shared<Mesh>(fname);
    Scene s;
    s.addMesh(m);
    m->debugPrint();
    Camera c({-3,0,0},{1,0,0});
    c.setImage("output.ppm",IMG_WIDTH,IMG_HEIGHT,IMG_MAXCOLOR);
    c.render(s);

    return 0;
}