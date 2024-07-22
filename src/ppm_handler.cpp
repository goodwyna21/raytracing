#ifndef PPM_HANDLER
#define PPM_HANDLER

#include "vec3.h"
#include <fstream>

void writePPM(const std::string &fname, const Vec3 image[], 
              const int width, const int height, const int maxColor){
    std::ofstream f;
    f.open(fname, std::ios::out);
    if(!f){
        throw std::invalid_argument("Cannot open output file");
    }
    
    //header        w               h            color depth
    f << "P3 " << width << " " << height << " " << maxColor << "\n";

    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            f << image[x + y*width].x << " ";
            f << image[x + y*width].y << " ";
            f << image[x + y*width].z << " ";
        }
        f << "\n";
    }
    f.close();
}

#endif