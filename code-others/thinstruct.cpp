#include "thinstruct.h"

ThinStruct::ThinStruct()
{

}
void ThinStruct::read(std::string file1, std::string file2, std::string file3, std::string file4)
{
    int verticesnum;
    int edgesnum;
    std::ifstream inputFile1(file1.c_str());
    inputFile1 >> verticesnum;
    inputFile1 >> edgesnum;
    vertices.clear();
    edges.clear();
    splitNorm.clear();
    for(int i=0;i<verticesnum*3;i++){
        float val;
        inputFile1 >> val;
        vertices.push_back(val);
    }
    for(int i=0;i<edgesnum*2;i++){
        float val;
        inputFile1 >> val;
        edges.push_back(val);
    }
    inputFile1.close();
    /***************************************/
    std::ifstream inputFile3(file3.c_str());
    inputFile3 >> innerR;
    inputFile3 >> outerR;
    inputFile3 >> nutR;
    inputFile3 >> setting[0];//clean cut?
    inputFile3 >> setting[1];//slot?
    inputFile3 >> setting[2];//lock?
    inputFile3.close();
    /***************************************/
    std::ifstream inputFile2(file2.c_str());
    forceLink.resize(vertices.size()/3,std::set<unsigned int>());
    int linkNum;
    inputFile2 >> linkNum;
    for(int i=0;i<linkNum;i++){
        int v,e;
        inputFile2 >> v;
        inputFile2 >> e;
        forceLink[v].insert(e);
    }

    inputFile2.close();
    /***************************************/
    std::ifstream inputFile4(file4.c_str());
    int es = edges.size()/2;
    splitNorm.clear();
    for(int i=0;i<es;i++){
        float val;
        inputFile4 >> val;splitNorm.push_back(val);
        inputFile4 >> val;splitNorm.push_back(val);
        inputFile4 >> val;splitNorm.push_back(val);
    }
    inputFile4.close();
    /***************************************/
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
