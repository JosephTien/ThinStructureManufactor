#ifndef ASSEMBLYINFO_H
#define ASSEMBLYINFO_H
#include<pch.h>
class AsmComp{
public:
    AsmComp(int edge, QVector3D splitNorm, QVector3D asmDir){
        this->edge = edge;
        this->splitNorm = splitNorm;
        this->asmDir = asmDir;
    }
    int edge;
    QVector3D splitNorm;
    QVector3D asmDir;
    bool operator < (const AsmComp& asmComp) const
    {
        return this->edge < asmComp.edge;
    }
    bool operator == (const AsmComp& asmComp) const
    {
        return this->edge == asmComp.edge;
    }
    bool operator > (const AsmComp& asmComp) const
    {
        return this->edge > asmComp.edge;
    }
};

class AsmSet : std::set<AsmComp>{
public:
    AsmSet(){}
    AsmSet(std::set<AsmComp> asmComp){(*this) = asmComp;}
    bool add(int edge, QVector3D splitNorm, QVector3D asmDir){
        AsmComp asmComp = AsmComp(edge, splitNorm, asmDir);
        if(this->count(asmComp)>0){
            this->erase(asmComp);
            this->insert(asmComp);
            return true;
        }else{
            this->insert(asmComp);
            return false;
        }
    }
};
class AssemblyInfo
{
public:
    AssemblyInfo();
    AssemblyInfo(std::vector<float> vertices, std::vector<int> edges);

    std::vector<float> vertices;
    std::vector<int> edges;
    std::vector<AsmSet> asmSets;
    std::vector<int> asmSetsPrefix;
    std::vector<int> combination;


    inline int getEdgeNum(){return edges.size()/2;}
    inline QVector3D getVertice(int idx){return QVector3D(vertices[idx*3],vertices[idx*3+1],vertices[idx*3+2]);}
    inline QVector3D getEdgeIdx(int idx, int &v1idx, int &v2idx){v1idx = edges[idx*2];v2idx = edges[idx*2+1];}
    inline QVector3D getEdge(int idx, QVector3D &v1, QVector3D &v2){v1 = getVertice(edges[idx*2]);v2 = getVertice(edges[idx*2+1]);}
    void calAsmSets();
    void calCombination();

};

#endif // ASSEMBLYINFO_H
