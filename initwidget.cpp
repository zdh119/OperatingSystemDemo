#include "initwidget.h"
#include "ui_initwidget.h"
#include "system.h"
#include "mainwindow.h"
#include <QMessageBox>
InitWidget::InitWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::InitWidget)
{
    ui->setupUi(this);

    connect(ui->confirmPushButton, &QPushButton::clicked, this, &InitWidget::confirmButtonClick);
}

InitWidget::~InitWidget()
{
    delete ui;
}
void InitWidget::confirmButtonClick(){
    SystemInfo* systemInfo = new SystemInfo;
    systemInfo->num = ui->numComboBox->currentText().toInt();
    systemInfo->remainNum = systemInfo->num;
    //操作系统默认起址为0
    systemInfo->osStartAddr = 0;
    systemInfo->osSize = ui->OSSizeLineEdit->text().toInt();
    systemInfo->totalStartAddr = 0;
    systemInfo->totalSize = ui->RAMLineEdit->text().toInt();
    systemInfo->pid = new bool[systemInfo->num];
    for(int i=0;i<systemInfo->num;i++){
        systemInfo->pid[i] = true;
    }


    if(systemInfo->osSize>=systemInfo->totalSize){
        QMessageBox::information(this, tr("设置失败"), tr("操作系统的大小应小于内存总大小"));
    }
    else{
        MainWindow* mainWindow = new MainWindow(systemInfo);
        mainWindow->show();
        this->close();
    }
}
