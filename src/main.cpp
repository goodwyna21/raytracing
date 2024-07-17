#include <iostream>
#include <fstream>

#include "loadfile.cpp"

int main(int argc, char** argv){
    std::string fname = "../Objects/cube_with_face_colors.obj";
    Mesh m(fname);
    //changed line
    return 0;
}