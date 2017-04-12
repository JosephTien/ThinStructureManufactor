#include "assemblyinfo.h"
void AsmTerm::creatAvaliableVecs(){
    int div = 36;
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
}
void AsmTerm::cleanAvaliableVecsByLast(){
    if(restrictVecs.size()==0)return;
    std::vector<QVector3D> _avaliableVecs;
    for(int i=0;i<(int)avaliableVecs.size();i++){
        QVector3D avaliableVec = avaliableVecs[i];
        bool nvalid=false;
        int j = (int)restrictVecs.size()-1;
        QVector3D restrictVec = restrictVecs[j];
        if(dot(avaliableVec-restrictVec,restrictVec)>0){
            nvalid = true;
        }
        if(!nvalid){
            _avaliableVecs.push_back(avaliableVec);
        }
    }
    avaliableVecs = _avaliableVecs;
}

void AsmTerm::cleanAvaliableVecs(){
    std::vector<QVector3D> _avaliableVecs;
    for(int i=0;i<(int)avaliableVecs.size();i++){
        QVector3D avaliableVec = avaliableVecs[i];
        bool nvalid=false;
        for(int j=0;j<(int)restrictVecs.size();j++){
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

void AsmTerm::delNValidVecs(QVector3D fromEdge, QVector3D toEdge){
    std::vector<QVector3D> _avaliableVecs;
    for(auto vec:avaliableVecs){
        QVector3D fromPlane = cross(fromEdge,cross(vec,fromEdge));
        QVector3D toPlane = cross(toEdge,cross(vec,toEdge));;
        if(dot(cross(vec,fromEdge),cross(vec,toEdge))>0)_avaliableVecs.push_back(vec);
    }
    avaliableVecs=_avaliableVecs;
}
//---------------------------------------------------------

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

void AssemblyInfo::addCompToSet(AsmSet &asmSet, int edge){
    int v1idx = edges[edge*2];
    int v2idx = edges[edge*2+1];
    if(v1idx>v2idx){
        int temp =  v1idx;
        v1idx = v2idx;
        v2idx = temp;
    }
    asmSet.add(edge,getVertice(v1idx),getVertice(v2idx));
}

void AssemblyInfo::getAsmSetNeighborEdges(AsmSet &asmSet)
{
    std::set<int> neighborEdges;
    std::vector<int> froms;
    std::vector<int> froms2;
    std::vector<std::set<int>> fromedges;
    std::vector<std::set<int>> fromedges2;
    froms.resize(this->getEdgeNum(),-1);
    froms2.resize(this->getEdgeNum(),-1);
    fromedges.resize(this->getEdgeNum(),std::set<int>());
    fromedges2.resize(this->getEdgeNum(),std::set<int>());
    for(auto asmComp : asmSet.asmComps){
        int edge = asmComp.edge;
        int v1idx = edges[edge*2];
        int v2idx = edges[edge*2+1];
        for(auto vert:neighbors[v1idx]){
            if(vert==v2idx)continue;
            int vidx = v1idx;
            int thatedge = edgesMap[vidx][vert];
            neighborEdges.insert(thatedge);
            if(froms[thatedge]!=-1&&froms[thatedge]!=vidx){
                froms2[thatedge] = vidx;
                fromedges2[thatedge].insert(edge);
            }
            else{
                froms[thatedge] = vidx;
                fromedges[thatedge].insert(edge);
            }
        }
        for(auto vert:neighbors[v2idx]){
            if(vert==v2idx)continue;
            int vidx = v2idx;
            int thatedge = edgesMap[vidx][vert];
            neighborEdges.insert(thatedge);
            if(froms[thatedge]!=-1&&froms[thatedge]!=vidx){
                froms2[thatedge] = vidx;
                fromedges2[thatedge].insert(edge);
            }
            else{
                froms[thatedge] = vidx;
                fromedges[thatedge].insert(edge);
            }
        }
    }
    for(auto asmComp : asmSet.asmComps){
        int edge = asmComp.edge;
        neighborEdges.erase(edge);
    }
    asmSet.infos.clear();
    for(auto edge:neighborEdges){
        asmSet.infos.insert(neighborEdgeInfo(edge ,froms[edge], fromedges[edge]));
        if(froms2[edge]!=-1){
            asmSet.infos.insert(neighborEdgeInfo(edge ,froms2[edge], fromedges2[edge]));
        }
    }
}

void AssemblyInfo::calAsmSets(){
    for(int i=0;i<getEdgeNum();i++){
        QVector3D v1,v2;
        getEdge(i,v1,v2);
        asmSets_push_back(AsmSet());
        addCompToSet(asmSets[i],i);
        asmSets[i].asmTerm.addRestrict((v2-v1).normalized(), AsmComp::flexAngle);
        asmSets[i].asmTerm.addRestrict((v1-v2).normalized(), AsmComp::flexAngle);
        getAsmSetNeighborEdges(asmSets[i]);
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
    asmSetsPrefix.clear();
    asmSetsPrefix.push_back(0);
    asmSetsPrefix.push_back(asmSets_size);
    for(int curPrefix=0;curPrefix<getVerticeNum();curPrefix++){
        for(int i=asmSetsPrefix[curPrefix];i<asmSetsPrefix[curPrefix+1];i++){
            AsmComp maxComp = *asmSets[i].asmComps.rbegin();
            for(auto info:asmSets[i].infos){
                int edge = info.neighborEdge;
                if(edge>maxComp.edge){
                    QVector3D v1,v2;
                    getEdge(edge,v1,v2);
                    AsmSet newAsmSet = asmSets[i];
                    addCompToSet(newAsmSet,edge);
                    getAsmSetNeighborEdges(newAsmSet);//can be faster(now is exhaustive)
                    newAsmSet.asmTerm.addRestrict((v2-v1).normalized(), AsmComp::flexAngle);
                    newAsmSet.asmTerm.addRestrict((v1-v2).normalized(), AsmComp::flexAngle);
                    for(auto paredge:info.parentEdges){
                        int parvert = info.parentVertice;
                        QVector3D parv1,parv2;
                        getEdge(paredge,parv1,parv2);
                        QVector3D parvec = edges[paredge*2]==parvert?parv1-parv2:parv2-parv1;
                        QVector3D neivec = edges[edge*2]==parvert?v2-v1:v1-v2;
                        newAsmSet.asmTerm.delNValidVecs(parvec,neivec);
                        //qDebug()<<parvec;
                        //qDebug()<<neivec;
                        //std::cout<< "vert : " <<parvert<<std::endl;
                        //std::cout<< "edge : " <<paredge<<std::endl;
                    }
                    if(newAsmSet.asmTerm.avaliableVecs.size()>0)asmSets_push_back(newAsmSet);
                    //------------------------------
                    for(auto e : newAsmSet.asmComps)
                        std::cout << e.edge << " ";
                    std::cout << " (avaliableSize : " << newAsmSet.asmTerm.avaliableVecs.size();
                    std::cout << std::endl;
                    //------------------------------
                }
            }
        }
        asmSetsPrefix.push_back(asmSets_size);
    }
    std::cout << std::endl;
}

void AssemblyInfo::calCombination(){
    combination_Val = FLT_MIN;
    std::stack<int> stack;
    bool * edgeVisited;
    edgeVisited = (bool*)malloc(getVerticeNum()*sizeof(bool));
    memset(edgeVisited,0,sizeof(edgeVisited));
    int edgeVisitedNum = 0;
    int cur=asmSets_size-1;
    for(auto e:asmSets[cur].asmComps){
        edgeVisited[e.edge]=true;
        edgeVisitedNum++;
    }
    stack.push(cur--);
    while(!stack.empty()||cur>=0){
        while(cur>=0){
            bool fit = true;
            for(auto e:asmSets[cur].asmComps){
                if(edgeVisited[e.edge]){
                    fit = false;
                }
            }
            if(fit){
                for(auto e:asmSets[cur].asmComps){
                    edgeVisited[e.edge]=true;
                    edgeVisitedNum++;
                }
                stack.push(cur);
            }
            cur--;
        }
        if(edgeVisitedNum==getEdgeNum()){
            combination.clear();
            std::stack<int> stack_cpy = stack;
            while(!stack_cpy.empty()){
                int edge = stack_cpy.top();
                combination.push_back(edge);
                stack_cpy.pop();
                std::cout << edge << " ";
            }
            float thisVal = energyFunction();
            if(combination_Val<thisVal){
                combination_Val=thisVal;
                combination_Best = combination;
            }
            std::cout << std::endl;
        }
        cur = stack.top();
        stack.pop();
        for(auto e:asmSets[cur].asmComps){
            edgeVisited[e.edge]=false;
            edgeVisitedNum--;
        }
        cur--;
    }

}

float AssemblyInfo::energyFunction(){
    return 1.0f/asmSets_size;
}
//clean the reallocate
