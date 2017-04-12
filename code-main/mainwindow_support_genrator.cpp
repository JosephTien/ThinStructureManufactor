#include<mainwindow_support.h>

void MainWindow_support::genTube(){
    ThinStruct * ts = &var->ts;
    float iR=ts->innerR/50,oR=ts->outerR/50,nR=ts->nutR/50;
    for(int i=0;i<ts->edges.size()/2;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,(v1-v2).length()/100-oR),
                    (v1+v2)/2,
                    v2-v1);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(iR,iR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
        applyCSG('-', getTarnum()-2, getTarnum()-1);
        glMain()->deleteTar(getTarnum()-1);
        /*
        if(ts->neighborset[ts->edges[i*2]].size()==1){
            putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,oR),
                        v1,
                        QVector3D(0,0,1));
            applyCSG('+', getTarnum()-2, getTarnum()-1);
            glMain()->deleteTar(getTarnum()-1);
        }
        if(ts->neighborset[ts->edges[i*2+1]].size()==1){
            putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,oR),
                        v2,
                        QVector3D(0,0,1));
            applyCSG('+', getTarnum()-2, getTarnum()-1);
            glMain()->deleteTar(getTarnum()-1);
        }
        */
        /*
        int cur = getTarnum()-1;
        int tar = getTarnum();
        glMain()->copyObj(cur);
        glMain()->genSlotCav((v1+v2)/2, v2-v1, QVector3D(0,0,1));
        applyCSG('-',cur,getTarnum()-1);
        applyCSG('*',tar,getTarnum()-1);
        glMain()->deleteTar(getTarnum()-1);
        */
    }
}

void MainWindow_support::genNut(){
    ThinStruct *ts = &var->ts;
    float iR=ts->innerR/50,oR=ts->outerR/50,nR=ts->nutR/50;
    for(int i=0;i<ts->vertices.size()/3;i++){
        putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(nR,nR,nR),
                    QVector3D(ts->vertices[i*3], ts->vertices[i*3+1], ts->vertices[i*3+2]),
                    QVector3D(0,0,1));
        bool caved=false;
        for(auto idx : ts->neighborset[i]){
            QVector3D v1 = ts->getVertice(i);
            QVector3D v2 = ts->getVertice(idx);
            putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(iR,iR,(v1-v2).length()/100),
                        (v1+v2)/2,
                        v2-v1);
            applyCSG('-', getTarnum()-2, getTarnum()-1);
            glMain()->deleteTar(getTarnum()-1);

            putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,(v1-v2).length()/100-oR),
                        (v1+v2)/2,
                        v2-v1);
            applyCSG('-', getTarnum()-2, getTarnum()-1);
            glMain()->deleteTar(getTarnum()-1);

            if(!caved){
                putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                            QVector3D(iR,iR,iR),
                            QVector3D(ts->vertices[i*3], ts->vertices[i*3+1], ts->vertices[i*3+2]),
                            QVector3D(0,0,1));
                applyCSG('-', getTarnum()-2, getTarnum()-1);
                glMain()->deleteTar(getTarnum()-1);
                caved = true;
            }
        }
    }
}

void MainWindow_support::genTest_1(){//try lock
    genST();
    ThinStruct * ts = &var->ts;
    float iR=ts->innerR/50,oR=ts->outerR/50,nR=ts->nutR/50;
    for(int i=0;i<ts->vertices.size()/3 -1;i++){ // ****************** -1 on purpose
        putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,oR),
                    QVector3D(ts->vertices[i*3], ts->vertices[i*3+1], ts->vertices[i*3+2]),
                    QVector3D(0,0,1));
    }
    for(int i=0;i<ts->edges.size()/2;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
    }
    mergeAll(1, getTarnum()-1);

    /************************************************/
    glMain()->genSpiral(QVector3D( oR*50 + iR*50, oR*50,0), QVector3D(0,1,0) , oR*50 /1.41421f, 1, 10, 0, 45, 0.5f, 0.5f);
    applyCSG('+', getTarnum()-2, getTarnum()-1);
    glMain()->deleteTar(getTarnum()-1);

    glMain()->genSpiral(QVector3D( oR*50 + iR*50,-oR*50,0), QVector3D(0,-1,0), oR*50 /1.41421f, 1, 10, 0, 45, 0.5f, 0.5f);
    applyCSG('+', getTarnum()-2, getTarnum()-1);
    glMain()->deleteTar(getTarnum()-1);

    /************************************************/

    glMain()->copyObj(1);
    putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                QVector3D( 10.0f, 10.0f, 10.0f),
                QVector3D( oR*50 + iR*50 + 500,0,0),
                QVector3D(0,0,1));
    /*
    putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                QVector3D( iR, iR, 1.0f),
                QVector3D( oR*50 + iR*50,0,0),
                QVector3D(0,0,1));
    applyCSG('+', getTarnum()-2, getTarnum()-1);
    glMain()->deleteTar(getTarnum()-1);
    */

    applyCSG('-', 1, 3);
    applyCSG('*', 2, 3);
    applyCSG('-', 1, 0);
    applyCSG('-', 2, 0);
    glMain()->deleteTar(getTarnum()-1);

    /************************************************///var->ui->btnCut->click();
    for(int i=1;i<2;i++){
        QVector3D c = QVector3D(0,0,0);
        QVector3D n = QVector3D(0,0,1);

        int cur = i;
        int tar = glMain()->getTarnum();
        glMain()->copyObj(cur);

        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),QVector3D(10,10,1),c-QVector3D(0,0,50),n);
        applyCSG('-',cur,glMain()->getTarnum()-1);
        applyCSG('*',tar,glMain()->getTarnum()-1);
        glMain()->deleteTar(glMain()->getTarnum()-1);
    }
    for(int i=2;i<3;i++){
        QVector3D c = QVector3D(0,0,0);
        QVector3D n = QVector3D(0,0,1);

        int cur = i;
        int tar = glMain()->getTarnum();
        glMain()->copyObj(cur);

        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),QVector3D(10,1,10),c-QVector3D(0,50,0),n);
        applyCSG('-',cur,glMain()->getTarnum()-1);
        applyCSG('*',tar,glMain()->getTarnum()-1);
        glMain()->deleteTar(glMain()->getTarnum()-1);
    }
    /************************************************/
    float tr = oR;
    putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),QVector3D(tr,tr,tr),QVector3D( oR*50 + iR*50,0,oR*50+tr*50),QVector3D(0,0,1));
    glMain()->genSpiral(QVector3D( oR*50 + iR*50,0,oR*50), QVector3D(0,0,1), oR*50  /1.41421f , 1, 100, 90, 45, 0.5f, 0.5f);
    applyCSG('-',getTarnum()-2,getTarnum()-1);
    glMain()->deleteTar(glMain()->getTarnum()-1);

}

void MainWindow_support::genTest_2(){//try lock on nut
    genST();
    genTube();
    genNut();
    ThinStruct * ts = &var->ts;
    float iR=ts->innerR/50,oR=ts->outerR/50,nR=ts->nutR/50;
    glMain()->genSpiral(QVector3D( -nR*50/2, nR*50/4,0), QVector3D(-2,1,0) , oR*50, 1, 10, 0, 0, 0.5f, 0.5f);
    applyCSG('+', 4, getTarnum()-1);
    glMain()->deleteTar(getTarnum()-1);
}

void MainWindow_support::genTest_3()//a sample printing
{
    genST();
    ThinStruct * ts = &var->ts;
    float iR=ts->innerR/50,oR=ts->outerR/50,nR=ts->nutR/50;
    for(int i=0;i<ts->vertices.size()/3;i++){
        putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,oR),
                    QVector3D(ts->vertices[i*3], ts->vertices[i*3+1], ts->vertices[i*3+2]),
                    QVector3D(0,0,1));
    }
    for(int i=0;i<ts->edges.size()/2;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
    }
    mergeAll(1, getTarnum()-1);
    /**************************************************/
    applyCut(1,QVector3D(oR*50         ,0             ,0),QVector3D(1,  0,0));
    applyCut(1,QVector3D(0             ,oR*50         ,0),QVector3D(0,  1,0));
    applyCut(1,QVector3D(-oR*50/1.41421f,-oR*50/1.41421f,0),QVector3D(-1,-1,0));
    /**************************************************/
    putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                QVector3D(nR,nR,nR),
                QVector3D(0,0,0),
                QVector3D(0,0,1));
    /**************************************************/
    applyCSG('-', 5, 2);
    applyCSG('-', 5, 3);
    applyCSG('-', 5, 4);
    applyCSG('-', 1, 0);
    applyCSG('-', 2, 0);
    applyCSG('-', 3, 0);
    applyCSG('-', 4, 0);
    applyCSG('-', 5, 0);

}
/*real test*/
/*
void MainWindow_support::genTest_4()
{
    genST();
    //var->instanceEdges.push_back(std::set<int>());
    //var->instanceNuts.push_back(std::set<int>());
    //----------------------------------------------
    ThinStruct * ts = &var->ts;
    float mul = 50;
    float fix = 0.001f;
    float iR=ts->innerR/mul,oR=ts->outerR/mul,nR=ts->nutR/mul,ioR = (iR+oR)/2;
    int vs = ts->vertices.size()/3;
    //----------------------------------------------
    for(int i=0;i<ts->vertices.size()/3;i++){
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,oR),
                    QVector3D(ts->vertices[i*3], ts->vertices[i*3+1], ts->vertices[i*3+2]),
                    QVector3D(0,0,1));
    }
    for(int i=0;i<ts->edges.size()/2;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
        fixRotateMatrix(v2-v1,getTarnum()-1);
    }
    mergeAll(1, getTarnum()-1);

    //var->instanceEdges.push_back(std::set<int>());
    //var->instanceNuts.push_back(std::set<int>());
    //for(int i=0;i<ts->edges.size()/2;i++)var->instanceEdges[1].insert(i);
    //for(int i=0;i<ts->vertices.size()/3;i++)var->instanceNuts[1].insert(i);

    //----------------------------------------------
    int * nutInstance = (int*)malloc(vs*sizeof(int));
    for(int i=0;i<vs;i++)nutInstance[i]=1;
    for(int i=0;i<vs;i++){
        int thisvert = i;
        for(auto edge : ts->verticesedges[thisvert]){
            int thatvert = i == ts->edges[edge*2]?ts->edges[edge*2+1]:ts->edges[edge*2];
            if(ts->forceLink[thisvert].count(edge)==0){
                QVector3D v1 = ts->getVertice(thisvert);
                QVector3D v2 = ts->getVertice(thatvert);
                QVector3D n = (v2-v1).normalized();
                QVector3D p = ts->getVertice(thisvert)+oR*mul*n;
                int cur = nutInstance[thisvert];
                int tar = getTarnum();
                if(ts->setting[2]>0){
                    glMain()->genSpiral(p+QVector3D(0,0,-oR*mul), -1*QUP, oR*mul/1.41421f, 1, 10, 0, 45, 0.5f, 0.5f);
                    applyCSG('+',cur, getTarnum()-1);deleteLastTar();
                }

                applyCut(cur,p,n);

                if(ts->setting[1]>0){
                    float sy;
                    if(ts->setting[1]==1)sy = ioR;
                    else if(ts->setting[1]==2)sy = oR+fix;
                    putStdModel("cube10X10", QGRAY,QVector3D(ioR-fix,sy,ioR),p,n);
                    fixRotateMatrix(n,getTarnum()-1);
                    applyCSG('+',cur,getTarnum()-1);glMain()->deleteTar(getTarnum()-1);

                    putStdModel("cube10X10", QGRAY,QVector3D(ioR+fix,sy,ioR),p,n);
                    fixRotateMatrix(n,getTarnum()-1);
                    applyCSG('-',tar,getTarnum()-1);glMain()->deleteTar(getTarnum()-1);
                }

                //var->instanceEdges.push_back(std::set<int>());
                //var->instanceNuts.push_back(std::set<int>());
                bool * visited = (bool*)malloc(vs*sizeof(bool));memset(visited,0,sizeof(visited));
                for(int j = 0 ; j < vs ; j++)if(nutInstance[j]!=cur)visited[j] = true;
                visited[thisvert]  = true;
                //std::set<int> etcl;//edge to-change list
                //etcl.insert(ts->edgesmap[thisvert][thatvert]);
                std::queue<int> queue;
                queue.push(thatvert);
                while(queue.size()>0){
                    int now = queue.front();queue.pop();
                    visited[now] = true;
                    nutInstance[now] = tar;
                    //var->instanceNuts[cur].erase(now);var->instanceNuts[tar].insert(now);
                    for(auto next : ts->neighborset[now]){
                        if(!visited[next]){
                            queue.push(next);
                            //etcl.insert(ts->edgesmap[now][next]);
                        }
                    }
                }
                //for(auto edge : etcl){
                //    var->instanceEdges[cur].erase(edge);
                //    var->instanceEdges[tar].insert(edge);
                //}
            }
        }
    }
    //----------------------------------------------
    genTest_5();
}
*/
void MainWindow_support::genTest_4()
{
    genST();
    //var->instanceEdges.push_back(std::set<int>());
    //var->instanceNuts.push_back(std::set<int>());
    //----------------------------------------------
    ThinStruct * ts = &var->ts;
    float mul = 50;
    float fix = 0.001f;
    float iR=ts->innerR/mul,oR=ts->outerR/mul,nR=ts->nutR/mul,ioR = (iR+oR)/2;
    int vs = ts->vertices.size()/3;
    int es = ts->edges.size()/2;
    float sy;
    if(ts->setting[1]==1)sy = ioR;
    else if(ts->setting[1]==2)sy = oR+fix;
    //----------------------------------------------
    int * edgeInstance = (int*)malloc(es*sizeof(int));
    memset(edgeInstance,0,sizeof(int)*es);
    //for(int i=0;i<es;i++)edgeInstance[i]=0;
    std::set<int> toDelete;
    for(int i=0;i<vs;i++){
        int thisvert = i;
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,oR+fix/2),
                    QVector3D(ts->getVertice(thisvert)),
                    QVector3D(0,0,1));
        int thisvertInstance = getTarnum()-1;
        for(auto edge : ts->verticesedges[thisvert]){
            int thatvert = thisvert == ts->edges[edge*2]?ts->edges[edge*2+1]:ts->edges[edge*2];
            std::cout << thisvert << " : " << thatvert << " ( " << edge << std::endl;
            QVector3D v1 = ts->getVertice(thisvert);
            QVector3D v2 = ts->getVertice(thatvert);
            QVector3D n = (v2-v1).normalized();
            QVector3D p1 = ts->getVertice(thisvert)+oR*mul*n;
            QVector3D p2 = ts->getVertice(thisvert)+((v2-v1).length()-oR*mul)*n;
            putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,(v1-v2).length()/100),
                        (v1+v2)/2,
                        v2-v1);
            int columnInstance = getTarnum()-1;
            fixRotateMatrix(v2-v1,columnInstance);
            if(ts->forceLink[thisvert].count(edge)==0){
                applyCut(columnInstance,p1,n);deleteLastTar();
                applyCSG('+',thisvertInstance, columnInstance);
                deleteTar(columnInstance);
                //edgeInstance[edge] = 0;
                std::cout << "is a cut" << std::endl;

                if(ts->setting[1]>0){
                    putStdModel("cube10X10", QGRAY,QVector3D(ioR-fix,sy,ioR),p1,n);
                    fixRotateMatrix(n,getTarnum()-1);
                    applyCSG('+',thisvertInstance,getTarnum()-1);deleteLastTar();
                }

            }else{
                if(edgeInstance[edge]==thisvertInstance)deleteTar(columnInstance);
                else if(edgeInstance[edge]!=0){
                    deleteTar(columnInstance);
                    applyCSG('+',edgeInstance[edge], thisvertInstance);
                    for(int j=0;j<es;j++)if(edgeInstance[j]==thisvertInstance)edgeInstance[j]=edgeInstance[edge];
                    toDelete.insert(thisvertInstance);
                    thisvertInstance = edgeInstance[edge];
                    std::cout << "is a merge" << std::endl;
                }else if(edgeInstance[edge]==0){
                    if(ts->forceLink[thatvert].count(edge)==0){
                        applyCut(columnInstance,p2,n);deleteLastTar();
                        applyCSG('+',thisvertInstance, columnInstance);
                        deleteTar(columnInstance);
                        edgeInstance[edge] = thisvertInstance;
                        std::cout << "is a new short" << std::endl;

                        if(ts->setting[1]>0){
                            putStdModel("cube10X10", QGRAY,QVector3D(ioR+fix,sy,ioR),p2,-n);
                            fixRotateMatrix(-n,getTarnum()-1);
                            applyCSG('-',thisvertInstance,getTarnum()-1);deleteLastTar();
                        }

                    }else{
                        applyCSG('+',thisvertInstance, columnInstance);
                        deleteTar(columnInstance);
                        edgeInstance[edge] = thisvertInstance;
                        std::cout << "is a new long" << std::endl;
                    }
                }
            }
        }
    }
    while(toDelete.size()>0){
        int maxidx = FLT_MIN;
        for(auto idx : toDelete){
            if(idx>maxidx)maxidx = idx;
        }
        deleteTar(maxidx);
        toDelete.erase(maxidx);
    }
    for(int i=0;i<es;i++){
        if(edgeInstance[i]==0){
            QVector3D v1 = ts->getVertice(ts->edges[i*2]);
            QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
            QVector3D n = (v2-v1).normalized();
            QVector3D p = (v2+v1)/2;
            putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,(v1-v2).length()/100-2*oR),
                        p, n);
            fixRotateMatrix(v2-v1,getTarnum()-1);
            edgeInstance[i] = getTarnum()-1;
        }
    }
    //----------------------------------------------
    genTest_5();
}
void MainWindow_support::genTest_5(){
    int tarnum = getTarnum();
    for(int i=1;i<tarnum;i++){
        glMain()->copyObj(i);
    }
    /**************************/
    ThinStruct * ts = &var->ts;
    float mul = 50;
    float iR=ts->innerR/mul,oR=ts->outerR/mul,nR=ts->nutR/mul,ioR = (iR+oR)/2;
    float fix = 0.001f;
    /**************************/
    /*
    for(int i=1;i<var->instanceEdges.size();i++){
        int from = getTarnum();
        for(auto nut : var->instanceNuts[i]){
            putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(ioR,ioR,oR+fix),
                        QVector3D(ts->vertices[nut*3], ts->vertices[nut*3+1], ts->vertices[nut*3+2]),
                        QVector3D(0,0,1));
        }
        for(auto edge : var->instanceEdges[i]){
            QVector3D v1 = ts->getVertice(ts->edges[edge*2]);
            QVector3D v2 = ts->getVertice(ts->edges[edge*2+1]);
            putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(ioR,oR+fix,(v1-v2).length()/100),
                        (v1+v2)/2,
                        v2-v1);
            int tarobj = getTarnum()-1;
            QMatrix4x4 rot = getRotateMatrix(QVector3D(0,0,1),v2-v1);
            QVector3D axis = QVector3D(0,0,1) * rot;
            QVector3D forw = QVector3D(0,1,0) * rot;
            QVector3D tarv = QVector3D(0,0,-1);
            QMatrix4x4 rot2 = getRotateMatrix(axis, forw, tarv);
            getModel(tarobj)->rotate(rot2);
            updateModel(tarobj);
        }
        int to = getTarnum()-1;
        mergeAll(from, to);
        applyCut(getTarnum()-1,QZERO,QUP);
        glMain()->deleteTar(getTarnum()-1);
    }
    int compNum = (getTarnum()-1)/3;
    for(int i=1;i<=compNum;i++){
        applyCSG('*',i,i+2*compNum);
        applyCSG('-',i+compNum,i+2*compNum);
    }
    */
    /**************************/
    int from = getTarnum();
    for(int i=0;i<ts->vertices.size()/3;i++){
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(ioR,ioR,oR+fix),
                    QVector3D(ts->vertices[i*3], ts->vertices[i*3+1], ts->vertices[i*3+2]),
                    QVector3D(0,0,1));
    }
    for(int i=0;i<ts->edges.size()/2;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(ioR,oR+fix,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
        fixRotateMatrix(v2-v1,getTarnum()-1);
    }
    int to = getTarnum()-1;
    mergeAll(from, to);
    applyCut(getTarnum()-1,QZERO,QUP);
    glMain()->deleteTar(getTarnum()-1);
    /**************************/
    if(ts->setting[0]>0){
        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(100,100,1),
                    QVector3D(0,0,-mul-ioR*mul),
                    QUP);
        applyCSG('+',getTarnum()-2,getTarnum()-1);deleteLastTar();
    }

    int compNum = (getTarnum()-1-1)/2;
    for(int i=1;i<=compNum;i++){
        applyCSG('*',i        ,1+2*compNum);
        applyCSG('-',i+compNum,1+2*compNum);
    }
    glMain()->deleteTar(getTarnum()-1);
    for(int i=getTarnum()-1;i>0;i--){
        applyCSG('-',i,0);
    }
    /**************************/
    if(ts->setting[2]>0){
        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,oR),
                    QVector3D(0,0,-oR*2*mul),
                    -1*QUP);
        glMain()->genSpiral(QVector3D(0,0,-oR*mul), -1*QUP, oR*mul/1.41421f, 1, 10, 0, 45, 0.5f, 0.5f);
        applyCSG('-',getTarnum()-2, getTarnum()-1);deleteLastTar();
    }
}

void MainWindow_support::genTest_6()
{
    //----------------------------------------------
    genST();
    if(var->ts.setting[3]==1){
        genTest_6_2();
        return;
    }
    //----------------------------------------------
    ThinStruct * ts = &var->ts;
    float mul = 50;
    float fix = 0.001f;
    float iR=ts->innerR/mul,oR=ts->outerR/mul,nR=ts->nutR/mul,ioR = (iR+oR)/2;
    int vs = ts->vertices.size()/3;
    int es = ts->edges.size()/2;
    float sy;
    if(ts->setting[1]==1)sy = ioR;
    else if(ts->setting[1]==2)sy = oR+fix;
    //----------------------------------------------
    int * edgeInstance = (int*)malloc(es*sizeof(int));
    memset(edgeInstance,0,sizeof(int)*es);
    //for(int i=0;i<es;i++)edgeInstance[i]=0;
    std::set<int> toDelete;
    for(int i=0;i<vs;i++){
        int thisvert = i;
        putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,oR),
                    QVector3D(ts->getVertice(thisvert)),
                    QVector3D(0,0,1));
        int thisvertInstance = getTarnum()-1;
        for(auto edge : ts->verticesedges[thisvert]){
            int thatvert = thisvert == ts->edges[edge*2]?ts->edges[edge*2+1]:ts->edges[edge*2];
            std::cout << thisvert << " : " << thatvert << " ( " << edge << std::endl;
            QVector3D v1 = ts->getVertice(thisvert);
            QVector3D v2 = ts->getVertice(thatvert);
            QVector3D n = (v2-v1).normalized();
            QVector3D p1 = ts->getVertice(thisvert)+oR*mul*n;
            QVector3D p2 = ts->getVertice(thisvert)+((v2-v1).length()-oR*mul)*n;
            putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,(v1-v2).length()/100),
                        (v1+v2)/2,
                        v2-v1);
            int columnInstance = getTarnum()-1;
            QVector3D down = -1*ts->getSplitNorm(edge);
            fixRotateMatrix(v2-v1,down,columnInstance);
            if(ts->forceLink[thisvert].count(edge)==0){
                applyCut(columnInstance,p1,n);deleteLastTar();
                applyCSG('+',thisvertInstance, columnInstance);
                deleteTar(columnInstance);
                //edgeInstance[edge] = 0;
                std::cout << "is a cut" << std::endl;

                if(ts->setting[1]==1){
                    putStdModel("cylinder10X10", QGRAY,QVector3D(ioR-fix,sy,ioR),p1,n);
                    fixRotateMatrix(n,down,getTarnum()-1);
                    applyCSG('+',thisvertInstance,getTarnum()-1);deleteLastTar();
                }else if(ts->setting[1]==2){
                    putStdModel("cube10X10", QGRAY,QVector3D(ioR-fix,sy,ioR),p1,n);
                    fixRotateMatrix(n,down,getTarnum()-1);
                    applyCSG('+',thisvertInstance,getTarnum()-1);deleteLastTar();
                }

            }else{
                if(edgeInstance[edge]==thisvertInstance)deleteTar(columnInstance);
                else if(edgeInstance[edge]!=0){
                    deleteTar(columnInstance);
                    applyCSG('+',edgeInstance[edge], thisvertInstance);
                    for(int j=0;j<es;j++)if(edgeInstance[j]==thisvertInstance)edgeInstance[j]=edgeInstance[edge];
                    toDelete.insert(thisvertInstance);
                    thisvertInstance = edgeInstance[edge];
                    std::cout << "is a merge" << std::endl;
                }else if(edgeInstance[edge]==0){
                    if(ts->forceLink[thatvert].count(edge)==0){
                        applyCut(columnInstance,p2,n);deleteLastTar();
                        applyCSG('+',thisvertInstance, columnInstance);
                        deleteTar(columnInstance);
                        edgeInstance[edge] = thisvertInstance;
                        std::cout << "is a new short" << std::endl;

                        if(ts->setting[1]==1){
                            putStdModel("cylinder10X10", QGRAY,QVector3D(ioR+fix,sy,ioR),p2,-n);
                            fixRotateMatrix(-n,down,getTarnum()-1);
                            applyCSG('-',thisvertInstance,getTarnum()-1);deleteLastTar();
                        }else if(ts->setting[1]==2){
                            putStdModel("cube10X10", QGRAY,QVector3D(ioR-fix,sy,ioR),p2,-n);
                            fixRotateMatrix(-n,down,getTarnum()-1);
                            applyCSG('-',thisvertInstance,getTarnum()-1);deleteLastTar();
                        }

                    }else{
                        applyCSG('+',thisvertInstance, columnInstance);
                        deleteTar(columnInstance);
                        edgeInstance[edge] = thisvertInstance;
                        std::cout << "is a new long" << std::endl;
                    }
                }
            }
        }
    }
    while(toDelete.size()>0){
        int maxidx = FLT_MIN;
        for(auto idx : toDelete){
            if(idx>maxidx)maxidx = idx;
        }
        deleteTar(maxidx);
        toDelete.erase(maxidx);
    }
    for(int i=0;i<es;i++){
        if(edgeInstance[i]==0){
            QVector3D v1 = ts->getVertice(ts->edges[i*2]);
            QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
            QVector3D n = (v2-v1).normalized();
            QVector3D p = (v2+v1)/2;
            putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,(v1-v2).length()/100-2*oR),
                        p, n);
            fixRotateMatrix(v2-v1,getTarnum()-1);
            edgeInstance[i] = getTarnum()-1;
        }
    }
    //----------------------------------------------
    genTest_7();
    /*
    var->edgeInstance = (int*)malloc(2*es*sizeof(int));memset(var->edgeInstance,0,sizeof(int)*es*2);
    std::set<int> toDelete;
    for(int i=0;i<es;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        QVector3D dir = -1*ts->getSplitNorm(i);
        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR+fix,oR+fix,(v1-v2).length()/100+fix),
                    (v1+v2)/2,
                    v2-v1);
        fixRotateMatrix(v2-v1, dir,getTarnum()-1);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR+fix,oR+fix,oR+fix),
                    v1,
                    dir);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR+fix,oR+fix,oR+fix),
                    v2,
                    dir);
        mergeAll(getTarnum()-3, getTarnum()-1);
        var->edgeInstance[i] = getTarnum()-1;
    }
    for(int i=0;i<es;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        QVector3D dir = -1*ts->getSplitNorm(i);
        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
        fixRotateMatrix(v2-v1, dir,getTarnum()-1);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,oR),
                    v1,
                    dir);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,oR),
                    v2,
                    dir);
        mergeAll(getTarnum()-3, getTarnum()-1);
        var->edgeInstance[i+es] = getTarnum()-1;
    }
    for(int i=0;i<es;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        applyCut(var->edgeInstance[i],(v1+v2)/2,ts->getSplitNorm(i));
        deleteLastTar();
    }
    //----------------------------------------------
    for(int i=0;i<vs;i++){
        std::vector<int> edgeList;
        for(auto edge : ts->forceLink[i]){
            edgeList.push_back(edge);
        }
        int mainInstance = var->edgeInstance[edgeList[0]];
        for(int j=1;j<edgeList.size();j++){
            int instIdx = edgeList[j];
            int oldInst = var->edgeInstance[instIdx];
            applyCSG('+',mainInstance,oldInst);
            if(oldInst!=mainInstance)toDelete.insert(oldInst);
            for(int k=0;k<es;k++)if(var->edgeInstance[k]==oldInst)var->edgeInstance[k]=mainInstance;
        }

        mainInstance = var->edgeInstance[edgeList[0]+es];
        for(int j=1;j<edgeList.size();j++){
            int instIdx = edgeList[j]+es;
            int oldInst = var->edgeInstance[instIdx];
            applyCSG('+',mainInstance,oldInst);
            if(oldInst!=mainInstance)toDelete.insert(oldInst);
            for(int k=es;k<es+es;k++)if(var->edgeInstance[k]==oldInst)var->edgeInstance[k]=mainInstance;
        }
    }
    //----------------------------------------------
    while(toDelete.size()>0){
        int maxidx = FLT_MIN;
        for(auto idx : toDelete){
            if(idx>maxidx)maxidx = idx;
        }
        deleteTar(maxidx);
        toDelete.erase(maxidx);
    }
    //----------------------------------------------
    int compNum = (getTarnum()-1)/2;
    for(int i=1;i<=compNum;i++){
        applyCSG('-',i+compNum,compNum);
    }
    //----------------------------------------------
    */
}
void MainWindow_support::genTest_6_2()
{
    //----------------------------------------------
    ThinStruct * ts = &var->ts;
    float mul = 50;
    float fix = 0.001f;
    float iR=ts->innerR/mul,oR=ts->outerR/mul,nR=ts->nutR/mul,ioR = (iR+oR)/2;
    int vs = ts->vertices.size()/3;
    int es = ts->edges.size()/2;
    float sy;
    if(ts->setting[1]==1)sy = ioR;
    else if(ts->setting[1]==2)sy = oR+fix;
    //----------------------------------------------
    int * edgeInstance = (int*)malloc(es*sizeof(int));
    memset(edgeInstance,0,sizeof(int)*es);
    //for(int i=0;i<es;i++)edgeInstance[i]=0;
    std::set<int> toDelete;
    for(int i=0;i<vs;i++){
        int thisvert = i;
        if(ts->verticesedges[thisvert].size()==1){
            putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,oR),
                        ts->getVertice(thisvert),
                        -1*ts->getSplitNorm(*ts->verticesedges[thisvert].begin()));
        }else{
            putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,oR),
                        ts->getVertice(thisvert),
                        QVector3D(0,0,1));
        }
        int thisvertInstance = getTarnum()-1;
        for(auto edge : ts->verticesedges[thisvert]){
            int thatvert = thisvert == ts->edges[edge*2]?ts->edges[edge*2+1]:ts->edges[edge*2];
            std::cout << thisvert << " : " << thatvert << " ( " << edge << std::endl;
            QVector3D v1 = ts->getVertice(thisvert);
            QVector3D v2 = ts->getVertice(thatvert);
            QVector3D n = (v2-v1).normalized();
            QVector3D p1 = ts->getVertice(thisvert)+oR*mul*n;
            QVector3D p2 = ts->getVertice(thisvert)+((v2-v1).length()-oR*mul)*n;
            putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,(v1-v2).length()/100),
                        (v1+v2)/2,
                        v2-v1);
            int columnInstance = getTarnum()-1;
            QVector3D down = -1*ts->getSplitNorm(edge);
            fixRotateMatrix(v2-v1,down,columnInstance);
            if(ts->forceLink[thisvert].count(edge)==0){
                //applyCut(columnInstance,p1,n);deleteLastTar();
                applyCSG('+',thisvertInstance, columnInstance);
                applyCut(thisvertInstance,p1,n);deleteLastTar();
                deleteTar(columnInstance);
                //edgeInstance[edge] = 0;
                std::cout << "is a cut" << std::endl;

                if(ts->setting[1]==1){
                    putStdModel("cube10X10", QGRAY,QVector3D(ioR-fix,sy,ioR),p1,n);
                    fixRotateMatrix(n,down,getTarnum()-1);
                    applyCSG('+',thisvertInstance,getTarnum()-1);deleteLastTar();
                }else if(ts->setting[1]==2){
                    putStdModel("cube10X10", QGRAY,QVector3D(ioR-fix,sy,ioR),p1,n);
                    fixRotateMatrix(n,down,getTarnum()-1);
                    applyCSG('+',thisvertInstance,getTarnum()-1);deleteLastTar();
                }

            }else{
                if(edgeInstance[edge]==thisvertInstance)deleteTar(columnInstance);
                else if(edgeInstance[edge]!=0){
                    deleteTar(columnInstance);
                    applyCSG('+',edgeInstance[edge], thisvertInstance);
                    for(int j=0;j<es;j++)if(edgeInstance[j]==thisvertInstance)edgeInstance[j]=edgeInstance[edge];
                    toDelete.insert(thisvertInstance);
                    thisvertInstance = edgeInstance[edge];
                    std::cout << "is a merge" << std::endl;
                }else if(edgeInstance[edge]==0){
                    if(ts->forceLink[thatvert].count(edge)==0){
                        applyCut(columnInstance,p2,n);deleteLastTar();
                        applyCSG('+',thisvertInstance, columnInstance);
                        deleteTar(columnInstance);
                        edgeInstance[edge] = thisvertInstance;
                        std::cout << "is a new short" << std::endl;

                        if(ts->setting[1]==1){
                            putStdModel("cube10X10", QGRAY,QVector3D(ioR+fix,sy,ioR),p2,-n);
                            fixRotateMatrix(-n,down,getTarnum()-1);
                            applyCSG('-',thisvertInstance,getTarnum()-1);deleteLastTar();
                        }else if(ts->setting[1]==2){
                            putStdModel("cube10X10", QGRAY,QVector3D(ioR-fix,sy,ioR),p2,-n);
                            fixRotateMatrix(-n,down,getTarnum()-1);
                            applyCSG('-',thisvertInstance,getTarnum()-1);deleteLastTar();
                        }

                    }else{
                        applyCSG('+',thisvertInstance, columnInstance);
                        deleteTar(columnInstance);
                        edgeInstance[edge] = thisvertInstance;
                        std::cout << "is a new long" << std::endl;
                    }
                }
            }
        }
    }
    while(toDelete.size()>0){
        int maxidx = FLT_MIN;
        for(auto idx : toDelete){
            if(idx>maxidx)maxidx = idx;
        }
        deleteTar(maxidx);
        toDelete.erase(maxidx);
    }
    for(int i=0;i<es;i++){
        if(edgeInstance[i]==0){
            QVector3D v1 = ts->getVertice(ts->edges[i*2]);
            QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
            QVector3D n = (v2-v1).normalized();
            QVector3D p = (v2+v1)/2;
            putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,(v1-v2).length()/100-2*oR),
                        p, n);
            fixRotateMatrix(v2-v1,getTarnum()-1);
            edgeInstance[i] = getTarnum()-1;
        }
    }
    //----------------------------------------------
    genTest_7();
}
void MainWindow_support::genTest_7(){
    //----------------------------------------------
    ThinStruct * ts = &var->ts;
    float mul = 50;
    float fix = 0.0001f;
    float iR=ts->innerR/mul,oR=ts->outerR/mul,nR=ts->nutR/mul,ioR = (iR+oR)/2;
    int vs = ts->vertices.size()/3;
    int es = ts->edges.size()/2;
    float sy;
    if(ts->setting[1]==1)sy = ioR;
    else if(ts->setting[1]==2)sy = oR+fix;
    //----------------------------------------------
    int tarnum=getTarnum();
    for(int i=1;i<tarnum;i++){
        glMain()->copyObj(i);
    }
    int flag = getTarnum();
    var->edgeInstance = (int*)malloc(2*es*sizeof(int));memset(var->edgeInstance,0,sizeof(int)*es*2);
    for(int i=0;i<es;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        QVector3D dir = -1*ts->getSplitNorm(i);
        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(ioR,oR+fix,(v1-v2).length()/100+fix),
                    (v1+v2)/2,
                    v2-v1);
        fixRotateMatrix(v2-v1, dir,getTarnum()-1);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(ioR,ioR,oR+fix),
                    v1,
                    dir);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(ioR,ioR,oR+fix),
                    v2,
                    dir);
        mergeAll(getTarnum()-3, getTarnum()-1);
        if(ts->setting[0]>0){
            float th = ts->setting[0]/mul;
            putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR+fix,th/2+fix,(v1-v2).length()/100+fix),
                        (v1+v2)/2+dir.normalized()*(oR-th/2)*mul,
                        v2-v1);
            fixRotateMatrix(v2-v1, dir,getTarnum()-1);
            putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR+fix,oR+fix,th/2+fix),
                        v1+dir.normalized()*(oR-th/2)*mul,
                        dir);
            putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR+fix,oR+fix,th/2+fix),
                        v2+dir.normalized()*(oR-th/2)*mul,
                        dir);
            mergeAll(getTarnum()-4, getTarnum()-1);
        }
        var->edgeInstance[i] = getTarnum()-1;
    }
    for(int i=0;i<es;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        applyCut(var->edgeInstance[i],(v1+v2)/2,ts->getSplitNorm(i));
        deleteLastTar();
    }
    //----------------------------------------------
    mergeAll(flag, getTarnum()-1);
    int instNum = (getTarnum()-2)/2;
    for(int i=1;i<=instNum;i++){
        applyCSG('-',i        ,getTarnum()-1);
        applyCSG('*',i+instNum,getTarnum()-1);
    }
    //----------------------------------------------
    deleteLastTar();

    //for(int i=1;i<getTarnum();i++){
    //    applyCSG('-',i,0);
    //}

}
