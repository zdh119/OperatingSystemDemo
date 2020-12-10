#include "util.h"
#include <QDebug>
void sortPCB(QList<PCB*> *list){
    for(int i=0;i<list->size();i++){
        for(int j=0;j<list->size()-1;j++){
            if(list->at(i)->priority>list->at(j)->priority){
                PCB* pcb1 =  list->at(i);
                PCB* pcb2 = list->at(j);
                list->replace(i, pcb2);
                list->replace(j, pcb1);
            }
        }
    }
}
void sortPCBByPID(QList<PCB *> *PCBList){
    for(int i=0;i<PCBList->size();i++){
        for(int j=0;j<PCBList->size()-1;j++){
            if(PCBList->at(i)->PID<PCBList->at(j)->PID){
                PCB* pcb1 =  PCBList->at(i);
                PCB* pcb2 = PCBList->at(j);
                PCBList->replace(i, pcb2);
                PCBList->replace(j, pcb1);
            }
        }
    }
}

int getPID(int num, bool pid[]){//false表示被占用
    int i=0;
    while(i<num){
        if(pid[i]){
            break;
        }
        i++;
    }

    return i;
}
//从后备队列中添加进程
void addProcess(QList<Job*> *list, QList<PCB*> *PCBList, SystemInfo* systemInfo){
    PCB* newPCB = new PCB;
    newPCB->content = list->first()->content;
    newPCB->processName = list->first()->processName;
    newPCB->priority = list->first()->priority;
    newPCB->runTime = list->first()->runTime;
    newPCB->status = READY;
    newPCB->PID = getPID(systemInfo->num, systemInfo->pid);
    systemInfo->pid[newPCB->PID] = false;
    if(PCBList->size()==0){
        newPCB->next = NULL;
        newPCB->previous = NULL;
        PCBList->append(newPCB);
    }
    else{
        PCBList->last()->next = newPCB;
        newPCB->previous = PCBList->last();
        newPCB->next = NULL;
        PCBList->append(newPCB);
    }

    list->removeFirst();
}
bool addProcess(QList<Job*> *list, int i, QList<PCB*> *PCBList, QList<Subarea*> *subareaList , SystemInfo* systemInfo){
    PCB* newPCB = new PCB;
    newPCB->need = list->at(i)->need;
    if(firstFit(subareaList, newPCB)){
        newPCB->status = READY;
        newPCB->content = list->at(i)->content;
        newPCB->processName = list->at(i)->processName;
        newPCB->priority = list->at(i)->priority;
        newPCB->runTime = list->at(i)->runTime;
        newPCB->PID = getPID(systemInfo->num, systemInfo->pid);
        systemInfo->pid[newPCB->PID] = false;
        if(PCBList->size()==0){
            newPCB->next = NULL;
            newPCB->previous = NULL;
            PCBList->append(newPCB);
        }
        else{
            PCBList->last()->next = newPCB;
            newPCB->previous = PCBList->last();
            newPCB->next = NULL;
            PCBList->append(newPCB);
        }
        list->removeAt(i);
        return true;
    }
    else{
        return false;
    }

}
int getIndexByPID(int PID, QList<PCB*> *list){
    if(list!=NULL){
        for(int i=0;i<list->size();i++){
            if(list->at(i)->PID==PID){
                return i;
            }
        }
    }
    return -1;

}
void copyList(QList<PCB *> *nowList, QList<PCB *> *originList){
    nowList->clear();
    for(int i=0;i<originList->size();i++){
        nowList->append(originList->at(i));
    }
}
QList<PCB*>* filterList(QList<PCB *> *PCBList, int status){
    QList<PCB*> *current = new QList<PCB*>;
    for(int i=0;i<PCBList->size();i++){
        if(PCBList->at(i)->status == status){
            current->append(PCBList->at(i));
        }
    }
    return current;
}

bool firstFit(QList<Subarea *> *subareaList, PCB *pcb){
    bool isAllocated = false;
    for(int i=0;i<subareaList->size();i++){
        if(subareaList->at(i)->size>=pcb->need){
            pcb->startAddr = subareaList->at(i)->start;
            if(subareaList->at(i)->size==pcb->need){
                subareaList->removeAt(i);
            }
            else{
                subareaList->at(i)->start = subareaList->at(i)->start + pcb->need;
                subareaList->at(i)->size = subareaList->at(i)->size - pcb->need;
            }
            isAllocated = true;
            break;
        }
    }
    return isAllocated;
}
QList<Subarea *> *mergeSubarea(QList<Subarea *> *subareaList, PCB *pcb){
    if(subareaList->size()==0){
        Subarea* temp = new Subarea;
        temp->start = pcb->startAddr;
        temp->size = pcb->need;
        subareaList->append(temp);
    }
    else{
        bool isMerge[subareaList->size()] = {false};
        bool isRelease = true;
        for(int i=0;i<subareaList->size();i++){
            isRelease = true;
            Subarea* current = subareaList->at(i);
            if(pcb->startAddr+pcb->need < current->start){
                if(!isMerge[i-1]){
                    Subarea* newArea  = new Subarea;
                    newArea->start = pcb->startAddr;
                    newArea->size = pcb->need;
                    subareaList->insert(i, newArea);
                }
                break;
            }
            if(pcb->startAddr+pcb->need == current->start){

                if(isMerge[i-1]&&subareaList->at(i-1)->start+subareaList->at(i-1)->size == current->start){
                    subareaList->at(i-1)->size += current->size;
                    subareaList->removeAt(i);
                }
                else{
                    current->start = pcb->startAddr;
                    current->size += pcb->need;
                    subareaList->replace(i, current);
                }
                break;
            }
            if(current->start+current->size == pcb->startAddr){
                current->size += pcb->need;
                subareaList->replace(i, current);
                isMerge[i] = true;
            }
            if(current->start+current->size < pcb->startAddr){
                isRelease = false;
            }
        }
        if(!isRelease){
            Subarea* newArea  = new Subarea;
            newArea->start = pcb->startAddr;
            newArea->size = pcb->need;
            subareaList->append(newArea);
        }
    }
    return subareaList;
}








