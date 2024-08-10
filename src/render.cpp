#ifndef RENDER_CPP
#define RENDER_CPP

#include "camera.h"

#include <iostream>
#include <limits>

struct Camera::castReturn{
    bool didCollide = false;
    int object;
    int face;
    float distance;
    Vec3 point;
};

Camera::castReturn Camera::castRay(const Scene &scene, const Vec3 &origin, const Vec3 &dir){
    std::cout << dir.x << " " << dir.y << " " << dir.z << "\n";

    float t;
    Vec3 normal;
    float denom;
    Vec3 A,B,C,Q; //triangle points and collision

    //current closest hit
    int closest_obj = -1;
    int closest_face = -1;
    Vec3 collision_pnt;
    float distance = std::numeric_limits<float>::max();

    for(int obj = 0; obj < scene.objectCount(); obj++){
        for(int face = 0; face < scene.object(obj)->faceCount(); face++){
            normal = scene.object(obj)->getNormal(scene.object(obj)->getFace(face).normal);

            //intersection from https://courses.cs.washington.edu/courses/csep557/09sp/lectures/triangle_intersection.pdf

            //get intersection point with ray and supporting plane
            denom = Vec3::dot(normal, dir);
            //check for parallels
            if(denom == 0){
                continue;
            }
            //get intersection point
            t = (scene.object(obj)->getFace(face).planeOffset - Vec3::dot(normal, origin))
                 / denom;

            A = scene.object(obj)->getVertex(scene.object(obj)->getFace(face).points.at(0).vertex);
            B = scene.object(obj)->getVertex(scene.object(obj)->getFace(face).points.at(1).vertex);
            C = scene.object(obj)->getVertex(scene.object(obj)->getFace(face).points.at(2).vertex);

            Q = origin + (t * dir);

            //check inside or outside of triangle
            if(Vec3::dot(Vec3::cross(B-A, Q-A), normal) >= 0 &&
               Vec3::dot(Vec3::cross(C-B, Q-B), normal) >= 0 &&
               Vec3::dot(Vec3::cross(A-C, Q-C), normal) >= 0){
                //valid collision
                if(t < distance){
                    closest_obj = obj;
                    closest_face = face;
                    collision_pnt = Q;
                }
            }   
        }
    }

    if(closest_obj == -1){ //no collision
        return {};
    }
    return {true, closest_obj, closest_face, distance, collision_pnt};
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
            Camera::castReturn collision = castRay(scene, m_origin, p0 + (x*dx) + (y*dy));

            //WIP FIX THIS, right now it only sets the pixel white if
            //there is a collision, black otherwise
            if(collision.didCollide){
                m_img.arr[x + m_img.width*y] = {(float)m_img.maxColor,
                                                (float)m_img.maxColor,
                                                (float)m_img.maxColor};
            } else {
                m_img.arr[x + m_img.width*y] = {0,0,0};
            }
        }
    }
    m_img.writePPM();
}

#endif