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
void AssemblyInfo::calAsmSets_quick(){
    for(int i=0;i<getEdgeNum();i++){
        QVector3D v1,v2;
        getEdge(i,v1,v2);
        asmSets_push_back(AsmSet());
        addCompToSet(asmSets[i],i);
        asmSets[i].asmTerm.addRestrict((v2-v1).normalized(), AsmComp::flexAngle);
        asmSets[i].asmTerm.addRestrict((v1-v2).normalized(), AsmComp::flexAngle);
        getAsmSetNeighborEdges(asmSets[i]);
    }
    for(int i=0;i<asmSets_size;i++){
        combination_Best.push_back(i);
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
                    newAsmSet.nodes.insert(info.parentVertice);
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
    edgeVisited = (bool*)malloc(getEdgeNum()*sizeof(bool));
    memset(edgeVisited,0,sizeof(edgeVisited));
    bool * verticeVisited;
    verticeVisited = (bool*)malloc(getVerticeNum()*sizeof(bool));
    memset(verticeVisited,0,sizeof(verticeVisited));
    int edgeVisitedNum = 0;
    int verticeVisitedNum = 0;
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
            for(auto v:asmSets[cur].nodes){
                if(verticeVisited[v]){
                    fit = false;
                }
            }
            if(fit){
                for(auto e:asmSets[cur].asmComps){
                    edgeVisited[e.edge]=true;
                    edgeVisitedNum++;
                }
                for(auto v:asmSets[cur].nodes){
                    verticeVisited[v]=true;
                    verticeVisitedNum++;
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
        for(auto v:asmSets[cur].nodes){
            verticeVisited[v]=false;
            verticeVisitedNum--;
        }
        cur--;
    }

}

float AssemblyInfo::energyFunction(){
    return 1.0f/asmSets_size;
}

void AssemblyInfo::calLink(){

    int * vertBelongToGroup;
    vertBelongToGroup = (int*)malloc(getVerticeNum()*sizeof(int));
    for(int i=0;i<getVerticeNum();i++){
        vertBelongToGroup[i]=-1;
    }
    int * edgeBelongToGroup;
    edgeBelongToGroup = (int*)malloc(getEdgeNum()*sizeof(int));
    for(auto c:combination_Best){
        AsmSet asmSet = myAsmSets[myAsmSets_size]=asmSets[c];
        for(auto comp:asmSet.asmComps){
            edgeBelongToGroup[comp.edge]=myAsmSets_size;
        }
        for(auto v:asmSet.nodes){
            vertBelongToGroup[v]=myAsmSets_size;
        }
        myAsmSets_size++;
    }
    for(int i=0;i<getVerticeNum();i++){
        if(neighbors[i].size()==1){
           int neivert = *neighbors[i].begin();
           int neiedge = edgesMap[i][neivert];
           vertBelongToGroup[i]=edgeBelongToGroup[neiedge];
        }
    }
    //-----------------------------------------------------------

    std::vector<std::vector<int>> groupMap(myAsmSets_size, std::vector<int>(myAsmSets_size, 0));
    std::vector<std::set<int>> groupNeighborList(myAsmSets_size, std::set<int>());
    std::set<IdxPair> linkPair;
    std::set<IdxPair> waitAssignSet;
    std::vector<IdxPair> waitAssignVec;

    for(int i=0;i<myAsmSets_size;i++){
        int curgroup = i;
        for(auto info:myAsmSets[i].infos){
            int taredge = info.neighborEdge;
            int curvert = info.parentVertice;
            int targroup = edgeBelongToGroup[taredge];

            linkPair.insert(IdxPair(curgroup,targroup));
            groupNeighborList[curgroup].insert(targroup);
            groupNeighborList[targroup].insert(curgroup);

            if(myAsmSets[i].nodes.count(curvert)>0){
                groupMap[curgroup][targroup]=1;
                groupMap[targroup][curgroup]=-1;
            }else{
                groupMap[curgroup][targroup]=2;
                groupMap[targroup][curgroup]=2;
                waitAssignSet.insert(IdxPair(curgroup,targroup));
            }

        }
    }
    for(auto pair:waitAssignSet){waitAssignVec.push_back(pair);}
    //-----------------------------------------------------------
    int binary[1000]={0};
    int wz = waitAssignVec.size();
    int lz = linkPair.size();
    int gz = myAsmSets_size;
    int minLocker = INT_MAX;
    int minBeginer = INT_MAX;
    std::vector<std::vector<int>> myGroupMap_best;
    while(binary[wz]==0){
        std::vector<std::vector<int>> myGroupMap = groupMap;
        std::vector<std::set<int>> myGroupNeighborList = groupNeighborList;
        std::set<IdxPair> myLinkPair = linkPair;
        for(int j=0;j<wz;j++){
            int idx1 = waitAssignVec[j].idx1;
            int idx2 = waitAssignVec[j].idx2;
            if(binary[j]==0){
                myGroupMap[idx1][idx2]=1;
                myGroupMap[idx2][idx1]=-1;
            }else{
                myGroupMap[idx1][idx2]=-1;
                myGroupMap[idx2][idx1]=1;
            }
        }

        //-----------------------------

        std::vector<int>depths(gz,-1);
        std::queue<int> queue;
        queue.push(0);
        depths[0]=100;
        bool nValid = false;

        while(queue.size()>0){
            nValid = false;
            int cur = queue.front();
            queue.pop();
            std::vector<int> curGroupNeighborList;
            for(auto tar : myGroupNeighborList[cur])curGroupNeighborList.push_back(tar);
            for(auto tar : curGroupNeighborList){
                if(depths[tar]==-1){
                    depths[tar]=depths[cur]+myGroupMap[cur][tar];
                    myGroupNeighborList[cur].erase(tar);
                    myGroupNeighborList[tar].erase(cur);
                    myLinkPair.erase(IdxPair(cur,tar));
                    queue.push(tar);
                }else if( (myGroupMap[cur][tar]<0&&depths[tar]>depths[cur])
                        ||(myGroupMap[cur][tar]>0&&depths[tar]<depths[cur]))
                {
                    nValid = true;
                    break;
                }
            }
            if(nValid)break;
        }
        for(int i=0;i<myGroupNeighborList.size();i++){
            if(myGroupNeighborList[i].size()!=0)nValid=true;
        }
        if(!nValid){
            int lockernum = 0;
            int beginernum = 0;
            for(int i=0;i<groupNeighborList.size();i++){
                bool islocker=true;
                bool isbeginer=true;
                for(auto g:groupNeighborList[i]){
                    if(depths[i]<depths[g])islocker=false;
                    if(depths[i]>depths[g])isbeginer=false;
                }
                if(islocker)lockernum++;
                if(isbeginer)beginernum++;
            }
            if(minLocker>lockernum||(minLocker==lockernum&&minBeginer>beginernum)){
               minLocker=lockernum;
               minBeginer=beginernum;
               myGroupMap_best = myGroupMap;
            }
            //-----------------
            for(int i=0;i<wz;i++){
                std::cout << binary[i] << " ";
            }
            std::cout << std::endl << "     ";
            std::cout << "Locker Num : " << lockernum << std::endl;
            std::cout << std::endl << "     ";
            std::cout << "Beginer Num : " << beginernum << std::endl;
            std::cout << std::endl << "     ";
            std::cout << "Depths : ";
            for(int i=0;i<gz;i++){
                std::cout << depths[i] << " ";
            }
            std::cout << std::endl;
        }
        //-----------------------------

        binary[0]++;
        for(int j=0;binary[j]==2;j++){
            if(binary[j]==2){
                binary[j+1]++;
                binary[j]=0;
            }
        }

    }
    //-----------------------------------------------------------
    links.resize(getVerticeNum(),std::set<int>());

    for(int i=0;i<myAsmSets_size;i++){
        AsmSet asmSet = myAsmSets[i];
        for(auto node:asmSet.nodes){
            for(auto edge:neighbors[node]){
                if(edgeBelongToGroup[edge]==vertBelongToGroup[node]){
                    links[node].insert(edge);
                }
            }
        }
        int thisgroup = i;
        for(auto info:asmSet.infos){
            int thatgroup = edgeBelongToGroup[info.neighborEdge];
            if(myGroupMap_best[thisgroup][thatgroup]==1){
                links[info.parentVertice].insert(info.neighborEdge);
            }
        }

    }

    //-----------------
    std::cout << std::endl;
    std::cout << "Best Link :" << std::endl;
    for(int i=0;i<links.size();i++){
        std::cout <<"     "<< i<< " : ";
        for(auto edge:links[i]){
            std::cout << edge << " ";
        }
        std::cout << std::endl;
    }

}

//clean the reallocate
