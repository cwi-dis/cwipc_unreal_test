// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSourceFiles.h"
#include "Misc/Paths.h"
#include <string>
#include <filesystem>

// Define as empty to get debug prints
#define DBG
// Define is if(0) to not get debug prints
// #define DBG if(0)

bool UCwipcSourceFiles::_AllocateReaderThread()
{
    readerThread = new FCwipcFileReaderThread(PathName, readerQueue);
    readerThread->StartThread();
    DBG UE_LOG(LogTemp, Display, TEXT("UcwipcSource[%s]: created point cloud source"), *GetPathNameSafe(this));
    return true;
}


FCwipcFileReaderThread::FCwipcFileReaderThread(FString _pathName, TCircularQueue<cwipc*>& _queue)
:   FCwipcReaderThread(nullptr, _queue)
{
    std::filesystem::path partial(TCHAR_TO_UTF8(*_pathName));
    if (partial.is_relative())
    {
        FString projectDir = FPaths::ProjectDir();
        std::filesystem::path base(TCHAR_TO_UTF8(*projectDir));
        partial = base / partial;
    }
    std::filesystem::path abs = std::filesystem::absolute(partial);
    pathName = abs.string();
    bool ok = std::filesystem::exists(pathName);
    
}


void FCwipcFileReaderThread::StartThread() {
    Thread = FRunnableThread::Create(this, TEXT("CwipcFileReaderThread"));
}

cwipc* FCwipcFileReaderThread::ReadNextPointCloud()
{
    std::string filename_s(nextFileName);
    std::filesystem::path filename(filename_s);
    std::filesystem::path ext = filename.extension();
    const char* filename_c = filename_s.c_str();
    cwipc* pc = nullptr;
    char* errorMessage = nullptr;
    if (ext == std::filesystem::path(".ply")) {
        UE_LOG(LogTemp, Display, TEXT("xxxjack should read ply"));
        pc = cwipc_read(filename_c, 424242, &errorMessage, CWIPC_API_VERSION);
    }
    else if (ext == std::filesystem::path(".cwipcdump")) {
        UE_LOG(LogTemp, Display, TEXT("xxxjack should read cwipcdump"));
        pc = cwipc_read_debugdump(filename_c, &errorMessage, CWIPC_API_VERSION);
    }
    else if (ext == std::filesystem::path(".cwicpc")) {
        UE_LOG(LogTemp, Display, TEXT("xxxjack should read cwicpc"));
    }
    else {
        UE_LOG(LogTemp, Error, TEXT("FCwipcFileReaderThread: Unknown extension '%s' in '%s' "), *FString(ext.c_str()), *FString(filename.c_str()));
        return nullptr;
    }
    if (pc == nullptr) {
        if (errorMessage)
        {
            UE_LOG(LogTemp, Error, TEXT("FCwipcFileReaderThread: reader returned error: %s"), *FString(errorMessage));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("FCwipcFileReaderThread: reader returned null, but no error message"));
        }
    }
    return pc;
}

uint32 FCwipcFileReaderThread::Run()
{
    nextFileName = pathName;
    cwipc* pc = ReadNextPointCloud();
    if (pc != nullptr)
    {
        queue.Enqueue(pc);
    }
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
