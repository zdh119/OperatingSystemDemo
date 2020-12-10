#include "ramwidget.h"
#include "ui_ramwidget.h"
#include <QDebug>
RAMWidget::RAMWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RAMWidget)
{
    ui->setupUi(this);
}

RAMWidget::~RAMWidget()
{
    delete ui;
}

void RAMWidget::showRAM(QList<Subarea*> *subareaList, QList<PCB*> *PCBList){
    for(int i=ui->unallocatedTableWidget->rowCount();i>=0;i--){
        ui->unallocatedTableWidget->removeRow(i);
    }
    for(int i=ui->allocatedTableWidget->rowCount();i>=0;i--){
        ui->allocatedTableWidget->removeRow(i);
    }

    for(int i=0;i<subareaList->size();i++){
        Subarea* temp = subareaList->at(i);
        ui->unallocatedTableWidget->insertRow(i);
        ui->unallocatedTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(temp->start)));
        ui->unallocatedTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(temp->size)));
    }

    for(int i=0;i<PCBList->size();i++){
        PCB* pcb = PCBList->at(i);
        ui->allocatedTableWidget->insertRow(i);
        ui->allocatedTableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(pcb->PID)));
        ui->allocatedTableWidget->setItem(i, 1, new QTableWidgetItem(QString::number(pcb->startAddr)));
        ui->allocatedTableWidget->setItem(i, 2, new QTableWidgetItem(QString::number(pcb->need)));
    }
}
