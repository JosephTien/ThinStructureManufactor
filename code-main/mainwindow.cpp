#include "mainwindow.h"
#include "ui_mainwindow.h"

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
    support = MainWindow_support(ui, &var);
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

void MainWindow::on_btnReadST_clicked(){
    support.readST();
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

        int cur = i;
        int tar = ui->glMain->getTarnum();
        ui->glMain->copyObj(cur);

        support.putStdModel("cube10X10", QVector3D(0.5f,0.5f,0.5f),QVector3D(10,10,1),c-QVector3D(0,0,50),n);
        support.applyCSG('-',cur,ui->glMain->getTarnum()-1);
        support.applyCSG('*',tar,ui->glMain->getTarnum()-1);
        ui->glMain->deleteTar(ui->glMain->getTarnum()-1);
    }
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}

void MainWindow::on_btnTest_1_clicked(){
    support.genTest_1();
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}
void MainWindow::on_btnTest_2_clicked(){
    support.genTest_2();
    renew_cmbbObject();
    ui->cmbbObject->setCurrentIndex(ui->glMain->getTarnum());
}
void MainWindow::on_btnTest_3_clicked(){
    support.genTest_3();
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
