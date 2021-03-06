#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "maintainvar.h"
#include "mainwindow_support.h"

#include <qtconcurrentrun.h>

MaintainVar var;
MainWindow_support support;
/*
int threadState = 0;
void threadCont(QString name){
    while(true){
        std::cout << "test" << std::endl;
        QThread::sleep(1);
        if(threadState==1){
            support.genTest_6();
            threadState = 0;
        }
    }
}
QFuture<void> thread = QtConcurrent::run(threadCont, QString("threadCont"));
*/

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    Qt::WindowFlags flags = 0;
    setWindowFlags(flags);
    setFixedSize(800,520);
    /******************/
    flags |= Qt::WindowMinimizeButtonHint;
    //support = MainWindow_support(ui, &var);
    var = MaintainVar(ui);
    support = MainWindow_support(&var);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/****************************************/

void MainWindow::on_btnReset_clicked(){
    ui->glMain->viewMgr->Reset();
    support.stateInit();
}

void MainWindow::on_btnOutput_clicked()
{
    support.iglMachine.reset();
    support.iglMachine.put("mainobj", support.getModel(var.tarObj)->vertices, support.getModel(var.tarObj)->indices);
    char str[20];
    sprintf(str,"output.obj");
    support.iglMachine.writeFile("mainobj",str);
}

void MainWindow::on_btnOutputAll_clicked(){
    for(int i=0;i<ui->glMain->getTarnum();i++){
        support.iglMachine.reset();
        support.iglMachine.put("mainobj", support.getModel(i)->vertices, support.getModel(i)->indices);
        char str[20];
        sprintf(str,"output_%d.obj", i);
        support.iglMachine.writeFile("mainobj",str);
    }
}

void MainWindow::on_btnLoad_Manu_clicked()
{
    std::string path = QFileDialog::getOpenFileName(this, tr("Load obj"), "..", tr("Object Files(*.obj)")).toStdString();
    if(path.length() == 0) {
            QMessageBox::information(NULL, tr("Path"), tr("You didn't select any files."));
    } else {
            //QMessageBox::information(NULL, tr("Path"),tr(path.data()));
            QMessageBox qmb;
            qmb.setStandardButtons(NULL);
            qmb.setText(tr("                     "));
            qmb.setWindowTitle(tr("Loading......."));
            qmb.show();
            ui->glMain->load(path.data());
            qmb.close();
            char str[10];
            var.tarObj = ui->glMain->getTarnum()-1;
            sprintf(str,"Object %d",var.tarObj+1);
            ui->cmbbObject->addItem(QString(str));
            ui->cmbbObject->setCurrentIndex((var.tarObj));
            ui->glMain->setVis(var.tarObj,1);
            this->setFocus();
    }
}

void MainWindow::on_btnCalST_clicked(){
    support.calST();
}

void MainWindow::on_btnCalSTLink_clicked(){
    support.calSTLink();
}

void MainWindow::on_btnGenST_clicked(){
    support.genST();
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}

void MainWindow::on_btnGenTube_clicked(){
    support.genTube();
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}

void MainWindow::on_btnGenNut_clicked(){
    support.genNut();
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}

void MainWindow::on_btnCut_clicked(){

    int tarnum = ui->glMain->getTarnum();
    for(int i=0;i<tarnum;i++){
        QVector3D c = QVector3D(0,0,0);
        QVector3D n = QVector3D(0,0,1);
        support.applyCut(i,QVector3D(0,0,0),QVector3D(0,0,1));
    }
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}
void MainWindow::on_btnTest_clicked(){
    support.genTest_4();
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}

void MainWindow::on_btnTest_2_clicked(){
    //threadState=1;
    support.genTest_6();
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}

void MainWindow::on_btnTest_3_clicked(){
    ui->btnLoad_Manu->click();
    support.genMolt();
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}

/****************************************/

void MainWindow::on_cmbbObject_currentIndexChanged(int index)
{
    if(index == ui->glMain->getTarnum())ui->glMain->setAllVis(1);
    else for(int i=0;i<ui->glMain->getTarnum();i++){
         if(i==index)ui->glMain->setVis(i,1);
         else ui->glMain->setVis(i,0);
    }
     var.tarObj = index;
     ui->glMain->tarObj = var.tarObj;
     ui->glMain->update();
}

void MainWindow::mousePressEvent(QMouseEvent *){
    this->setFocus();
}
void MainWindow::keyReleaseEvent(QKeyEvent *event){
    ui->glMain->keyReleaseEvent(event);
    this->setFocus();
}
void MainWindow::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_E){
        ui->cmbbObject->setCurrentIndex((var.tarObj+ui->cmbbObject->count()-1)%ui->cmbbObject->count());
    }else if(event->key() == Qt::Key_D){
        ui->cmbbObject->setCurrentIndex((var.tarObj+1)%ui->cmbbObject->count());
    }
    ui->glMain->keyPressEvent(event);
    this->setFocus();
}

void MainWindow::renew_cmbbObject(){
    ui->cmbbObject->clear();
    for(int i=0;i<ui->glMain->getTarnum();i++){
        char str[10];
        sprintf(str,"Object %d",i);
        ui->cmbbObject->addItem(QString(str));
    }
    ui->cmbbObject->addItem("All");
}
