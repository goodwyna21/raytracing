#ifndef TRIANGULATE_CPP

#include "mesh.h"

//returns true on one side of line a-b
//and negative on the other or colinear
bool getSideOfLine(Vec3 a, Vec3 b, Vec3 c){
    return (b.x - a.x)*(c.y - a.y) > (c.x - a.x)*(b.y - a.y);
}

std::vector<std::shared_ptr<Mesh::Face>> Mesh::triangulateFace(int index){
    std::vector<std::shared_ptr<Mesh::Face>> ret; //vectors to be added back in to faces list

    std::shared_ptr<Face> f = std::make_shared<Face>(m_faces.at(index));

    if(f->points.size() <= 3){
        ret.push_back(f);
        return ret;
    }

    //generate 2d projections of points onto the shared plane
    std::vector<Vec3> coplanar_pts;
    Vec3 p1 = m_vertices.at(f->points.at(0).vertex);
    Vec3 p2 = m_vertices.at(f->points.at(1).vertex);

    Vec3 u = (p1 - ((Vec3::dot(p1,p2)/Vec3::dot(p2,p2))*p2)).normalize();
    Vec3 v = p2.normalize();
    float c1, c2;

    for(int i = 0; i < f->points.size(); i++){
        c1 = Vec3::dot(m_vertices.at(f->points.at(i).vertex),u)/Vec3::dot(u,u);
        c2 = Vec3::dot(m_vertices.at(f->points.at(i).vertex),v)/Vec3::dot(v,v);
        coplanar_pts.push_back({c1,c2});
    }

    /* Algorithm works by selecting a line between
     * Two points, then checking if that splits off
     * exactly one point from the rest. If it does,
     * Create a new face with the two points that formed 
     * the line and the one point split off, then remove the
     * split off point from the original face
    */
    while(m_faces.at(index).points.size() > 3){
        
    }

    ret.push_back(std::make_shared<Mesh::Face>(m_faces.at(index)));
    return ret;
}

void Mesh::triangulateFaces(){
    
}

#endif