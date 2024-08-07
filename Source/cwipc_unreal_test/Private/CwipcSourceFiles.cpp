// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSourceFiles.h"


bool UCwipcSourceFiles::_AllocateReaderThread()
{
    return false;
}


FCwipcFileReaderThread::FCwipcFileReaderThread(FString _pathName, TCircularQueue<cwipc*>& _queue)
:   FCwipcReaderThread(nullptr, _queue),
    pathName(_pathName)
{
}

uint32 FCwipcFileReaderThread::Run()
{
#if 0
    while (!bShutdown) {
        if (source->eof()) {
            return 1;
        }
        if (source->available(true)) {
            cwipc* pc = source->get();
            if (pc == nullptr) {
                UE_LOG(LogTemp, Error, TEXT("FCwipcReaderThread::Run: get() returned NULL"));
                return 2;
            }
            if (!queue.Enqueue(pc)) {
                pc->free();
                UE_LOG(LogTemp, Warning, TEXT("FCwipcReaderThread::Run: dropped point cloud, queue full"));
            }
        }
    }
#endif
    return 0;
}

void FCwipcFileReaderThread::Exit()
{
 
}
