#include "mainwindow_support.h"

ModelManager* MainWindow_support::getModel(int tar){
    return &(glMain()->viewMgr->modelMgr[tar]);
}

ModelViewer * MainWindow_support::glMain(){
    return var->ui->glMain;
}

void MainWindow_support::putStdModel(const char * name,QVector3D color, QVector3D scale, QVector3D translate, QVector3D rotate){
    char file[30];
    sprintf(file,":/object/%s.obj",name);
    QFile stdObjFile(file);
    glMain()->load_rc(&stdObjFile);
    var->connectTarNum = glMain()->getTarnum()-1;
    glMain()->setColor(var->connectTarNum,color.x(),color.y(),color.z());
    getModel(var->connectTarNum)->ResetModel();
    //getModel(var->connectTarNum)->SetScale(scale.x(),scale.y(),scale.z());
    getModel(var->connectTarNum)->scaleDepend_ori(QVector3D(0,0,0),scale.x(),scale.y(),scale.z());
    getModel(var->connectTarNum)->translate_pure(translate);
    getModel(var->connectTarNum)->rotateTo(rotate);
    glMain()->setVis(var->connectTarNum,1);
    getModel(var->connectTarNum)->applyModelMatrix_force();
    getModel(var->connectTarNum)->renewByMatrix();
    glMain()->reBuffer(var->connectTarNum);
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
    glMain()->reBuffer(er);
}

void MainWindow_support::stateInit(){
    //deleteSkelSup();
    for(int i=glMain()->getTarnum()-1;i>=0;i--){
        glMain()->deleteTar(i);
    }
    for(int i=glMain()->getTarnum_ass()-1;i>=0;i--){
        glMain()->deleteTar_ass(i);
    }
    //var->skelMgr.Reset();
    var->ui->cmbbObject->clear();

}

#ifdef COMPILECGAL
void MainWindow_support::fill(int tar){
    std::string mstr = cgaltool.fillHoleAndGetStr(getModel(var->tarObj)->vertices_ori, getModel(var->tarObj)->indices);
    cgaltool.readFromOFFStream(getModel(var->tarObj)->vertices_ori, getModel(var->tarObj)->indices,mstr);
    mstr.clear();
    getModel(tar)->applyModelMatrix_force();
    getModel(tar)->refresh();
}
#endif

void MainWindow_support::readST(){
    ThinStruct * ts = &var->ts;
    ts->read("thinstruct.txt");
    float iR=ts->innerR/50,oR=ts->outerR/50,nR=ts->nutR/50;
    for(int i=0;i<ts->vertices.size()/3;i++){
        putStdModel("sphere10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(iR,iR,iR),
                    QVector3D(ts->vertices[i*3], ts->vertices[i*3+1], ts->vertices[i*3+2]),
                    QVector3D(0,0,1));
    }
    for(int i=0;i<ts->edges.size()/2;i++){
        QVector3D v1 = ts->getVertice(ts->edges[i*2]);
        QVector3D v2 = ts->getVertice(ts->edges[i*2+1]);
        putStdModel("cylinder10X10", QVector3D(0.5f,0.5f,0.5f),
                    QVector3D(iR,iR,(v1-v2).length()/100),
                    (v1+v2)/2,
                    v2-v1);
    }
    mergeAll();
}

void MainWindow_support::mergeAll(){
    int tarnum;
    while((tarnum=getTarnum())>1){
        for(int i=0;i<tarnum/2;i++){
            applyCSG('+', i, i+(tarnum+1)/2);
        }
        for(int i=tarnum-1; i>=(tarnum+1)/2; i--){
            glMain()->deleteTar(i);
        }
    }
}

void MainWindow_support::mergeAll(int from, int to){
    while(from<to){
        for(int i=from;i<from+(to-from+1)/2;i++){
            applyCSG('+', i, i+(to-from+1+1)/2);
        }
        for(int i=to; i>=from+(to-from+1+1)/2; i--){
            glMain()->deleteTar(i);
        }
        to = from+(to-from+1+1)/2-1;
    }
}

void MainWindow_support::applyCut(int tar, QVector3D c, QVector3D n){
    glMain()->copyObj(tar);
    putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),
                QVector3D( 100, 100, 100),
                c + n.normalized()*5000,
                n);
    //getModel(getTarNum()-1)->translate_pure(n.normalized()*500);
    //getModel(getTarNum()-1)->applyModelMatrix_force();

    applyCSG('-', tar, getTarnum()-1);
    applyCSG('*', getTarnum()-2, getTarnum()-1);
    glMain()->deleteTar(getTarnum()-1);
}
