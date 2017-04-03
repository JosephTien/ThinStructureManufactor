#ifndef MAINTAINVAR_H
#define MAINTAINVAR_H
#include<ui_mainwindow.h>
#include<pch.h>
#include<thinstruct.h>
class MaintainVar{

public:
    MaintainVar(){}
    Ui::MainWindow * ui;
    MaintainVar(Ui::MainWindow * ui){this->ui = ui;}
    int tarObj = 0;
    int connectTarNum;
    ThinStruct ts;
    int * edgeInstance;
};

#endif // MAINTAINVAR_H
