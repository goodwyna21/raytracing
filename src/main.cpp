#include <iostream>
#include <fstream>

#include "loadfile.cpp"
#include "triangulate.cpp"

int main(int argc, char** argv){
    std::string fname = "../Objects/cube_with_face_colors.obj";
    Mesh m(fname);

    m.printFaces();
    std::cout << "Done\n";
    return 0;
}