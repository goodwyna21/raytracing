#ifndef RENDER_CPP
#define RENDER_CPP

#include "camera.h"


#include <iostream>
Vec3 Camera::castRay(const Scene &scene, const Vec3 &origin, const Vec3 &dir){
    std::cout << dir.x << " " << dir.y << " " << dir.z << "\n";
}

void Camera::render(const Scene &scene){
    assert(m_setImage && "No image set");

    float scr_dist = tan(m_fov)*m_screen_height/2;
    Vec3 Ihat = Vec3::cross(m_facing, {0,0,1}).normalize();
    Vec3 Jhat = Vec3::cross(m_facing, Ihat).normalize();

    //top left corner of image
    Vec3 p0 = (scr_dist * m_facing) - (m_screen_width * Ihat) + (m_screen_height * Jhat);
    //offset vector for x and y directions
    Vec3 dx = (m_screen_width / m_img.width) * Ihat;
    Vec3 dy = -(m_screen_height / m_img.height) * Jhat;

    for(int x = 0; x < m_img.width; x++){
        for(int y = 0; y < m_img.height; y++){
            m_img.arr[x + m_img.width*y] = castRay(scene, m_origin, p0 + (x*dx) + (y*dy));
        }
    }
    m_img.writePPM();
}

#endif