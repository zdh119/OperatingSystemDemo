#ifndef RAMWIDGET_H
#define RAMWIDGET_H

#include <QWidget>
#include "mainwindow.h"
#include "subarea.h"
#include "pcb.h"
class MainWindow;
namespace Ui {
class RAMWidget;
}

class RAMWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RAMWidget(QWidget *parent = 0);
    ~RAMWidget();

public slots:
    void showRAM(QList<Subarea*> *subareaList, QList<PCB*> *PCBList);
private:
    Ui::RAMWidget *ui;
};

#endif // RAMWIDGET_H
