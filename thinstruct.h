#ifndef THINSTRUCT_H
#define THINSTRUCT_H
#include "pch.h"
class ThinStruct
{
public:
    ThinStruct();
    void read(std::string file1, std::string file2, std::string file3, std::string file4);
    std::vector<float> vertices;
    std::vector<float> edges;
    std::vector<float> splitNorm;
    std::vector<std::set<unsigned int>> neighborset;
    std::vector<std::set<unsigned int>> verticesedges;
    std::vector<std::set<unsigned int>> forceLink;
    std::vector<std::vector<unsigned int>> edgesmap;

    void calNeighbor();
    inline int getVecticesNum(){return vertices.size()/3;}
    inline int getEdgeNum(){return edges.size()/2;}
    inline QVector3D getVertice(int idx){return QVector3D(vertices[idx*3],vertices[idx*3+1],vertices[idx*3+2]);}
    inline QVector3D getSplitNorm(int idx){return QVector3D(splitNorm[idx*3],splitNorm[idx*3+1],splitNorm[idx*3+2]);}
    float innerR;
    float outerR;
    float nutR;
    int setting[3];
};

#endif // THINSTRUCT_H
