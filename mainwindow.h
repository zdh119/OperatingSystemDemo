#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "pcb.h"
#include "pool.h"
#include "util.h"
#include "system.h"
#include <QList>
#include <QMessageBox>
#include <QTimer>
#include "addjobwidget.h"
#include "ramwidget.h"
#include "settings.h"
class RAMWidget;
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(SystemInfo* systemInfo, QWidget *parent = 0);

    ~MainWindow();

    AddJobWidget* addJobWidget;
    RAMWidget* ramWidget;


    void showJobList();
    void showReadyList();
    void showSuspendList();
    void showPCBList();

    QList<Job*> *jobList;
    QList<PCB*> *PCBList;
    QList<PCB*> *readyList;
    PCB* runningPCB;
    QList<PCB*> *suspendList;
    SystemInfo* systemInfo;
    QTimer* modelTimer;
    int model;//0--自动模式，1--手动模式
    QList<Subarea*> *subareaList;
signals:
    void processEnd();
    void keepRunning();
    void updateStatus();
    void ramChange(QList<Subarea*> *subareaList, QList<PCB*> *PCBList);
protected slots:
    //点击添加进程按钮
    void confirmButtonClick();
    void jobSchedulingButtonClick();
    void suspendButtonClick();
    void recoverButtonClick();
    void CPUSchedulingButtonClick();
    void autoJobSchedulingSlot();
    void addJobButtonClick();
    void viewRAMButtonClick();
    void settingsButtonClick();
private:
    Ui::MainWindow *ui;


};

#endif // MAINWINDOW_H
