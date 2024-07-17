#ifndef MESH_H
#define MESH_H

#include <sstream>
#include <string>
#include <vector>
#include <regex>

#include "vec3.h"

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
        std::string material;
        std::string smoothing_group;
    };

    Mesh(std::string fname){
        if(!loadOBJFile(fname)){
            throw std::invalid_argument("Could not open file");
        }
    }

#ifdef MESH_DEBUG
    void debugPrint();
#endif


private:
    bool loadOBJFile(const std::string fname);
    void parseOBJLine(const std::string keyword, std::vector<std::string> &arguments);
    void triangulateFaces();
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

#endif