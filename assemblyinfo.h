#ifndef ASSEMBLYINFO_H
#define ASSEMBLYINFO_H
#include<pch.h>
#define ALMOSTONE 0.97f
#define ALMOSTZERO 0.2f
class AsmComp{
public:
    AsmComp(int edge, QVector3D v1, QVector3D v2){
        this->edge = edge;
        this->v1 = v1;
        this->v2 = v2;
        this->direction = (v2-v1).normalized();
    }
    constexpr static float flexAngle = 45.0f;
    int edge;
    QVector3D v2;
    QVector3D v1;
    QVector3D direction;
    bool operator < (const AsmComp& asmComp) const
    {
        return this->edge < asmComp.edge;
    }
    bool operator == (const AsmComp& asmComp) const
    {
        return this->edge == asmComp.edge;
    }
    bool operator != (const AsmComp& asmComp) const
    {
        return this->edge != asmComp.edge;
    }
    bool operator > (const AsmComp& asmComp) const
    {
        return this->edge > asmComp.edge;
    }
};

class AsmTerm{
public:
    //constexpr static int div = 36;
    AsmTerm(){
        creatAvaliableVecs();
    }
    std::vector<QVector3D> restrictVecs;
    //std::vector<float> restrictAngles;
    std::vector<QVector3D> avaliableVecs;

    void creatAvaliableVecs();

    void cleanAvaliableVecs();

    void cleanAvaliableVecsByLast();

    void delNValidVecs(QVector3D fromEdge, QVector3D toEdge);

    void addRestrict(QVector3D restrictVec, float restrictAngle){
        restrictVecs.push_back(restrictVec.normalized()*sin(restrictAngle/M_PI*180));
        //restrictAngles.push_back(restrictAngle);
        cleanAvaliableVecsByLast();
    }

    float dot(QVector3D a, QVector3D b)const{
        return QVector3D::dotProduct(a.normalized(), b.normalized());
    }

    QVector3D cross(QVector3D a, QVector3D b)const{
        return QVector3D::crossProduct(a.normalized(), b.normalized());
    }

    int size() const{
        return restrictVecs.size();
    }

    AsmTerm& operator = (const AsmTerm& asmTerm){
        this->restrictVecs = asmTerm.restrictVecs;
        this->avaliableVecs = asmTerm.avaliableVecs;
        //this->restrictAngles = asmTerm.restrictAngles;
        return *this;
    }
    AsmTerm& operator += (const AsmTerm& asmTerm){
        for(int i=0;i<asmTerm.size();i++){
            this->restrictVecs.push_back(asmTerm.restrictVecs[i]);
            //this->restrictAngles.push_back(asmTerm.restrictAngles[i]);
        }
        return *this;
    }

    AsmTerm operator + (const AsmTerm& asmTermR) const{
        return (AsmTerm()=*this) += asmTermR;
    }
};

class neighborEdgeInfo{
public:
    neighborEdgeInfo(int neighborEdge, int parentVertice, std::set<int> parentEdges){
        this->neighborEdge = neighborEdge;
        this->parentVertice = parentVertice;
        this->parentEdges = parentEdges;
    }
    int neighborEdge;
    int parentVertice;
    std::set<int> parentEdges;
    bool operator < (const neighborEdgeInfo& info) const
    {
        if(this->neighborEdge == info.neighborEdge)
            return this->parentVertice < info.parentVertice;
        return this->neighborEdge < info.neighborEdge;
    }
    bool operator == (const neighborEdgeInfo& info) const
    {
        if(this->neighborEdge == info.neighborEdge)
            return this->parentVertice == info.parentVertice;
        return this->neighborEdge == info.neighborEdge;
    }
    bool operator != (const neighborEdgeInfo& info) const
    {
        if(this->neighborEdge == info.neighborEdge)
            return this->parentVertice != info.parentVertice;
        return this->neighborEdge != info.neighborEdge;
    }
    bool operator > (const neighborEdgeInfo& info) const
    {
        if(this->neighborEdge == info.neighborEdge)
            return this->parentVertice > info.parentVertice;
        return this->neighborEdge > info.neighborEdge;
    }
    neighborEdgeInfo& operator = (const neighborEdgeInfo& info)
    {
        this->neighborEdge = info.neighborEdge;
        this->parentEdges = info.parentEdges;
        this->parentVertice = info.parentVertice;
        return *this;
    }
};
class AsmSet{
public:
    AsmSet(){}
    AsmSet(std::set<AsmComp> asmSet){(*this) = asmSet;}
    std::set<AsmComp> asmComps;
    AsmTerm asmTerm;
    std::set<neighborEdgeInfo> infos;
    /*
    bool addInfo(neighborEdgeInfo info);
    bool delInfo(neighborEdgeInfo info);
    */
    bool add(AsmComp asmComp){
        if(asmComps.count(asmComp)>0){
            asmComps.erase(asmComp);
            asmComps.insert(asmComp);
            return true;
        }else{
            asmComps.insert(asmComp);
            return false;
        }
    }

    bool add(int edge, QVector3D v1, QVector3D v2){
        AsmComp asmComp = AsmComp(edge, v1, v2);
        if(asmComps.count(asmComp)>0){
            asmComps.erase(asmComp);
            asmComps.insert(asmComp);
            return true;
        }else{
            asmComps.insert(asmComp);
            return false;
        }
    }
    std::vector<AsmComp> getAsmCompsVector() const
    {
        std::vector<AsmComp> asmCompsVector;
        for(auto asmComp:asmComps){
            asmCompsVector.push_back(asmComp);
        }
        return asmCompsVector;
    }
    bool operator < (const AsmSet& asmSet) const
    {
        std::vector<AsmComp> thisSet = this->getAsmCompsVector();
        std::vector<AsmComp> thatSet = asmSet.getAsmCompsVector();
        if(thisSet.size()<thatSet.size())return true;
        if(thisSet.size()>thatSet.size())return false;
        int ss = thisSet.size();
        for(int i=0;i<ss;i++){
            if(thisSet[i]<thatSet[i])return true;
            if(thisSet[i]>thatSet[i])return false;
        }
        return false;
    }

    bool operator == (const AsmSet& asmSet) const
    {
        std::vector<AsmComp> thisSet = this->getAsmCompsVector();
        std::vector<AsmComp> thatSet = asmSet.getAsmCompsVector();
        if(thisSet.size()<thatSet.size())return false;
        if(thisSet.size()>thatSet.size())return false;
        int ss = thisSet.size();
        for(int i=0;i<ss;i++){
            if(thisSet[i]!=thatSet[i])return false;
        }
        return true;
    }
    bool operator > (const AsmSet& asmSet) const
    {
        std::vector<AsmComp> thisSet = this->getAsmCompsVector();
        std::vector<AsmComp> thatSet = asmSet.getAsmCompsVector();
        if(thisSet.size()<thatSet.size())return false;
        if(thisSet.size()>thatSet.size())return true;
        int ss = thisSet.size();
        for(int i=0;i<ss;i++){
            if(thisSet[i]<thatSet[i])return false;
            if(thisSet[i]>thatSet[i])return true;
        }
        return false;
    }

    AsmSet& operator = (const AsmSet& asmSet)
    {
        this->asmComps = asmSet.asmComps;
        this->asmTerm = asmSet.asmTerm;
        this->infos = asmSet.infos;
        return *this;
    }

    AsmSet& operator += (const AsmSet& asmSet){
        this->asmTerm+=asmSet.asmTerm;
        for(auto asmComp : asmSet.asmComps){
            this->add(asmComp);
        }
        return *this;
    }
    AsmSet operator + (const AsmSet& asmSetR)const{
        return AsmSet(*this)+=asmSetR;
    }
};
class AssemblyInfo
{
public:
    AssemblyInfo();
    AssemblyInfo(std::vector<float> vertices, std::vector<int> edges);

    std::vector<float> vertices;
    std::vector<int> edges;
    std::vector<std::set<int>> neighbors;
    std::vector<std::vector<int>> edgesMap;
    AsmSet asmSets[1000];
    int asmSets_size = 0;
    std::vector<int> asmSetsPrefix;
    std::vector<int> combination;
    std::vector<int> combination_Best;
    float combination_Val;
    std::vector<int> verticeBelong;

    inline int getVerticeNum(){return vertices.size()/3;}
    inline int getEdgeNum(){return edges.size()/2;}
    inline QVector3D getVertice(int idx){return QVector3D(vertices[idx*3],vertices[idx*3+1],vertices[idx*3+2]);}
    inline void getEdgeIdx(int idx, int &v1idx, int &v2idx){v1idx = edges[idx*2];v2idx = edges[idx*2+1];}
    inline void getEdge(int idx, QVector3D &v1, QVector3D &v2){v1 = getVertice(edges[idx*2]);v2 = getVertice(edges[idx*2+1]);}
    void asmSets_push_back(AsmSet asmSet){asmSets[asmSets_size++] = asmSet;}
    void getAsmSetNeighborEdges(AsmSet &asmSet);
    void addCompToSet(AsmSet &asmSet, int edge);
    void calAsmSets();
    void calCombination();
    float energyFunction();

};

#endif // ASSEMBLYINFO_H
