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

cwipc* FCwipcFileReaderThread::_ReadNextPointCloud(std::string filename)
{
    std::filesystem::path filename_p(filename);
    std::filesystem::path ext = filename_p.extension();
    const char* filename_c = filename.c_str();
    cwipc* pc = nullptr;
    char* errorMessage = nullptr;
    switch (fileType) {
    case CwipcFileType::ply:
        UE_LOG(LogTemp, Display, TEXT("CwipcFileReaderThread: read ply from %s"), *FString(filename_c));
        pc = cwipc_read(filename_c, 424242, &errorMessage, CWIPC_API_VERSION);
        break;
    case CwipcFileType::cwipcdump:
        UE_LOG(LogTemp, Display, TEXT("CwipcFileReaderThread: read cwipcdump from %s"), *FString(filename_c));
        pc = cwipc_read_debugdump(filename_c, &errorMessage, CWIPC_API_VERSION);
        break;
    case CwipcFileType::cwicpc:
        UE_LOG(LogTemp, Display, TEXT("CwipcFileReaderThread: read cwicpc from %s"), *FString(filename_c));
        break;
    default:
        // Don't print error, has been done before.
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

void FCwipcFileReaderThread::_Initialize()
{
    std::filesystem::path pathname_p(pathName);
    fileType = CwipcFileType::unknown;
    if (std::filesystem::is_directory(pathname_p))
    {
        for (auto direntry : std::filesystem::directory_iterator(pathname_p))
        {
            std::string entry_s(direntry.path().string());
            CwipcFileType thisType = _GetFileType(entry_s);
            if (thisType == CwipcFileType::unknown) continue;
            fileType = fileType | thisType;
            if (thisType != CwipcFileType::unknown) {
                allFileNames.push_back(entry_s);
            }
        }
    }
    else
    {
        allFileNames.push_back(pathName);
        fileType = _GetFileType(pathName);
    }
    if (fileType == CwipcFileType::unknown)
    {
        UE_LOG(LogTemp, Error, TEXT("FCwipcFileReaderThread: No supported file types in %s"), *FString(pathName.c_str()));
    }
    else if (fileType != CwipcFileType::ply && fileType != CwipcFileType::cwipcdump && fileType != CwipcFileType::cwicpc)
    {
        UE_LOG(LogTemp, Error, TEXT("FCwipcFileReaderThread: Multiple file types in %s"), *FString(pathName.c_str()));

    }
}

CwipcFileType FCwipcFileReaderThread::_GetFileType(std::string filename)
{
    std::filesystem::path filename_p(filename);
    std::filesystem::path ext = filename_p.extension();
    if (ext == std::filesystem::path(".ply")) {
        return CwipcFileType::ply;
    }
    else if (ext == std::filesystem::path(".cwipcdump")) {
        return CwipcFileType::cwipcdump;
    }
    else if (ext == std::filesystem::path(".cwicpc")) {
        return CwipcFileType::cwicpc;
    }
    return CwipcFileType::unknown;
}

uint32 FCwipcFileReaderThread::Run()
{
    _Initialize();
    for (auto fileName : allFileNames) {

        cwipc* pc = _ReadNextPointCloud(fileName);
        if (pc != nullptr)
        {
            while (queue.IsFull())
            {
                if (bShutdown) {
                    return 0;
                }
                FPlatformProcess::Sleep(0.033);
            }
            queue.Enqueue(pc);
        }
        if (bShutdown) return 0;
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
