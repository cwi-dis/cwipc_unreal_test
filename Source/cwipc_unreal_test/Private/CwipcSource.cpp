// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSource.h"
#include <chrono>


// Define as empty to get debug prints
#define DBG
// Define is if(0) to not get debug prints
// #define DBG if(0)


UCwipcSource::UCwipcSource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
      readerThread(nullptr),
      pc(nullptr),  
      pc_points(nullptr),
      pc_points_count(0)

{
    DBG UE_LOG(LogTemp, Display, TEXT("UCwipcSource[%s]::UCwipcSource() called"), *GetPathNameSafe(this));
}


void UCwipcSource::_CleanupEverything()
{
    FScopeLock lock(&pc_lock);
    // xxxjack _CleanupEverything should also be called during destroction, after editing a source, etc.
    if (readerThread) {
        readerThread->Stop();
        readerThread = nullptr;
    }
    
    if (pc != nullptr) {
        pc->free();
        pc = nullptr;
    }
    if (pc_points != nullptr) {
        free(pc_points);
        pc_points = nullptr;
        pc_points_count = 0;
    }
}

void UCwipcSource::PostInitProperties()
{
    Super::PostInitProperties();
}

void UCwipcSource::PostLoad()
{
    Super::PostLoad();
    DBG UE_LOG(LogTemp, Display, TEXT("UCwipcSource[%s]::PostLoad() called"), *GetPathNameSafe(this));
    _CleanupEverything();
}

void UCwipcSource::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);
    DBG UE_LOG(LogTemp, Display, TEXT("UCwipcSource::PostEditChangeProperty() on 0x% called"), (void *)this);
    _CleanupEverything();
}

void UCwipcSource::BeginDestroy()
{
    Super::BeginDestroy();
    DBG UE_LOG(LogTemp, Display, TEXT("UCwipcSource[%s]::BeginDestroy() called"), *GetPathNameSafe(this));
    _CleanupEverything();
}

cwipc_source* UCwipcSource::_AllocateSource()
{
    char* errorMessage = nullptr;
    cwipc_source* source = cwipc_synthetic(synthetic_wanted_fps, synthetic_wanted_pointcount, &errorMessage, CWIPC_API_VERSION);
    if (source == nullptr)
    {
        if (errorMessage)
        {
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource[%s]: cwipc_synthetic() returned error: %s"), *GetPathNameSafe(this), *FString(errorMessage));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource[%s]: cwipc_synthetic() returned null, but no error message"), *GetPathNameSafe(this));
        }
    }
    return source;
}

bool UCwipcSource::_AllocateReaderThread()
{
    cwipc_source* source = _AllocateSource();
    if (source == nullptr)
    {
        // _AllocateSource will have given an error message
        return false;
    }
    readerThread = new FCwipcReaderThread(source, readerQueue);
    readerThread->StartThread();
    DBG UE_LOG(LogTemp, Display, TEXT("UcwipcSource[%s]: created point cloud source"), *GetPathNameSafe(this));
    return true;
}

bool UCwipcSource::InitializeSource()
{
    FScopeLock lock(&thread_lock);
    if (readerThread != nullptr) {
        return true;
    }
    
    if (pc != nullptr) {
        pc->free();
        pc = nullptr;
    }
    if (pc_points != nullptr)
    {
        free(pc_points);
        pc_points = nullptr;
        pc_points_count = 0;
    }
    pc_first_timestamp = -1;
    return _AllocateReaderThread();
}

bool UCwipcSource::LockPointCloud()
{
    return _CheckForNewPointCloudAvailable();
}

bool UCwipcSource::_CheckForNewPointCloudAvailable()
{
    FScopeLock lock(&pc_lock);
    if (readerThread == nullptr) {
        // xxxjack DBG UE_LOG(LogTemp, Warning, TEXT("UcwipcSource[%s]: _CheckForNewPointCloudAvailable: source == NULL, Initializing"), *GetPathNameSafe(this));
        // xxxjack InitializeSource();
        return false;
    }

    if (readerQueue.IsEmpty())
    {
        return false;
    }
    cwipc* new_pc = nullptr;
    if (!readerQueue.Dequeue(new_pc)) 
    {
        // Should not happen?
        return false;
    }
    // If a new pointcloud is available we get it.
    // First we release the old one.
    if (pc != nullptr)
    {
        cwipc_free(pc);
        pc = nullptr;
    }
    if (pc_points != nullptr)
    {
        free(pc_points);
        pc_points = nullptr;
        pc_points_count = 0;
    }
    pc = new_pc;
    if (pc_first_timestamp < 0) {
        pc_first_timestamp = pc->timestamp();
    }
    if (pc == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("UCwpicSource::_CheckForNewPointCloudAvailable: available returned true but no point cloud available"));
        return false;
    }
    pc_points_count = pc->count();
    int32 byte_count = pc->get_uncompressed_size();
    pc_points = (cwipc_point*)malloc(byte_count);
    if (pc_points == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("UCwpicSource::_CheckForNewPointCloudAvailable: malloc(%d) failed"), byte_count);
        // For consistency we also free the pointcloud.
        pc->free();
        pc = nullptr;
        pc_points_count = 0;
        return false;
    }
    int32 copied_count = pc->copy_uncompressed(pc_points, byte_count);
    if (pc_points_count != copied_count) {
        UE_LOG(LogTemp, Error, TEXT("UCwpicSource::_CheckForNewPointCloudAvailable: copy_uncompressed copied wrong number of points. Wanted %d, got %d"), pc_points_count, copied_count);
        // For consistency we also free the pointcloud.
        pc->free();
        pc = nullptr;
        free(pc_points);
        pc_points = nullptr;
        pc_points_count = 0;
        return false;
    }
    return pc != nullptr;
}

bool UCwipcSource::_ValidPointCloudAvailable()
{
    return pc != nullptr;
}

int32 UCwipcSource::GetNumberOfPoints()
{
    //  const std::clock_t start = std::clock();
    FScopeLock lock(&pc_lock);
    if (!_ValidPointCloudAvailable()) {
		return 0;
	}
    int32 rv = pc_points_count;
    // const std::clock_t end = std::clock();
    //  
// UE_LOG(LogTemp, Display, TEXT("UCwipcSource[%s]::GetNumberOfPoints() took %f ms"), *GetPathNameSafe(this), 1000.0 * (end - start) / CLOCKS_PER_SEC);
    return rv;

}

int32 UCwipcSource::GetTimeStamp()
{
    FScopeLock lock(&pc_lock);
    if (!_ValidPointCloudAvailable()) {
        return 0;
    }
    return pc->timestamp() - pc_first_timestamp;
}

float UCwipcSource::GetParticleSize()
{
    FScopeLock lock(&pc_lock);
    if (!_ValidPointCloudAvailable()) {
        return 0;
    }
    float cellsize = pc->cellsize();
    if (cellsize == 0) {
        cellsize = defaultCellSize;
    }
    return pc->cellsize() * particle_size_factor;
}



cwipc_point* UCwipcSource::GetPoint(int32 index)
{
    FScopeLock lock(&pc_lock);
    static cwipc_point nullpoint {1.0f, 1.0f, 1.0f, 112, 54, 25, 0};   
    if (pc_points == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("UCwipcSource::GetPoint: pc_points is null"));
		return &nullpoint;
	}
    if (index < 0 || index >= pc_points_count) {
		UE_LOG(LogTemp, Error, TEXT("UCwipcSource::GetPoint: index %d out of range %d"), index, pc_points_count);
		return &nullpoint;
	}
    return &pc_points[index];
}

FCwipcReaderThread::FCwipcReaderThread(cwipc_source* _source, TCircularQueue<cwipc*>& _queue)
:   source(_source),
    queue(_queue)
{
}

void FCwipcReaderThread::StartThread() {
    Thread = FRunnableThread::Create(this, TEXT("CwipcReaderThread"));
}

bool FCwipcReaderThread::Init()
{
    return true;
}

uint32 FCwipcReaderThread::Run()
{
    while (!bShutdown && source != nullptr) {
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
    return 0;
}

void FCwipcReaderThread::Exit()
{
    if (source) {
        source->free();
    }
}

void FCwipcReaderThread::Stop()
{
    bShutdown = true;
    cwipc* pc;
    while (queue.Dequeue(pc)) {
        pc->free();
    }
}
