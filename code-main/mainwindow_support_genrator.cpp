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
    readST();
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
    readST();
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
    readST();
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
void MainWindow_support::genTest_4()
{
    readST();
    //var->instanceEdges.push_back(std::set<int>());
    //var->instanceNuts.push_back(std::set<int>());
    /**************************/
    ThinStruct * ts = &var->ts;
    float mul = 50;
    float fix = 0.001f;
    float iR=ts->innerR/mul,oR=ts->outerR/mul,nR=ts->nutR/mul,ioR = (iR+oR)/2;
    int vs = ts->vertices.size()/3;
    /**************************/
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

    /**************************/
    int nutInstance[vs];
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
                bool visited[vs];memset(visited,0,sizeof(visited));
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
    /**************************/
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
    putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                QVector3D(oR,oR,oR),
                QVector3D(0,0,-oR*2*mul),
                -1*QUP);
    glMain()->genSpiral(QVector3D(0,0,-oR*mul), -1*QUP, oR*mul/1.41421f, 1, 10, 0, 45, 0.5f, 0.5f);
    applyCSG('-',getTarnum()-2, getTarnum()-1);deleteLastTar();
}
