#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <maintainvar.h>
#include <mainwindow_support.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    Ui::MainWindow * getUi(){return ui;}

private slots:

    void on_btnReset_clicked();

    void on_btnOutput_clicked();

    void on_btnOutputAll_clicked();

    void on_btnLoad_Manu_clicked();

    void on_btnCalST_clicked();

    void on_btnGenST_clicked();

    void on_btnGenTube_clicked();

    void on_btnGenNut_clicked();

    void on_btnCut_clicked();

    void on_btnTest_clicked();

    void on_btnTest_2_clicked();

    void on_cmbbObject_currentIndexChanged(int index);

    void mousePressEvent(QMouseEvent *event);

    void keyPressEvent(QKeyEvent *);

    void keyReleaseEvent(QKeyEvent *event);


private:

    Ui::MainWindow *ui;
    MaintainVar var;
    MainWindow_support support;
    void renew_cmbbObject();
};

#endif // MAINWINDOW_H
