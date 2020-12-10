#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include "system.h"
namespace Ui {
class Settings;
}

class Settings : public QWidget
{
    Q_OBJECT

public:
    explicit Settings(SystemInfo* systemInfo, QWidget *parent = 0);
    ~Settings();

private:
    Ui::Settings *ui;
};

#endif // SETTINGS_H
