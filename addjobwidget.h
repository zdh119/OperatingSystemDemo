#ifndef ADDJOBWIDGET_H
#define ADDJOBWIDGET_H

#include <QWidget>
#include <QList>
#include <QMessageBox>
#include "pool.h"
#include "system.h"
namespace Ui {
class AddJobWidget;
}

class AddJobWidget : public QWidget
{
    Q_OBJECT

public:
    explicit AddJobWidget(SystemInfo* systemInfo,QWidget *parent = 0);
    ~AddJobWidget();

    QList<Job*> *jobList;
    SystemInfo* systemInfo;
signals:
    void addJobSignal();
protected slots:
    void confirmButtonClick();

private:
    Ui::AddJobWidget *ui;
};

#endif // ADDJOBWIDGET_H
