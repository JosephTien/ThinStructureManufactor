#ifndef MAINWINDOW_SUPPORT_H
#define MAINWINDOW_SUPPORT_H
#include <cgaltool.h>
//#include <ui_mainwindow.h>
#include <iglmachine.h>
#include <viewmanager.h>
#include <modelmanager.h>
#include <maintainvar.h>
#include <thinstruct.h>

class MainWindow_support{
public:
    MainWindow_support(){}
    //Ui::MainWindow * ui;
    MaintainVar * var;
    IglMachine iglMachine;
    CGALTool cgaltool;
    /*
    MainWindow_support(Ui::MainWindow * ui, MaintainVar * var){
        this->ui = ui;
        this->var = var;
    }
    */
    MainWindow_support(MaintainVar * var){
        this->var = var;
    }
    ModelManager * getModel(int tar);
    ModelViewer * glMain();
    inline int getTarnum(){return glMain()->getTarnum();}
    inline void deleteTar(int tar){glMain()->deleteTar(tar);}
    inline void deleteLastTar(){glMain()->deleteTar(getTarnum()-1);}
    void fill(int tar);
    void putStdModel(const char * name, QVector3D color, QVector3D scale, QVector3D translate, QVector3D rotate);
    void applyCSG(char c,int er, int ee);
    void stateInit();
    void calST();
    void calSTLink();
    void genST();
    void mergeAll();
    void mergeAll(int from, int to);
    void genTube();
    void genNut();
    void applyCut(int tar, QVector3D c, QVector3D n);
    void applyFastCut(int cur, QVector3D c, QVector3D n);
    void genTest_1();
    void genTest_2();
    void genTest_3();
    void genTest_4();
    void genTest_5();
    void genTest_6();
    void genTest_6_2();
    void genTest_7();
    void genTest_7_2();

    QMatrix4x4 getRotateMatrix(QVector3D axis, QVector3D from, QVector3D to){
        from.normalize();
        to.normalize();
        QMatrix4x4 rot;
        float dot = QVector3D::dotProduct(from,to);
        rot.rotate( acosf(dot)/(2*M_PI)*360, axis);
        return rot;
    }

    QMatrix4x4 getRotateMatrix(QVector3D from, QVector3D to){
        from.normalize();
        to.normalize();
        QMatrix4x4 rot;
        QVector3D axis = QVector3D::crossProduct(from, to);
        float dot = QVector3D::dotProduct(from,to);
        if(dot>0.95f||dot<-0.95f)axis = QVector3D(to.x(), to.z(), to.y());
        return getRotateMatrix(axis, from, to);
    }
    void updateModel(int tar){
        getModel(tar)->applyModelMatrix_force();
        getModel(tar)->renewByMatrix();
        glMain()->reBuffer(tar);
    }
    void fixRotateMatrix(QVector3D vec, int tarobj){
        QMatrix4x4 rot = getRotateMatrix(QVector3D(0,0,1),vec);
        QVector3D axis = (rot * QVector4D(0,0,1,1)).toVector3DAffine();
        QVector3D forw = (rot * QVector4D(0,1,0,1)).toVector3DAffine();
        QVector3D tarv = QVector3D(0,0,-1);
        QMatrix4x4 rot2 = getRotateMatrix(axis, forw, tarv);
        getModel(tarobj)->translate_pure(-getModel(tarobj)->centerTranslation); updateModel(tarobj);
        getModel(tarobj)->rotate(rot2);                                         updateModel(tarobj);
        getModel(tarobj)->translate_pure(getModel(tarobj)->centerTranslation);  updateModel(tarobj);

    }
    void fixRotateMatrix(QVector3D vec, QVector3D tarv, int tarobj){
        tarv = QVector3D::crossProduct(vec, QVector3D::crossProduct(tarv, vec).normalized()).normalized();
        QMatrix4x4 rot = getRotateMatrix(QVector3D(0,0,1),vec);
        QVector3D axis = (rot * QVector4D(0,0,1,1)).toVector3DAffine();
        QVector3D forw = (rot * QVector4D(0,1,0,1)).toVector3DAffine();
        QMatrix4x4 rot2 = getRotateMatrix(axis, forw, tarv);
        getModel(tarobj)->translate_pure(-getModel(tarobj)->centerTranslation); updateModel(tarobj);
        getModel(tarobj)->rotate(rot2);                                         updateModel(tarobj);
        getModel(tarobj)->translate_pure(getModel(tarobj)->centerTranslation);  updateModel(tarobj);

    }
};

#endif // MAINWINDOW_SUPPORT_H
