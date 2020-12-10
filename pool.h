#ifndef POOL_H
#define POOL_H
#include <QString>
struct Job{
    QString processName;
    QString content;
    int priority;
    int runTime;
    int need;
};

#endif // POOL_H
