#ifndef CAMERA_H
#define CAMERA_H

#include "scene.h"
#include <fstream> //saving ppms

#define CAMERA_DEBUG

class Camera{
public:
    struct castReturn;

    struct Image{
        std::string fname;
        int width;
        int height;
        int maxColor;
        Vec3 *arr;
        ~Image(){
            delete [] arr;
        }
        void writePPM(){
            std::ofstream f;
            f.open(fname, std::ios::out);
            if(!f){
                throw std::invalid_argument("Cannot open output file");
            }

            //header        w               h            color depth
            f << "P3 " << width << " " << height << " " << maxColor << "\n";

            for(int y = 0; y < height; y++){
                for(int x = 0; x < width; x++){
                    f << arr[x + y*width].x << " ";
                    f << arr[x + y*width].y << " ";
                    f << arr[x + y*width].z << " ";
                }
                f << "\n";
            }
            f.close();
        }
    };

    Camera(Vec3 origin, Vec3 facing, float fov = 90, float tilt = 0, 
           float screen_height = 2, float screen_width = 2)
    : m_origin(origin), m_facing(facing), m_fov(fov), m_tilt(tilt), 
      m_screen_height(screen_height), m_screen_width(screen_width) 
    {}

    castReturn castRay(const Scene &scene, const Vec3 &origin, const Vec3 &dir
//dirty trick
#ifdef CAMERA_DEBUG
, bool doDebug=false
#endif
);
    void render(const Scene &scene);
    void setImage(const std::string fname, const int width, const int height, const int maxColor){
        m_img.fname = fname;
        m_img.width = width;
        m_img.height = height;
        m_img.maxColor = maxColor;
        m_img.arr = new Vec3[width*height];
        m_setImage = true;
    }
private:
    Vec3 m_origin;
    Vec3 m_facing;
    float m_fov;
    float m_tilt;
    float m_screen_height;
    float m_screen_width;

    Image m_img;
    bool m_setImage = false;
};

#endif