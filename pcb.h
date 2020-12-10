#ifndef PCB_H
#define PCB_H
#include <QString>
enum status_value{
    READY,
    SUSPENDING,
    RUNNING,
    ENDING
};
const QString statusArray[] = {
    "READY",
    "SUSPENDING",
    "RUNNING",
    "ENDING"
};
struct PCB{
        int PID;
        int runTime;
        int priority;
        int status;//0-3
        int need;
        int startAddr;
        QString processName;
        QString content;
        PCB* previous;
        PCB* next;

};

#endif // PCB_H
