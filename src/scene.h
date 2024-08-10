#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "mesh.h"

class Scene{
public:
    Scene(Vec3 background = {0,0,0}){ m_background = background; }
    void addMesh(std::shared_ptr<Mesh> m){
        m_objects.push_back(m);
    }
    int objectCount() const { return m_objects.size(); }
    std::shared_ptr<Mesh> object(int index) const { return m_objects.at(index); }
private:
    std::vector<std::shared_ptr<Mesh>> m_objects;
    Vec3 m_background; //background color
};

#endif