#include "assemblyinfo.h"
void AsmTerm::creatAvaliableVecs(){
    avaliableVecs.clear();
    int thdiv = div*4;
    for(int theta=0;theta<thdiv;theta++){
        avaliableVecs.push_back(QVector3D(cos(2*M_PI*theta/div), sin(2*M_PI*theta/div), 0));
    }
    for(int phi=1;phi<=div;phi++){
        float phsin = sin(M_PI/2*phi/div);
        float phcos = cos(M_PI/2*phi/div);
        int thdiv = div*phcos*4+1;
        for(int theta=0;theta<thdiv;theta++){
            avaliableVecs.push_back(QVector3D(phcos*cos(2*M_PI*theta/thdiv), phcos*sin(2*M_PI*theta/thdiv), phsin));
        }
        for(int theta=0;theta<thdiv;theta++){
            avaliableVecs.push_back(QVector3D(phcos*cos(2*M_PI*theta/thdiv), phcos*sin(2*M_PI*theta/thdiv), -phsin));
        }
    }
    cleanAvaliableVecs();
}

void AsmTerm::cleanAvaliableVecs(){
    std::vector<QVector3D> _avaliableVecs;
    for(int i=0;i<avaliableVecs.size();i++){
        QVector3D avaliableVec = avaliableVecs[i];
        bool nvalid=false;
        for(int j=0;j<restrictVecs.size();j++){
            QVector3D restrictVec = restrictVecs[j];
            if(dot(avaliableVec-restrictVec,restrictVec)>0){
                nvalid = true;
                break;
            }
        }
        if(!nvalid){
            _avaliableVecs.push_back(avaliableVec);
        }
    }
    avaliableVecs = _avaliableVecs;
}
//---------------------------------------------------------
AssemblyInfo::AssemblyInfo()
{

}

AssemblyInfo::AssemblyInfo(std::vector<float> vertices, std::vector<int> edges){
    this->vertices = vertices;
    this->edges = edges;
    neighbors.resize(getVerticeNum(),std::set<int>());
    //std::vector<int> pushor;pushor.resize(getVerticeNum(),-1);
    //edgesMap.resize(getVerticeNum(),pushor);
    for(int i=0;i<getVerticeNum();i++){
        edgesMap.push_back(std::vector<int>());
        for(int j=0;j<getVerticeNum();j++){
            edgesMap[i].push_back(-1);
        }
    }
    for(int i=0;i<getEdgeNum();i++){
        neighbors[edges[i*2]].insert(edges[i*2+1]);
        neighbors[edges[i*2+1]].insert(edges[i*2]);
        edgesMap[edges[i*2]][edges[i*2+1]]=i;
        edgesMap[edges[i*2+1]][edges[i*2]]=i;
    }
}

std::set<neighborEdgeInfo> AssemblyInfo::getAsmSetNeighborEdges(int idx){
    std::set<int> neighborEdges;
    std::vector<int> froms;
    std::vector<int> froms2;
    std::vector<std::set<int>> fromedges;
    froms.resize(this->getEdgeNum(),-1);
    froms2.resize(this->getEdgeNum(),-1);
    fromedges.resize(this->getEdgeNum(),std::set<int>());
    for(auto asmComp : asmSets[idx].asmComps){
        int edge = asmComp.edge;
        int v1idx = edges[edge*2];
        int v2idx = edges[edge*2+1];
        for(auto vert:neighbors[v1idx]){
            int vidx = v1idx;
            int thatedge = edgesMap[vidx][vert];
            neighborEdges.insert(thatedge);
            if(froms[thatedge]!=-1&&froms[thatedge]!=vidx)froms2[thatedge] = vidx;
            else froms[thatedge] = vidx;
            fromedges[thatedge].insert(edge);
        }
        for(auto vert:neighbors[v2idx]){
            int vidx = v2idx;
            int thatedge = edgesMap[vidx][vert];
            neighborEdges.insert(thatedge);
            if(froms[thatedge]!=-1&&froms[thatedge]!=vidx)froms2[thatedge] = vidx;
            else froms[thatedge] = vidx;
            fromedges[thatedge].insert(edge);
        }
    }
    for(auto asmComp : asmSets[idx].asmComps){
        int edge = asmComp.edge;
        neighborEdges.erase(edge);
    }
    std::set<neighborEdgeInfo> infos;
    infos.clear();
    for(auto edge:neighborEdges){
        infos.insert(neighborEdgeInfo(edge ,froms[edge], fromedges[edge]));
        if(froms2[edge]!=-1){
            infos.insert(neighborEdgeInfo(edge ,froms2[edge], fromedges[edge]));
        }
    }
    return infos;
}

void AssemblyInfo::calAsmSets(){
    for(int i=0;i<getEdgeNum();i++){
        QVector3D v1,v2;
        getEdge(i,v1,v2);
        asmSets.push_back(AsmSet());
        asmSets[i].add(i,v1,v2);
        asmSets[i].asmTerm.addRestrict((v2-v1).normalized(), AsmComp::flexAngle);
        asmSets[i].asmTerm.addRestrict((v1-v2).normalized(), AsmComp::flexAngle);
        asmSets[i].infos = getAsmSetNeighborEdges(i);
        /*
        for(auto e : asmSets[i].infos){
            qDebug() << e.neighborEdge;
            qDebug() << " " << e.parentVertice;
            for(auto es : e.parentEdges){
                qDebug() << "  " << es;
            }
        }
        qDebug() << "";
        */

    }

}

void AssemblyInfo::calCombination(){

}
