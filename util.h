#ifndef UTIL_H
#define UTIL_H
#include "pcb.h"
#include "pool.h"
#include "system.h"
#include "subarea.h"
#include <QList>
#include <iostream>
//priority降序排序
void sortPCB(QList<PCB *> *list);//PCB为头结点
//PID升序排序
void sortPCBByPID(QList<PCB*> *PCBList);
//得到空闲PID
int getPID(int num, bool pid[]);//false表示被占用
//从后备队列中添加进程
void addProcess(QList<Job *> *list, QList<PCB *> *PCBList, SystemInfo* systemInfo);
//
bool addProcess(QList<Job*> *list, int i, QList<PCB *> *PCBList, QList<Subarea*> *subareaList,SystemInfo* systemInfo);
//根据PID得到PCB
int getIndexByPID(int PID, QList<PCB*> *list);
//copy List
void copyList(QList<PCB*> *nowList, QList<PCB*> *originList);
//从总PCBList中筛选出不同状态list
QList<PCB*>* filterList(QList<PCB*> *PCBList, int status);
//最先适应算法分配内存
bool firstFit(QList<Subarea*> *subareaList, PCB* pcb);
//释放内存，合并分区
QList<Subarea*>* mergeSubarea(QList<Subarea*> *subareaList, PCB* pcb);
#endif // UTIL_H
