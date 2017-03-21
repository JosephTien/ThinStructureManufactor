#ifndef MAINWINDOW_SUPPORT_H
#define MAINWINDOW_SUPPORT_H
#include "ui_mainwindow.h"
#include <iglmachine.h>
#include <cgaltool.h>
#include <viewmanager.h>
#include <modelmanager.h>
#include <maintainvar.h>
#include <iostream>
#include <thinstruct.h>

class MainWindow_support{
public:
    MainWindow_support(){}
    Ui::MainWindow * ui;
    MaintainVar * var;
    IglMachine iglMachine;
    CGALTool cgaltool;
    MainWindow_support(Ui::MainWindow * ui, MaintainVar * var){
        this->ui = ui;
        this->var = var;
    }
    ModelManager * getModel(int tar);
    inline int getTarNum(){return ui->glMain->getTarnum();}
    void putStdModel(const char * name, QVector3D color, QVector3D scale, QVector3D translate, QVector3D rotate);
    void applyCSG(char c,int er, int ee);
    void stateInit();
    void fill(int tar);
    void readST();
    void mergeAll();
    void mergeAll(int from, int to);
    void genTube();
    void genNut();
    void genTest_1();

};

#endif // MAINWINDOW_SUPPORT_H
