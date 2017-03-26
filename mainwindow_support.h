#ifndef MAINWINDOW_SUPPORT_H
#define MAINWINDOW_SUPPORT_H
#define QUP QVector3D(0,0,1)
#define QFORW QVector3D(0,1,0)
#define QZERO QVector3D(0,0,0)
#define QGRAY QVector3D(0.5f,0.5f,0.5f)
#ifdef COMPILECGAL
    #include <cgaltool.h>
#endif
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
    /*
    MainWindow_support(Ui::MainWindow * ui, MaintainVar * var){
        this->ui = ui;
        this->var = var;
    }
    */
    MainWindow_support(MaintainVar * var){
        this->var = var;
    }
#ifdef COMPILECGAL
    CGALTool cgaltool;
    void fill(int tar);
#endif
    ModelManager * getModel(int tar);
    ModelViewer * glMain();
    inline int getTarnum(){return glMain()->getTarnum();}
    inline void deleteTar(int tar){glMain()->deleteTar(tar);}
    inline void deleteLastTar(){glMain()->deleteTar(getTarnum()-1);}
    void putStdModel(const char * name, QVector3D color, QVector3D scale, QVector3D translate, QVector3D rotate);
    void applyCSG(char c,int er, int ee);
    void stateInit();
    void readST();
    void mergeAll();
    void mergeAll(int from, int to);
    void genTube();
    void genNut();
    void applyCut(int tar, QVector3D c, QVector3D n);
    void genTest_1();
    void genTest_2();
    void genTest_3();
    void genTest_4();
    void genTest_5();

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
        QVector3D axis = QVector3D(0,0,1) * rot;
        QVector3D forw = QVector3D(0,1,0) * rot;
        QVector3D tarv = QVector3D(0,0,-1);
        QMatrix4x4 rot2 = getRotateMatrix(axis, forw, tarv);
        getModel(tarobj)->rotate(rot2);
        updateModel(tarobj);
    }
};

#endif // MAINWINDOW_SUPPORT_H
