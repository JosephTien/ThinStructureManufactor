#include "thinstruct.h"

ThinStruct::ThinStruct()
{

}
void ThinStruct::read(std::string file)
{
    int verticesnum;
    int edgesnum;
    std::ifstream inputFile(file.c_str());
    inputFile >> verticesnum;
    inputFile >> edgesnum;
    vertices.clear();
    edges.clear();
    for(int i=0;i<verticesnum*3;i++){
        float val;
        inputFile >> val;
        vertices.push_back(val);
    }
    for(int i=0;i<edgesnum*2;i++){
        float val;
        inputFile >> val;
        edges.push_back(val);
    }
    inputFile >> innerR;
    inputFile >> outerR;
    inputFile >> nutR;
    inputFile.close();

    calNeighbor();
}

void ThinStruct::calNeighbor(){
    for(int i=0;i<(int)vertices.size()/3;i++){
        std::set<unsigned int> pushor;
        neighborset.push_back(pushor);
    }
    for(int i=0;i<edges.size();i+=2){
        neighborset[edges[i]].insert(edges[i+1]);
        neighborset[edges[i+1]].insert(edges[i]);
    }
}
