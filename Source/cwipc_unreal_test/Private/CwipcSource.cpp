// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSource.h"
#include <chrono>


// Define as empty to get debug prints
#define DBG
// Define is if(0) to not get debug prints
// #define DBG if(0)


UCwipcSource::UCwipcSource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
      source(nullptr),
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
    if (source != nullptr) {
        source->free();
        source = nullptr;
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

bool UCwipcSource::InitializeSource()
{
    FScopeLock lock(&source_lock);
    if (source != nullptr) {
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
    char* errorMessage = nullptr;
    source = cwipc_synthetic(synthetic_wanted_fps, synthetic_wanted_pointcount, &errorMessage, CWIPC_API_VERSION);
    if (source == nullptr)
    {
        if (errorMessage)
        {
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource[%s]: cwipc_synthetic() returned error: %s"), *GetPathNameSafe(this), errorMessage);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource[%s]: cwipc_synthetic() returned null, but no error message"), *GetPathNameSafe(this));
        }
    }
    
    DBG UE_LOG(LogTemp, Display, TEXT("UcwipcSource[%s]: created cwipc_synthetic() source"), *GetPathNameSafe(this));
    return true;
}

bool UCwipcSource::_CheckForNewPointCloudAvailable()
{
    FScopeLock lock(&pc_lock);
    // xxxjack this method *must* be protected with a lock at some point...
    if (InitializeSource()) {
        if (source->available(false))
        {
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
            pc = source->get();
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
        }
    }
    return pc != nullptr;
}

int32 UCwipcSource::GetNumberOfPoints()
{
    //  const std::clock_t start = std::clock();
    FScopeLock lock(&pc_lock);
    if (!_CheckForNewPointCloudAvailable()) {
		return 0;
	}
 
    int32 rv = pc_points_count;
    // const std::clock_t end = std::clock();
    //  
// UE_LOG(LogTemp, Display, TEXT("UCwipcSource[%s]::GetNumberOfPoints() took %f ms"), *GetPathNameSafe(this), 1000.0 * (end - start) / CLOCKS_PER_SEC);
    return rv;

}

float UCwipcSource::GetParticleSize()
{
    FScopeLock lock(&pc_lock);
    if (!_CheckForNewPointCloudAvailable()) {
        return 0;
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
