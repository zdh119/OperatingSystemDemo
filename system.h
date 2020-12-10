#ifndef SYSTEM_H
#define SYSTEM_H
struct SystemInfo{
    int osStartAddr;
    int osSize;
    int totalStartAddr;
    int totalSize;
    int num;//道数
    int remainNum;//剩余的道数
    bool* pid;
};

#endif // SYSTEM_H
