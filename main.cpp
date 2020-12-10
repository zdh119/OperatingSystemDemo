#include "initwidget.h"
#include <QApplication>
#include <QFile>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style.qss");
    file.open(QFile::ReadOnly);
    if(!file.isOpen()){
        qDebug()<<"file failed open";
    }
    else{
        QString styleSheet = QString::fromLatin1(file.readAll());
        qApp->setStyleSheet(styleSheet);
        InitWidget* w = new InitWidget;
        w->show();
    }
    return a.exec();
}
