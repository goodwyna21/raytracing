#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "vec3.h"

class Mesh;

class Scene{
public:
    Scene(){}
    void addMesh(std::shared_ptr<Mesh> m){
        m_objects.push_back(m);
    }
private:
    std::vector<std::shared_ptr<Mesh>> m_objects;
    Vec3 m_background; //background color
};

#endif