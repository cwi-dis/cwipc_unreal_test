// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSource.h"



UCwipcSource::UCwipcSource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
      source(nullptr),
      pc(nullptr),  
      pc_points(nullptr),
      pc_points_count(0)

{
    UE_LOG(LogTemp, Display, TEXT("UCwipcSource::UCwipcSource() on 0x%p called"), (void *)this);
}


void UCwipcSource::_CleanupEverything()
{
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
    UE_LOG(LogTemp, Display, TEXT("UCwipcSource::PostLoad() on 0x%p called"), (void *)this);
    _CleanupEverything();
}

void UCwipcSource::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    UE_LOG(LogTemp, Display, TEXT("UCwipcSource::PostEditChangeProperty() on 0x% called"), (void *)this);
    _CleanupEverything();
}

void UCwipcSource::BeginDestroy()
{
    UE_LOG(LogTemp, Display, TEXT("UCwipcSource::BeginDestroy() on 0x%p called"), (void *)this);
    _CleanupEverything();
}

bool UCwipcSource::_OptionalInitializeSource()
{
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
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource: cwipc_synthetic() returned error: %s"), errorMessage);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource: cwipc_synthetic() returned null, but no error message"));
        }
    }
    
    UE_LOG(LogTemp, Display, TEXT("UcwipcSource 0x%p: created cwipc_synthetic() source"), (void *)this);
    return true;
}

bool UCwipcSource::_CheckForNewPointCloudAvailable()
{
    // xxxjack this method *must* be protected with a lock at some point...
    if (_OptionalInitializeSource()) {
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
    if (!_CheckForNewPointCloudAvailable()) {
        return 0;
    }

    return pc->count();
}

float UCwipcSource::GetParticleSize()
{
    if (!_CheckForNewPointCloudAvailable()) {
        return 0;
    }
    return pc->cellsize() * particle_size_factor;
}



cwipc_point* UCwipcSource::GetPoint(int32 index) const
{
    if (pc_points == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("UCwipcSource::GetPoint: pc_points is null"));
		return nullptr;
	}
    if (index < 0 || index >= pc_points_count) {
		UE_LOG(LogTemp, Error, TEXT("UCwipcSource::GetPoint: index %d out of range %d"), index, pc_points_count);
		return nullptr;
	}
    return &pc_points[index];
}
