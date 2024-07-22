#ifndef TRIANGULATE_CPP

#include "mesh.h"

//returns +1 on one side of line a-b
//and -1 on the other,
//and 0 if colinear
int getSideOfLine(Vec3 a, Vec3 b, Vec3 c){
    float det = (b.x - a.x)*(c.y - a.y) > (c.x - a.x)*(b.y - a.y);
    if(det > 0) return 1;
    if(det < 0) return -1;
    return 0;
}

std::vector<std::shared_ptr<Mesh::Face>> Mesh::triangulateFace(int index){
#ifdef MESH_DEBUG
    std::cout << "Triangulate Face: " << index << "\n";
#endif
    std::vector<std::shared_ptr<Mesh::Face>> ret; //vectors to be added back in to faces list

    std::shared_ptr<Face> f = std::make_shared<Face>(m_faces.at(index));

    if(f->points.size() <= 3){
        ret.push_back(f);
        return ret;
    }

    //generate 2d projections of points onto the shared plane
    std::vector<Vec3> coplanar_pts;
    Vec3 origin = m_vertices.at(f->points.at(2).vertex);
    Vec3 p1 = m_vertices.at(f->points.at(0).vertex) - origin;
    Vec3 p2 = m_vertices.at(f->points.at(1).vertex) - origin;

#ifdef MESH_DEBUG
    std::cout << "p2:" << p2.x << ", " << p2.y << ", " << p2.z << "\n";    
#endif

    Vec3 u = (p1 - ((Vec3::dot(p1,p2)/Vec3::dot(p2,p2))*p2)).normalize();
    Vec3 v = p2.normalize();

#ifdef MESH_DEBUG
    std::cout << "p1:" << p1.x << ", " << p1.y << ", " << p1.z << "\n";
    std::cout << "p2:" << p2.x << ", " << p2.y << ", " << p2.z << "\n";    
    std::cout << "u: " << u.x << ", " << u.y << ", " << u.z << "\n";
    std::cout << "v: " << v.x << ", " << v.y << ", " << v.z << "\n";
#endif

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

    //THIS IS A HORRIBLE IDEA O(N^5) WHAT KIND OF MORON DID THIS
    //(hopefully this isn't a big deal and faces dont have too many points)
    std::vector<int> side_a;
    std::vector<int> side_b;
    int point_index;
    int side_of_line;

#ifdef MESH_DEBUG
    int _loop_count = 0;
#endif


    while(f->points.size() > 3 ){

#ifdef MESH_DEBUG
    std::cout << "points.size() = " << f->points.size() << "\n";
#endif

        for(int i = 0; i < f->points.size()-1; i++){
            for(int j = i+1; j < f->points.size(); j++){
                for(int k = 0; k < f->points.size(); k++){
                    if(k == i || k == j) continue;
                    side_of_line = getSideOfLine(coplanar_pts.at(i),
                                                 coplanar_pts.at(j),
                                                 coplanar_pts.at(k));

/*
#ifdef MESH_DEBUG
                    std::cout << "side: " << side_of_line << "\n";
                    std::cout << "a: " << m_vertices.at(f->points.at(i).vertex).x << ", "
                                       << m_vertices.at(f->points.at(i).vertex).y << ", "
                                       << m_vertices.at(f->points.at(i).vertex).z << "\n"
                              << "b: " << m_vertices.at(f->points.at(j).vertex).x << ", "
                                       << m_vertices.at(f->points.at(j).vertex).y << ", "
                                       << m_vertices.at(f->points.at(j).vertex).z << "\n"
                              << "b: " << m_vertices.at(f->points.at(k).vertex).x << ", "
                                       << m_vertices.at(f->points.at(k).vertex).y << ", "
                                       << m_vertices.at(f->points.at(k).vertex).z << "\n\n";
#endif*/


                    if(side_of_line > 0){
                        side_a.push_back(k);
                    } else if(side_of_line < 0){
                        side_b.push_back(k);
                    }
                }
                if(side_a.size() == 1 || side_b.size() == 1){
                    if(side_a.size() == 1){
                        point_index = side_a.front();
                    } else {
                        point_index = side_b.front();
                    }
                    ret.push_back(std::make_shared<Face>(Face()));
                    ret.back()->material = f->material;
                    ret.back()->smoothing_group = f->smoothing_group;
                    ret.back()->points.push_back(f->points.at(i));
                    ret.back()->points.push_back(f->points.at(j));
                    ret.back()->points.push_back(f->points.at(point_index));

                    f->points.erase(f->points.begin() + point_index);
                    coplanar_pts.erase(coplanar_pts.begin() + point_index);
                }
                side_a.clear();
                side_b.clear();
            }
        }
    }

    if(f->points.size() == 3){
        ret.push_back(f);
    } 
    return ret;
}

void Mesh::triangulateFaces(){
    std::vector<std::shared_ptr<Face>> new_faces;
    std::vector<std::shared_ptr<Face>> returned_faces;
    for(int i = 0; i < m_faces.size(); i++){
        returned_faces = triangulateFace(i);
        new_faces.insert(std::end(new_faces), 
                         std::begin(returned_faces), 
                         std::end(returned_faces));
    }

    m_faces.clear();
    for(std::shared_ptr<Face> f : new_faces){
        m_faces.push_back(*f);
    }
#ifdef MESH_DEBUG
    std::cout << "Finished Triangulation\n";
    debugPrint();
#endif
}

#endif