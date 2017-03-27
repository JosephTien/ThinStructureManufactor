#ifndef IGLMACHING_H
#define IGLMACHING_H

#include <pch.h>
#include <glm/glm.hpp>
#include <igl/read_triangle_mesh.h>
#include <igl/copyleft/cgal/mesh_boolean.h>
#include <igl/copyleft/cgal/CSGTree.h>
#include <Eigen/Core>

#include <igl/writeOFF.h>

class IglMesh
{
public:
    IglMesh(){}
    Eigen::MatrixXd V;
    Eigen::MatrixXi F;
    std::string name;
    std::vector<float> vertices;
    std::vector<int> indices;
    void setName(std::string name){
        this->name = name;
    }
    void getData(std::vector<float> &vertices, std::vector<unsigned int> &indices){
        this->vertices.clear();
        this->indices.clear();
        for(int i=0;i<V.rows();i++){
            this->vertices.push_back(V(i,0));
            this->vertices.push_back(V(i,1));
            this->vertices.push_back(V(i,2));
        }
        for(int i=0;i<F.rows();i++){
            this->indices.push_back(F(i,0));
            this->indices.push_back(F(i,1));
            this->indices.push_back(F(i,2));
        }
        vertices = this->vertices;
        indices.clear();
        for(int i=0;i<this->indices.size();i++){
            indices.push_back(this->indices[i]);
        }
    }
    void setData(std::vector<float> vertices, std::vector<int> indices){
        this->vertices = vertices;
        this->indices = indices;
        V.resize(vertices.size()/3,3);
        F.resize(indices.size()/3,3);
        for(int i=0;i<V.rows();i++){
            V(i,0) = vertices[i*3];
            V(i,1) = vertices[i*3+1];
            V(i,2) = vertices[i*3+2];
        }
        for(int i=0;i<F.rows();i++){
            F(i,0) = indices[i*3];
            F(i,1) = indices[i*3+1];
            F(i,2) = indices[i*3+2];
        }
    }
    void setData(std::vector<float> vertices, std::vector<unsigned int> indices){
        this->indices.clear();
        for(int i=0;i<(int)indices.size();i++){
            this->indices.push_back((int)indices[i]);
        }
        setData(vertices, this->indices);
    }

    void setData(Eigen::MatrixXd V, Eigen::MatrixXi F){
        this->V = V;
        this->F = F;
        std::vector<double> vertices_;
        vertices_.resize(V.rows()*V.cols(),0);
        Eigen::MatrixXd::Map(vertices_.data(), 3, V.rows()) = V.transpose();
        vertices.clear();
        for(int i=0;i<(int)vertices_.size();i++){
            vertices.push_back(vertices_[i]);
        }
        indices.resize(F.rows()*F.cols(),0);
        Eigen::MatrixXi::Map(indices.data(), 3, F.rows()) = F.transpose();
    }

    void set(std::string name,std::vector<float> vertices, std::vector<int> indices){
        setName(name);
        setData(vertices, indices);
    }
    void set(std::string name,std::vector<float> vertices, std::vector<unsigned int> indices){
        setName(name);
        setData(vertices, indices);
    }
    void set(std::string name, Eigen::MatrixXd V, Eigen::MatrixXi F){
        setName(name);
        setData(V,F);
    }
    //todo : apply matrix
};

class IglMachine
{
public:
    IglMachine(){}
    const static int dataNumLimit = 50;
    int curDataTail=0;
    IglMesh mesh[dataNumLimit];
    igl::MeshBooleanType boolean_type;
    const static int treeNum=10;
    igl::copyleft::cgal::CSGTree M[treeNum];

    int searchTar(std::string tarName){
        for(int i=0;i<curDataTail;i++){
            if(tarName == mesh[i].name){
                return i;
            }
        }
        return -1;
    }
    void putNew(std::string name){
        int tar = searchTar(name);
        if(tar == -1){
            mesh[curDataTail++].setName(name);
        }else{

        }
    }
    void put(std::string name,std::vector<float> vertices, std::vector<unsigned int> indices){
        int tar = searchTar(name);
        if(tar == -1){
            mesh[curDataTail++].set(name, vertices, indices);
        }else{
            mesh[tar].setData(vertices, indices);
        }
    }
    void put(std::string name,std::vector<float> vertices, std::vector<int> indices){
        int tar = searchTar(name);
        if(tar == -1){
            mesh[curDataTail++].set(name, vertices, indices);
        }else{
            mesh[tar].setData(vertices, indices);
        }
    }
    void put(std::string name, Eigen::MatrixXd V, Eigen::MatrixXi F){
        int tar = searchTar(name);
        if(tar == -1){
            mesh[curDataTail++].set(name, V, F);
        }else{
            mesh[tar].setData(V, F);
        }
    }
    void get(std::string name,std::vector<float> &vertices, std::vector<unsigned int> &indices){
        int tar = searchTar(name);
        if(tar != -1){
            mesh[tar].getData(vertices, indices);
        }
    }

    void readFile(std::string name, const char * filePath){
        Eigen::MatrixXd V;
        Eigen::MatrixXi F;
        igl::read_triangle_mesh(filePath,V,F);
        put(name, V, F);
    }
    void writeFile(std::string name, char * filePath){
        int tar = searchTar(name);
        if(tar>=0){
            igl::writeOBJ(filePath,mesh[tar].V,mesh[tar].F);
        }else{

        }
    }
    void writeFileOff(std::string name, char * filePath){
        int tar = searchTar(name);
        if(tar>=0){
            igl::writeOFF(filePath,mesh[tar].V,mesh[tar].F);
        }else{

        }
    }

    Eigen::MatrixXd getV(std::string name){
        int tar = searchTar(name);
        if(tar == -1){
            return mesh[dataNumLimit-1].V;
        }else{
            return mesh[tar].V;
        }
    }
    Eigen::MatrixXi getF(std::string name){
        int tar = searchTar(name);
        if(tar == -1){
            return mesh[dataNumLimit-1].F;
        }else{
            return mesh[tar].F;
        }
    }
    void command(std::string line){
        std::string name[3];
        int tar[3];
        float val[3];
        std::string instruction;
        std::stringstream scriptIfs(line);
        scriptIfs >> instruction;
        if(instruction == "NEW"){
            scriptIfs >> name[0];
            putNew(name[0]);
        }else if(instruction == "+"){
            //std::cout<<"Start Union..."<<std::endl;
            boolean_type = igl::MESH_BOOLEAN_TYPE_UNION;
            scriptIfs >> name[0] >> name[1] >> name[2];
            tar[0] = searchTar(name[0]);
            tar[1] = searchTar(name[1]);
            tar[2] = searchTar(name[2]);
            igl::copyleft::cgal::mesh_boolean(mesh[tar[1]].V,mesh[tar[1]].F,mesh[tar[2]].V,mesh[tar[2]].F,boolean_type,mesh[tar[0]].V,mesh[tar[0]].F);
        }else if(instruction == "-"){
            //std::cout<<"Start Minus..."<<std::endl;
            boolean_type = igl::MESH_BOOLEAN_TYPE_MINUS;
            scriptIfs >> name[0] >> name[1] >> name[2];
            tar[0] = searchTar(name[0]);
            tar[1] = searchTar(name[1]);
            tar[2] = searchTar(name[2]);
            igl::copyleft::cgal::mesh_boolean(mesh[tar[1]].V,mesh[tar[1]].F,mesh[tar[2]].V,mesh[tar[2]].F,boolean_type,mesh[tar[0]].V,mesh[tar[0]].F);
        }else if(instruction == "*"){
            //std::cout<<"Start Intersec..."<<std::endl;
            boolean_type = igl::MESH_BOOLEAN_TYPE_INTERSECT;
            scriptIfs >> name[0] >> name[1] >> name[2];
            tar[0] = searchTar(name[0]);
            tar[1] = searchTar(name[1]);
            tar[2] = searchTar(name[2]);
            igl::copyleft::cgal::mesh_boolean(mesh[tar[1]].V,mesh[tar[1]].F,mesh[tar[2]].V,mesh[tar[2]].F,boolean_type,mesh[tar[0]].V,mesh[tar[0]].F);
        }else if(instruction == "SCALE"){
            float x,y,z;
            scriptIfs >> name[0]>> name[1] >> x >> y >> z;
            tar[0] = searchTar(name[0]);
            tar[1] = searchTar(name[1]);
            std::vector<float> vertices_new = mesh[tar[1]].vertices;
            for(int i=0;i<(int)vertices_new.size();i+=3){
                vertices_new[i]*=x;
                vertices_new[i+1]*=y;
                vertices_new[i+2]*=z;
            }
            put(name[0],vertices_new, mesh[tar[1]].indices);
        }else if(instruction == "TRANS"){
            float x,y,z;
            scriptIfs >> name[0]>> name[1] >> x >> y >> z;
            tar[0] = searchTar(name[0]);
            tar[1] = searchTar(name[1]);
            std::vector<float> vertices_new = mesh[tar[1]].vertices;
            for(int i=0;i<(int)vertices_new.size();i+=3){
                vertices_new[i]+=x;
                vertices_new[i+1]+=y;
                vertices_new[i+2]+=z;
            }
            put(name[0],vertices_new, mesh[tar[1]].indices);
        }
    }
    void fetchTree(std::string name, int tarTree){
        Eigen::MatrixXi MF = M[tarTree].F();
        Eigen::MatrixXd MV = M[tarTree].cast_V<Eigen::MatrixXd>();
        put(name, MV, MF);
    }
    void reset(){
        curDataTail = 0;
        for(int i=0;i<treeNum;i++){
            M[i] = {};
        }
    }
    //todo ex-handle


};

#endif // IGLMACHING_H
