#include "addjobwidget.h"
#include "ui_addjobwidget.h"

AddJobWidget::AddJobWidget(SystemInfo *systemInfo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddJobWidget)
{
    ui->setupUi(this);
    this->systemInfo = systemInfo;
    jobList = new QList<Job*>;
    connect(ui->confirmPushButton, &QPushButton::clicked, this, &AddJobWidget::confirmButtonClick);
}

AddJobWidget::~AddJobWidget()
{
    delete ui;
}
void AddJobWidget::confirmButtonClick(){
    if(ui->processNameLineEdit->text()==""||ui->runTimeComboBox->currentText()==""||ui->needRAMLineEdit->text()==""){
        QMessageBox::information(this, tr("添加作业失败"), tr("进程名、运行时间、所需内存不可为空"));
    }
    else if(ui->needRAMLineEdit->text().toInt()>systemInfo->totalSize-systemInfo->osSize){
        QMessageBox::information(this, tr("添加作业失败"), tr("进程所需内存不可为超过系统内存"));
    }
    else{
        Job* job = new Job;
        job->processName = ui->processNameLineEdit->text();
        job->runTime = ui->runTimeComboBox->currentText().toInt();
        job->priority = ui->priorityComboBox->currentText().toInt();
        job->need = ui->needRAMLineEdit->text().toInt();
        job->content = ui->contentLineEdit->text();
        jobList->append(job);
        emit addJobSignal();
    }
}
