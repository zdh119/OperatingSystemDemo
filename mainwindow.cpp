#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
MainWindow::MainWindow(SystemInfo *systemInfo, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    this->addJobWidget = new AddJobWidget(systemInfo);
    this->ramWidget = new RAMWidget();

    this->addJobWidget->hide();
    this->ramWidget->hide();

    ui->setupUi(this);
    ui->backupTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->readyTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->suspendTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->statusTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //初始化系统信息设置
    this->systemInfo = systemInfo;
    jobList = new QList<Job*>;
    PCBList = new QList<PCB*>;
    readyList = new QList<PCB*>;
    suspendList = new QList<PCB*>;
    runningPCB = NULL;

    subareaList = new QList<Subarea*>;
    Subarea* initarea = new Subarea;
    initarea->start = systemInfo->totalStartAddr + systemInfo->osSize;
    initarea->size = systemInfo->totalSize - systemInfo->osSize;
    subareaList->append(initarea);


    modelTimer = new QTimer(this);
    modelTimer->start(50);

    model = 1;

    connect(ui->jobSchedulingPushButton, &QPushButton::clicked, this, &MainWindow::jobSchedulingButtonClick);
    connect(this, &MainWindow::processEnd, this, &MainWindow::autoJobSchedulingSlot);
    connect(ui->suspendPushButton, &QPushButton::clicked, this, &MainWindow::suspendButtonClick);
    connect(ui->recoverPushButton, &QPushButton::clicked, this, &MainWindow::recoverButtonClick);
    connect(ui->CPUSchedulingPushButton, &QPushButton::clicked, this, &MainWindow::CPUSchedulingButtonClick);
    connect(this, &MainWindow::keepRunning, this, &MainWindow::CPUSchedulingButtonClick);
    connect(this, &MainWindow::updateStatus, this, &MainWindow::showPCBList);
    connect(ui->addJobButton, &QPushButton::clicked, this, &MainWindow::addJobButtonClick);
    connect(this->addJobWidget, &AddJobWidget::addJobSignal, this, &MainWindow::confirmButtonClick);
    connect(ui->viewRAMButton, &QPushButton::clicked, this, &MainWindow::viewRAMButtonClick);
    connect(this, &MainWindow::ramChange, this->ramWidget, &RAMWidget::showRAM);

    connect(ui->settingsButton, &QPushButton::clicked, this, &MainWindow::settingsButtonClick);

    connect(modelTimer, &QTimer::timeout, [=](){
        if(ui->manRadioButton->isChecked()){
            model = 1;

        }
        else{
            model = 0;
            if(readyList->size()!=0)
            emit keepRunning();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::settingsButtonClick(){
    Settings* settings = new Settings(systemInfo);
    settings->show();

}

void MainWindow::viewRAMButtonClick(){
    ramWidget->show();
    ramWidget->raise();
    emit ramChange(subareaList, PCBList);
}

void MainWindow::addJobButtonClick(){
    addJobWidget->show();
    addJobWidget->raise();
}

//添加作业按钮点击事件函数
void MainWindow::confirmButtonClick(){
    this->jobList = this->addJobWidget->jobList;
    showJobList();
}
//作业调度按钮点击事件函数
void MainWindow::jobSchedulingButtonClick(){
    int size = 0;
    if(systemInfo->remainNum>jobList->size()){
        size = jobList->size();
    }
    else{
        size = systemInfo->remainNum;
    }
    qDebug()<<" readyList has "<<readyList->size();
    if(size!=0){
        int i = 0;
        int j = 0;
        while(systemInfo->remainNum>0){
            if(addProcess(jobList, i-j, PCBList, subareaList, systemInfo)){

                systemInfo->remainNum--;
                j++;
            }
            i++;
            if(jobList->size()<=i-j){
                break;
            }
        }
        showJobList();
        sortPCBByPID(PCBList);
        emit updateStatus();
        readyList = filterList(PCBList, READY);
        sortPCB(readyList);
        showReadyList();
        suspendList = filterList(PCBList, SUSPENDING);
        sortPCB(suspendList);
        showSuspendList();
        emit ramChange(subareaList, PCBList);
        qDebug()<<"current readyList has "<<readyList->size();
        qDebug()<<"当前剩余道数"<<systemInfo->remainNum;

    }
    else{
        QMessageBox::information(this, tr("作业调度失败"),tr("当前后备队列为空，或内存中进程数已满"));
    }
}
void MainWindow::suspendButtonClick(){
    QList<QTableWidgetItem*> items = ui->readyTableWidget->selectedItems();
    if(items.size()!=0){
        int row = ui->readyTableWidget->row(items.first());
        int PID = ui->readyTableWidget->item(row, 0)->text().toInt();

        suspendList->append(readyList->at(getIndexByPID(PID, readyList)));
        ui->readyTableWidget->removeRow(row);

        readyList->removeAt(row);

        PCBList->at(getIndexByPID(PID, PCBList))->status = SUSPENDING;
        emit updateStatus();
        sortPCB(suspendList);
        showSuspendList();
    }
    else{
        QMessageBox::information(this, tr("挂起失败"), tr("未选中任何内容，请选中内容后挂起"));
    }

}
void MainWindow::recoverButtonClick(){
    QList<QTableWidgetItem*> items = ui->suspendTableWidget->selectedItems();
    if(items.size()!=0){
        int row = ui->suspendTableWidget->row(items.first());
        int PID = ui->suspendTableWidget->item(row, 0)->text().toInt();

        readyList->append(suspendList->at(getIndexByPID(PID, suspendList)));

        ui->suspendTableWidget->removeRow(row);
        suspendList->removeAt(row);

        PCBList->at(getIndexByPID(PID, PCBList))->status = READY;
        emit updateStatus();

        sortPCB(readyList);
        showReadyList();
    }
    else{
        QMessageBox::information(this, tr("解挂失败"), tr("未选中任何内容，请选中内容后解挂"));
    }
}
void MainWindow::CPUSchedulingButtonClick(){
    if(runningPCB==NULL&&readyList->size()!=0){
        ui->CPUSchedulingPushButton->setText("运行中");
        ui->CPUSchedulingPushButton->setEnabled(false);
        runningPCB = readyList->first();
        readyList->removeAt(0);

        ui->readyTableWidget->removeRow(0);
        QString s = "";
        s+="进程名:    " + runningPCB->processName + '\n';
        s+="PID:      " + QString::number(runningPCB->PID) + '\n';
        s+="Priority: " + QString::number(runningPCB->priority) + '\n';
        s+="RunTime:  " + QString::number(runningPCB->runTime) + '\n';
        s+="StartAddr:" + QString::number(runningPCB->startAddr) + '\n';
        s+="Size:     " + QString::number(runningPCB->need) + '\n';
        s+="Content:  " + runningPCB->content + '\n';

        s+="— — — —Finish— — — —";
        ui->CPUTextEdit1->append(s);

        runningPCB->status = RUNNING;
        emit updateStatus();
        QEventLoop eventloop;
        QTimer::singleShot(2000, &eventloop, SLOT(quit())); //wait 2s
        eventloop.exec();

        runningPCB->runTime--;
        runningPCB->priority--;

        s = "";
        s+="进程名:    " + runningPCB->processName + '\n';
        s+="PID:      " + QString::number(runningPCB->PID) + '\n';
        s+="Priority: " + QString::number(runningPCB->priority) + '\n';
        s+="RunTime:  " + QString::number(runningPCB->runTime) + '\n';
        s+="StartAddr:" + QString::number(runningPCB->startAddr) + '\n';
        s+="Size:     " + QString::number(runningPCB->need) + '\n';
        s+="Content:  " + runningPCB->content + '\n';
        s+="— — — —Finish— — — —";
        ui->CPUTextEdit2->append(s);

        ui->CPUSchedulingPushButton->setText("CPU调度");
        ui->CPUSchedulingPushButton->setEnabled(true);

        if(runningPCB->runTime<=0){
            PCBList->removeAt(getIndexByPID(runningPCB->PID, PCBList));
            emit updateStatus();
            systemInfo->remainNum++;
            systemInfo->pid[runningPCB->PID] = 1;
            mergeSubarea(subareaList, runningPCB);

            emit ramChange(subareaList, PCBList);
        }
        else{
            runningPCB->status = READY;
            emit updateStatus();
            readyList->append(runningPCB);
            sortPCB(readyList);
            showReadyList();

        }
        runningPCB = NULL;


        if(ui->autoRadioButton->isChecked()){

            if(systemInfo->remainNum<systemInfo->num){
                emit processEnd();
            }

            QTimer::singleShot(1000, &eventloop, SLOT(quit())); //wait 2s
            eventloop.exec();
            if(readyList->size()!=0){
                emit keepRunning();
            }
            else{
                ui->manRadioButton->setChecked(true);
                ui->autoRadioButton->setChecked(false);
            }
        }
    }
    else{
        QMessageBox::information(this, tr("CPU调度失败"), tr("CPU正在执行进程或当前ready队列为空"));
    }

}
void MainWindow::autoJobSchedulingSlot(){
    int size = 0;
    if(systemInfo->remainNum>jobList->size()){
        size = jobList->size();
    }
    else{
        size = systemInfo->remainNum;
    }
    if(size!=0){
        int i = 0;
        int j = 0;
        while(systemInfo->remainNum>0){
            if(addProcess(jobList, i-j, PCBList, subareaList, systemInfo)){

                systemInfo->remainNum--;
                j++;
            }
            i++;
            if(jobList->size()<=i-j){
                break;
            }
        }
        showJobList();
        sortPCBByPID(PCBList);
        emit updateStatus();
        readyList = filterList(PCBList, READY);
        sortPCB(readyList);
        showReadyList();
        suspendList = filterList(PCBList, SUSPENDING);
        sortPCB(suspendList);
        showSuspendList();
        emit ramChange(subareaList, PCBList);

    }
}

void MainWindow::showJobList(){
    for(int i=ui->backupTableWidget->rowCount();i>=0;i--){
        ui->backupTableWidget->removeRow(i);
    }
    for(int i=0;i<jobList->size();i++){
        Job* job = jobList->at(i);
        ui->backupTableWidget->insertRow(i);
        ui->backupTableWidget->setItem(i, 0, new QTableWidgetItem(job->processName));
        ui->backupTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(job->priority)));
        ui->backupTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(job->need)));
        ui->backupTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(job->runTime)));
        ui->backupTableWidget->setItem(i, 4, new QTableWidgetItem(job->content));
    }

}
void MainWindow::showReadyList(){
    for(int i=ui->readyTableWidget->rowCount();i>=0;i--){
        ui->readyTableWidget->removeRow(i);
    }
    for(int i=0;i<readyList->size();i++){
        PCB* pcb = readyList->at(i);
        ui->readyTableWidget->insertRow(i);
        ui->readyTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(pcb->PID)));
        ui->readyTableWidget->setItem(i, 1, new QTableWidgetItem(pcb->processName));
        ui->readyTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(pcb->priority)));
        ui->readyTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(pcb->startAddr)));
        ui->readyTableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(pcb->need)));
        ui->readyTableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(pcb->runTime)));
        ui->readyTableWidget->setItem(i, 6, new QTableWidgetItem(pcb->content));

    }
}
void MainWindow::showSuspendList(){
    for(int i=ui->suspendTableWidget->rowCount();i>=0;i--){
        ui->suspendTableWidget->removeRow(i);
    }
    for(int i=0;i<suspendList->size();i++){
        PCB* pcb = suspendList->at(i);
        ui->suspendTableWidget->insertRow(i);
        ui->suspendTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(pcb->PID)));
        ui->suspendTableWidget->setItem(i, 1, new QTableWidgetItem(pcb->processName));
        ui->suspendTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(pcb->priority)));
        ui->suspendTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(pcb->startAddr)));
        ui->suspendTableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(pcb->need)));
        ui->suspendTableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(pcb->runTime)));
        ui->suspendTableWidget->setItem(i, 6, new QTableWidgetItem(pcb->content));
    }
}
void MainWindow::showPCBList(){
    for(int i=ui->statusTableWidget->rowCount();i>=0;i--){
        ui->statusTableWidget->removeRow(i);
    }
    for(int i=0;i<PCBList->size();i++){
        PCB* pcb = PCBList->at(i);
        ui->statusTableWidget->insertRow(i);
        ui->statusTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(pcb->PID)));
        ui->statusTableWidget->setItem(i, 1, new QTableWidgetItem(pcb->processName));
        ui->statusTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(pcb->priority)));
        ui->statusTableWidget->setItem(i, 3, new QTableWidgetItem(QString::number(pcb->startAddr)));
        ui->statusTableWidget->setItem(i, 4, new QTableWidgetItem(QString::number(pcb->need)));
        ui->statusTableWidget->setItem(i, 5, new QTableWidgetItem(QString::number(pcb->runTime)));
        ui->statusTableWidget->setItem(i, 6, new QTableWidgetItem(pcb->content));
        ui->statusTableWidget->setItem(i, 7, new QTableWidgetItem(statusArray[pcb->status]));
    }
}
