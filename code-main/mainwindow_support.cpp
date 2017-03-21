#include "mainwindow_support.h"

ModelManager* MainWindow_support::getModel(int tar){
    return &(ui->glMain->viewMgr->modelMgr[tar]);
}

void MainWindow_support::putStdModel(const char * name,QVector3D color, QVector3D scale, QVector3D translate, QVector3D rotate){
    char file[30];
    sprintf(file,":/object/%s.obj",name);
    QFile stdObjFile(file);
    ui->glMain->load_rc(&stdObjFile);
    var->connectTarNum = ui->glMain->getTarnum()-1;
    ui->glMain->setColor(var->connectTarNum,color.x(),color.y(),color.z());
    getModel(var->connectTarNum)->ResetModel();
    //getModel(var->connectTarNum)->SetScale(scale.x(),scale.y(),scale.z());
    getModel(var->connectTarNum)->scaleDepend_ori(QVector3D(0,0,0),scale.x(),scale.y(),scale.z());
    getModel(var->connectTarNum)->translate_pure(translate);
    getModel(var->connectTarNum)->rotateTo(rotate);
    ui->glMain->setVis(var->connectTarNum,1);
    getModel(var->connectTarNum)->applyModelMatrix_force();
    ui->glMain->reBuffer(var->connectTarNum);
}

void MainWindow_support::applyCSG(char c, int er, int ee){
    iglMachine.reset();
    iglMachine.put("mainobj", getModel(er)->vertices, getModel(er)->indices);
    iglMachine.put("conn", getModel(ee)->vertices, getModel(ee)->indices);
    iglMachine.command("NEW outcome");
    char str[50];
    sprintf(str,"%c outcome mainobj conn",c);
    iglMachine.command(str);
    iglMachine.get("outcome",getModel(er)->vertices_ori, getModel(er)->indices);
    getModel(er)->ResetModel();
    getModel(er)->refresh();
    ui->glMain->reBuffer(er);
}

void MainWindow_support::stateInit(){
    //deleteSkelSup();
    for(int i=ui->glMain->getTarnum()-1;i>=0;i--){
        ui->glMain->deleteTar(i);
    }
    for(int i=ui->glMain->getTarnum_ass()-1;i>=0;i--){
        ui->glMain->deleteTar_ass(i);
    }
    //var->skelMgr.Reset();
    ui->cmbbObject->clear();

}

void MainWindow_support::fill(int tar){
    std::string mstr = cgaltool.fillHoleAndGetStr(getModel(var->tarObj)->vertices_ori, getModel(var->tarObj)->indices);
    cgaltool.readFromOFFStream(getModel(var->tarObj)->vertices_ori, getModel(var->tarObj)->indices,mstr);
    mstr.clear();
    getModel(tar)->applyModelMatrix_force();
    getModel(tar)->refresh();
}

void MainWindow_support::readST(){
    ThinStruct * ts = &var->ts;
    ts->read("TSEX.txt");
    float iR=ts->innerR/50,oR=ts->outerR/50,nR=ts->nutR/50;
    for(int i=0;i<ts->vertices.size()/3;i++){
        putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(iR,iR,iR),
                    QVector3D(ts->vertices[i*3], ts->vertices[i*3+1], ts->vertices[i*3+2]),
                    QVector3D(0,0,1));
    }
    for(int i=0;i<ts->edges.size()/2;i++){
        QVector3D v1 = ts->getVectice(ts->edges[i*2]);
        QVector3D v2 = ts->getVectice(ts->edges[i*2+1]);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(iR,iR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
    }
    mergeAll();
}

void MainWindow_support::mergeAll(){
    int tarnum;
    while((tarnum=getTarNum())>1){
        for(int i=0;i<tarnum/2;i++){
            applyCSG('+', i, i+(tarnum+1)/2);
        }
        for(int i=tarnum-1; i>=(tarnum+1)/2; i--){
            ui->glMain->deleteTar(i);
        }
    }
}

void MainWindow_support::mergeAll(int from, int to){
    while(from<to){
        for(int i=from;i<from+(to-from+1)/2;i++){
            applyCSG('+', i, i+(to-from+1+1)/2);
        }
        for(int i=to; i>=from+(to-from+1+1)/2; i--){
            ui->glMain->deleteTar(i);
        }
        to = from+(to-from+1+1)/2-1;
    }
}

void MainWindow_support::genTube(){
    ThinStruct * ts = &var->ts;
    float iR=ts->innerR/50,oR=ts->outerR/50,nR=ts->nutR/50;
    for(int i=0;i<ts->edges.size()/2;i++){
        QVector3D v1 = ts->getVectice(ts->edges[i*2]);
        QVector3D v2 = ts->getVectice(ts->edges[i*2+1]);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,(v1-v2).length()/100-oR),
                    (v1+v2)/2,
                    v2-v1);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(iR,iR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
        applyCSG('-', getTarNum()-2, getTarNum()-1);
        ui->glMain->deleteTar(getTarNum()-1);
        /*
        int cur = getTarNum()-1;
        int tar = getTarNum();
        ui->glMain->copyObj(cur);
        ui->glMain->genSlotCav((v1+v2)/2, v2-v1, QVector3D(0,0,1));
        applyCSG('-',cur,getTarNum()-1);
        applyCSG('*',tar,getTarNum()-1);
        ui->glMain->deleteTar(getTarNum()-1);
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
            QVector3D v1 = ts->getVectice(i);
            QVector3D v2 = ts->getVectice(idx);
            putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(iR,iR,(v1-v2).length()/100),
                        (v1+v2)/2,
                        v2-v1);
            applyCSG('-', getTarNum()-2, getTarNum()-1);
            ui->glMain->deleteTar(getTarNum()-1);

            putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                        QVector3D(oR,oR,(v1-v2).length()/100-oR),
                        (v1+v2)/2,
                        v2-v1);
            applyCSG('-', getTarNum()-2, getTarNum()-1);
            ui->glMain->deleteTar(getTarNum()-1);

            if(!caved){
                putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                            QVector3D(iR,iR,iR),
                            QVector3D(ts->vertices[i*3], ts->vertices[i*3+1], ts->vertices[i*3+2]),
                            QVector3D(0,0,1));
                applyCSG('-', getTarNum()-2, getTarNum()-1);
                ui->glMain->deleteTar(getTarNum()-1);
                caved = true;
            }
        }
    }
}
void MainWindow_support::genCut(int tar, QVector3D c, QVector3D n){
    ui->glMain->copyObj(tar);
    putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                QVector3D( 10, 10, 10),
                c+QVector3D( 0, 0, 500),
                n);
    applyCSG('-', tar, getTarNum()-1);
    applyCSG('*', getTarNum()-2, getTarNum()-1);
    ui->glMain->deleteTar(getTarNum()-1);

}

void MainWindow_support::genTest_1(){
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
        QVector3D v1 = ts->getVectice(ts->edges[i*2]);
        QVector3D v2 = ts->getVectice(ts->edges[i*2+1]);
        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
    }
    mergeAll(1, getTarNum()-1);

    /************************************************/
    ui->glMain->genSpiral(QVector3D( oR*50 + iR*50, oR*50,0), QVector3D(0,1,0) , oR*50 /1.41421f, 1, 10, 0, 45, 0.5f, 0.5f);
    applyCSG('+', getTarNum()-2, getTarNum()-1);
    ui->glMain->deleteTar(getTarNum()-1);

    ui->glMain->genSpiral(QVector3D( oR*50 + iR*50,-oR*50,0), QVector3D(0,-1,0), oR*50 /1.41421f, 1, 10, 0, 45, 0.5f, 0.5f);
    applyCSG('+', getTarNum()-2, getTarNum()-1);
    ui->glMain->deleteTar(getTarNum()-1);

    /************************************************/

    ui->glMain->copyObj(1);
    putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                QVector3D( 10.0f, 10.0f, 10.0f),
                QVector3D( oR*50 + iR*50 + 500,0,0),
                QVector3D(0,0,1));
    /*
    putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                QVector3D( iR, iR, 1.0f),
                QVector3D( oR*50 + iR*50,0,0),
                QVector3D(0,0,1));
    applyCSG('+', getTarNum()-2, getTarNum()-1);
    ui->glMain->deleteTar(getTarNum()-1);
    */

    applyCSG('-', 1, 3);
    applyCSG('*', 2, 3);
    applyCSG('-', 1, 0);
    applyCSG('-', 2, 0);
    ui->glMain->deleteTar(getTarNum()-1);

    /************************************************///ui->btnCut->click();
    for(int i=1;i<2;i++){
        QVector3D c = QVector3D(0,0,0);
        QVector3D n = QVector3D(0,0,1);

        int cur = i;
        int tar = ui->glMain->getTarnum();
        ui->glMain->copyObj(cur);

        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),QVector3D(10,10,1),c-QVector3D(0,0,50),n);
        applyCSG('-',cur,ui->glMain->getTarnum()-1);
        applyCSG('*',tar,ui->glMain->getTarnum()-1);
        ui->glMain->deleteTar(ui->glMain->getTarnum()-1);
    }
    for(int i=2;i<3;i++){
        QVector3D c = QVector3D(0,0,0);
        QVector3D n = QVector3D(0,0,1);

        int cur = i;
        int tar = ui->glMain->getTarnum();
        ui->glMain->copyObj(cur);

        putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),QVector3D(10,1,10),c-QVector3D(0,50,0),n);
        applyCSG('-',cur,ui->glMain->getTarnum()-1);
        applyCSG('*',tar,ui->glMain->getTarnum()-1);
        ui->glMain->deleteTar(ui->glMain->getTarnum()-1);
    }
    /************************************************/
    float tr = oR;
    putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),QVector3D(tr,tr,tr),QVector3D( oR*50 + iR*50,0,oR*50+tr*50),QVector3D(0,0,1));
    ui->glMain->genSpiral(QVector3D( oR*50 + iR*50,0,oR*50), QVector3D(0,0,1), oR*50  /1.41421f , 1, 100, 90, 45, 0.5f, 0.5f);
    applyCSG('-',getTarNum()-2,getTarNum()-1);
    ui->glMain->deleteTar(ui->glMain->getTarnum()-1);

}

void MainWindow_support::genTest_2(){
    readST();
    genTube();
    genNut();
    ThinStruct * ts = &var->ts;
    float iR=ts->innerR/50,oR=ts->outerR/50,nR=ts->nutR/50;
    ui->glMain->genSpiral(QVector3D( -nR*50/2, nR*50/4,0), QVector3D(-2,1,0) , oR*50, 1, 10, 0, 0, 0.5f, 0.5f);
    applyCSG('+', 4, getTarNum()-1);
    ui->glMain->deleteTar(getTarNum()-1);

}

void MainWindow_support::genTest_3()
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
        QVector3D v1 = ts->getVectice(ts->edges[i*2]);
        QVector3D v2 = ts->getVectice(ts->edges[i*2+1]);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(oR,oR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
    }
    mergeAll(1, getTarNum()-1);
    /**************************************************/
    genCut(1,QVector3D(oR*50         ,0             ,0),QVector3D(1,  0,0));
    genCut(1,QVector3D(0             ,oR*50         ,0),QVector3D(0,  1,0));
    genCut(1,QVector3D(oR*50/1.41421f,oR*50/1.41421f,0),QVector3D(-1,-1,0));
    /**************************************************/

    applyCSG('-', 1, 0);
    applyCSG('-', 2, 0);
    applyCSG('-', 3, 0);
    applyCSG('-', 4, 0);
    ui->glMain->deleteTar(getTarNum()-1);
}
