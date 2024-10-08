#ifndef LOADFILE_CPP
#define LOADFILE_CPP

#include <fstream>
#include "mesh.h"

void Mesh::parseOBJLine(const std::string keyword, std::vector<std::string> &arguments){
    float n; //used for storing arguments converted to floats
    
    std::regex face_argument_expr("^([0-9-]*)\\/([0-9-]*)\\/([0-9-]*)$");
    std::smatch match;

    assert(!arguments.empty() && "Arguments list empty in parseOBJLine");

    //materials file
    if(keyword == "mtllib"){
        m_mtllib = arguments.at(0);
    }
    //set current material
    if(keyword == "usemtl"){
        m_cur_material = arguments.at(0);
    }
    //set current face group
    if(keyword == "g"){
        m_cur_smoothing_group = arguments.at(0);
    }
    //vertex
    if(keyword == "v"){ 
        m_vertices.push_back({  std::stof(arguments.at(0)),
                                std::stof(arguments.at(1)),
                                std::stof(arguments.at(2))});
    }
    //vertex normal
    else if(keyword == "vn"){
        m_normals.push_back({   std::stof(arguments.at(0)),
                                std::stof(arguments.at(1)),
                                std::stof(arguments.at(2))});
    }
    //vertex texture
    else if(keyword == "vt"){
        m_texture_coords.push_back({std::stof(arguments.at(0)),
                                    std::stof(arguments.at(1),0)});
    }
    //face
    else if(keyword == "f"){
        m_faces.push_back(Face());
        m_faces.back().material = m_cur_material;
        m_faces.back().smoothing_group = m_cur_smoothing_group;
        for(int i = 0; i < arguments.size(); i++){
            //check if simple argument (just vertex index)
            if(arguments.at(i).find('/') == std::string::npos){ 
                n = std::stoi(arguments.at(i));
                if(n < 0){
                    m_faces.back().points.push_back(m_vertices.size() - n);
                } else {
                    m_faces.back().points.push_back(n - 1);
                }
            } else {
            //complex argument (v/vt/vn)
                std::regex_search(arguments.at(i), match, face_argument_expr);
                if(match.empty()){
                    throw std::invalid_argument("Invalid face argument: " + keyword + arguments.at(i));
                }
                //vertex index
                n = std::stoi(match[1]);
                if(n < 0){
                    m_faces.back().points.push_back(m_vertices.size() - n);
                } else {
                    m_faces.back().points.push_back(n - 1);
                }
                //vertex texture index
                if(!match[2].str().empty()){
                    n = std::stoi(match[2]);
                    if(n < 0){
                        m_faces.back().points.back().texture = m_vertices.size() - n;
                    } else {
                        m_faces.back().points.back().texture = n - 1;
                    }
                }
                //vertex normal index
                if(!match[3].str().empty()){
                    n = std::stoi(match[3]);
                    if(n < 0){
                        m_faces.back().points.back().normal = m_vertices.size() - n;
                    } else {
                        m_faces.back().points.back().normal = n - 1;
                    }
                }
            }
        }
    }
}

bool Mesh::loadOBJFile(const std::string fname){
#ifdef MESH_DEBUG
    std::cout << "Start OBJ read " << fname << "\n";
#endif

    std::ifstream file(fname);
    if(!file) return false;

#ifdef MESH_DEBUG
    std::cout << "Opened " << fname << "\n";
#endif

    std::string line;
    std::regex keyword_expr("^\\s*([a-zA-Z]+)\\s*");
    std::regex argument_expr("^([-0-9a-zA-Z\\./_]+)\\s*");
    std::smatch match;
    std::string keyword;
    std::vector<std::string> arguments;

#ifdef MESH_DEBUG
    int i = 0;
#endif
    //parse lines
    while(!file.eof()){
        std::getline(file,line);

#ifdef MESH_DEBUG
        std::cout << "\n------------\nLine " << i+1 << "\n------------\n";
        i++;
        std::cout << line << "\n";
#endif

        //match keyword expr
        std::regex_search(line, match, keyword_expr);
        
        if(match.size() < 2){ //handles empty lines
            continue;
        }

#ifdef MESH_DEBUG
        std::cout << "(" << match[1] << ")\n";
#endif

        keyword = match[1]; //1st capture group is the keyword

        //check for comment
        if(keyword == "#") continue;

        //remove keyword and surrounding whitespace
        line = std::regex_replace(line,keyword_expr,"");

        //get arguments
        while(!line.empty()){
            std::regex_search(line, match, argument_expr);
#ifdef MESH_DEBUG
            std::cout << match.size() << "\n";
#endif
            if(match.size() < 2) continue;
            arguments.push_back(match[1]);
            line = std::regex_replace(line, argument_expr, "");
        }

        parseOBJLine(keyword, arguments);

#ifdef MESH_DEBUG
        std::cout << keyword << "\n";
        for(auto arg : arguments){
            std::cout << "(" << arg << ")\n";
        }
#endif

        arguments.clear();
    }

#ifdef MESH_DEBUG
    debugPrint();
#endif

    file.close();
    return true;
}

void Mesh::addNormals(){
    for(Face f : m_faces){
        f.normal = f.points.at(0).normal;
        if(f.normal != PNT_NO_FIELD){
            continue;
        }
        m_normals.push_back(Vec3::cross(m_vertices.at(f.points.at(1).vertex) - m_vertices.at(f.points.at(0).vertex),
                                        m_vertices.at(f.points.at(2).vertex) - m_vertices.at(f.points.at(0).vertex))
                            .normalize());
        f.normal = m_normals.size() - 1;
    }
}

void Mesh::addPlaneOffsets(){
    for(Face f : m_faces){
        f.planeOffset = Vec3::dot(m_normals.at(f.normal),m_vertices.at(f.points.at(0).vertex));
    }
}

#endif