#include "settings.h"
#include "ui_settings.h"

Settings::Settings(SystemInfo *systemInfo, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Settings)
{
    ui->setupUi(this);
    ui->numLineEdit->setText(QString::number(systemInfo->num));
    ui->OSSizeLineEdit->setText(QString::number(systemInfo->osSize));
    ui->OSStartLineEdit->setText(QString::number(systemInfo->osStartAddr));
    ui->RAMLineEdit->setText(QString::number(systemInfo->totalSize));
    ui->numLineEdit->setText(QString::number(systemInfo->num));
}

Settings::~Settings()
{
    delete ui;
}
