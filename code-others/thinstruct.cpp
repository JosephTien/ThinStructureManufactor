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
    edgesCutNorm.clear();
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

    forceLink.resize(vertices.size()/3,std::set<unsigned int>());
    int linkNum;
    inputFile >> linkNum;
    for(int i=0;i<linkNum;i++){
        int v,e;
        inputFile >> v;
        inputFile >> e;
        forceLink[v].insert(e);
    }

    inputFile >> setting[0];//clean cut?
    inputFile >> setting[1];//slot?
    inputFile >> setting[2];//lock?

    inputFile.close();

    calNeighbor();
}

void ThinStruct::calNeighbor(){
    int vs = (int)vertices.size()/3;
    std::set<unsigned int> pushor;
    neighborset.resize(vs,pushor);
    //forceLink.resize(vs,pushor);
    verticesedges.resize(vs,pushor);
    std::vector<unsigned int> vpushor;vpushor.resize(vs,-1);
    edgesmap.resize(vs, vpushor);

    for(int i=0;i<edges.size();i+=2){
        neighborset[edges[i]].insert(edges[i+1]);
        neighborset[edges[i+1]].insert(edges[i]);
        verticesedges[edges[i]].insert(i/2);
        verticesedges[edges[i+1]].insert(i/2);
        edgesmap[edges[i]][edges[i+1]]=edgesmap[edges[i+1]][edges[i]] = i/2;
    }
    for(int i=0;i<vs;i++){
        if(verticesedges[i].size()==1){
            for(auto temp : verticesedges[i])forceLink[i].insert(temp);
        }
    }
}
