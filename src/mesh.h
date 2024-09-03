#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
#include <regex>

#include "vec3.h"

//#define MESH_DEBUG
#define MESH_DEBUG_PRINT

#define OBJ_NO_FIELD (std::string)"__NONE__"
#define PNT_NO_FIELD -1

class Mesh{
public:
    //stores indices of vertices
    struct Point{
        int vertex;
        int texture = PNT_NO_FIELD;
        int normal = PNT_NO_FIELD;
        Point(int _vertex) : vertex(_vertex) {}
        Point(int v, int t, int n) : 
            vertex(v), texture(t), normal(n) {}
    };

    struct Face{
        std::vector<Point> points;
        //std::vector<int> vertices;
        //std::vector<int> textures;
        //std::vector<int> norms;
        int normal;
        float planeOffset;
        std::string material;
        std::string smoothing_group;
    };

    Mesh(std::string fname){
        if(!loadOBJFile(fname)){
            throw std::invalid_argument("Could not open file");
        }
        triangulateFaces();
        addNormals();
    }

    int faceCount() const { return m_faces.size(); }
    Face getFace(int index) const { return m_faces.at(index); }
    Vec3 getNormal(int index) const { return m_normals.at(index); }
    Vec3 getVertex(int index) const { return m_vertices.at(index); }

#ifdef MESH_DEBUG_PRINT
    void debugPrint();
    void printFaces();
#endif


private:
    bool loadOBJFile(const std::string fname);
    void parseOBJLine(const std::string keyword, std::vector<std::string> &arguments);
    void triangulateFaces();
    void addNormals();
    void addPlaneOffsets();
    std::vector<std::shared_ptr<Face>> triangulateFace(int index);
    
    std::vector<Vec3> m_vertices;
    std::vector<Vec3> m_texture_coords;
    std::vector<Vec3> m_normals;
    std::vector<Face> m_faces;

    //materials
    std::string m_mtllib = OBJ_NO_FIELD;
    std::string m_cur_smoothing_group = OBJ_NO_FIELD;
    std::string m_cur_material = OBJ_NO_FIELD;
};


#ifdef MESH_DEBUG_PRINT
#include <iostream>

void Mesh::debugPrint(){
    std::cout << "-------- Mesh Contents --------\n";
    std::cout << "Vertex count:  " << m_vertices.size() << "\n";
    std::cout << "Faces count:   " << m_faces.size() << "\n";
    std::cout << "Texture count: " << m_texture_coords.size() << "\n";
    std::cout << "Normals count: " << m_normals.size() << "\n";
    std::cout << "---------- Vertices -----------\n";
    for(int i = 0; i < m_vertices.size(); i++){
        std::cout << i << ": " << m_vertices.at(i).x << " "
                               << m_vertices.at(i).y << " "
                               << m_vertices.at(i).z << "\n";
    }
    std::cout << "------------ Faces ------------\n";
    for(int i = 0; i < m_faces.size(); i++){
        std::cout << i << ":\n";
        std::cout << "Material: " << m_faces.at(i).material << "\n";
        std::cout << "Smoothing group: " << m_faces.at(i).smoothing_group << "\n";
        std::cout << "Vertices: ";
        for(Mesh::Point j : m_faces.at(i).points){
            std::cout << j.vertex << " ";
        }
        std::cout << "\nTextures: ";
        for(Mesh::Point j : m_faces.at(i).points){
            std::cout << j.texture << " ";
        }
        std::cout << "\nNormals: ";
        for(Mesh::Point j : m_faces.at(i).points){
            std::cout << j.normal << " ";
        }
        std::cout << "\n";
    }
}

void Mesh::printFaces(){
    for(Face f : m_faces){
        std::cout << "--------\n";
        for(Point p : f.points){
            std::cout << m_vertices.at(p.vertex).x << " "
                      << m_vertices.at(p.vertex).y << " "
                      << m_vertices.at(p.vertex).z << " \n";
        }
    }
    std::cout << "Number of faces: " << m_faces.size() << "\n";
}
#endif


#endif