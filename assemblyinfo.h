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
class AsmTermExp{
public:
    AsmTermExp(){}
    AsmTermExp(int asmType,QVector3D asmUp,QVector3D asmRight,QVector3D asmForw,float asmForwRLB, float asmForwRUB, float asmRightRLB, float asmRightRUB){
        this->asmType=asmType;
        this->asmUp=asmUp.normalized();
        this->asmRight=asmRight.normalized();
        this->asmForw=asmForw.normalized();
        this->asmForwRLB=asmForwRLB;
        this->asmForwRUB=asmForwRUB;
        this->asmRightRLB=asmRightRLB;
        this->asmRightRUB=asmRightRUB;

    }
    int asmType;        //2 : Up&Forw ; 1 : Up ; 0 : const
    QVector3D asmUp;
    QVector3D asmRight;
    QVector3D asmForw;
    float asmForwRLB;
    float asmForwRUB;
    float asmRightRLB;
    float asmRightRUB;
    bool operator ==(const AsmTermExp& asmTerm) const{
        return (QVector3D::dotProduct(this->asmUp,asmTerm.asmUp)>0.95f);
    }
};

class AsmTerm{
public:
    AsmTerm(){}
    std::vector<AsmTermExp> _;
    int size()const{
        return _.size();
    }
    void add(int asmType, QVector3D asmUp,QVector3D asmForw,float asmRightRLB,float asmRightRUB, float asmForwRLB, float asmForwRUB){
        _.push_back(AsmTermExp(asmType
                                ,asmUp.normalized()
                                ,asmForw.normalized()
                                ,QVector3D::crossProduct(asmForw, asmUp).normalized()
                                ,asmRightRLB
                                ,asmRightRUB
                                ,asmForwRLB
                                ,asmForwRUB));
    }
    void add(QVector3D asmUp,QVector3D asmForw,float asmRightRLB,float asmRightRUB, float asmForwRLB, float asmForwRUB){
        add(2, asmUp, asmForw, asmRightRLB, asmRightRUB, asmForwRLB, asmForwRUB);
    }
    void add(QVector3D asmUp,QVector3D asmForw,float asmForwRLB,float asmForwRUB){
        add(1, asmUp, asmForw, 0, 0, asmForwRLB, asmForwRUB);
    }
    void add(QVector3D asmUp){
        add(0, asmUp, QZERO, 0, 0, 0, 0);//auto set right to zero?
    }

    float dot(QVector3D a, QVector3D b)const{
        return QVector3D::dotProduct(a.normalized(), b.normalized());
    }

    QVector3D cross(QVector3D a, QVector3D b)const{
        return QVector3D::crossProduct(a.normalized(), b.normalized());
    }

    AsmTerm operator + (const AsmTerm& asmTerm) const{
        AsmTerm rtn;
        for(int i=0;i<size();i++){
            rtn._.push_back(this->_[i]);
        }
        for(int i=0;i<size();i++){
            rtn._.push_back(asmTerm._[i]);
        }
        return rtn;
    }

    AsmTerm operator * (const AsmTerm& asmTerm) const;
};
class AsmSet : std::set<AsmComp>{
public:
    AsmSet(){}
    AsmSet(std::set<AsmComp> asmComp){(*this) = asmComp;}
    AsmTerm asmTerm;
    bool add(int edge, QVector3D v1, QVector3D v2){
        AsmComp asmComp = AsmComp(edge, v1, v2);
        if(this->count(asmComp)>0){
            this->erase(asmComp);
            this->insert(asmComp);
            return true;
        }else{
            this->insert(asmComp);
            return false;
        }
    }
    std::vector<AsmComp> getAsmComps() const
    {
        std::vector<AsmComp> asmComps;
        for(auto asmComp:*this){
            asmComps.push_back(asmComp);
        }
        return asmComps;
    }
    bool operator < (const AsmSet& asmSet) const
    {
        std::vector<AsmComp> thisSet = this->getAsmComps();
        std::vector<AsmComp> thatSet = asmSet.getAsmComps();
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
        std::vector<AsmComp> thisSet = this->getAsmComps();
        std::vector<AsmComp> thatSet = asmSet.getAsmComps();
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
        std::vector<AsmComp> thisSet = this->getAsmComps();
        std::vector<AsmComp> thatSet = asmSet.getAsmComps();
        if(thisSet.size()<thatSet.size())return false;
        if(thisSet.size()>thatSet.size())return true;
        int ss = thisSet.size();
        for(int i=0;i<ss;i++){
            if(thisSet[i]<thatSet[i])return false;
            if(thisSet[i]>thatSet[i])return true;
        }
        return false;
    }
    AsmSet operator + (const AsmSet& asmSet) const;
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
