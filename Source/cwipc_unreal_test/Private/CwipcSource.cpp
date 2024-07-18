// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSource.h"



UCwipcSource::UCwipcSource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
      source(nullptr),
      pc(nullptr),  
      pc_points(nullptr), 
      pc_count(0)

{
	//UE_LOG(LogTemp, Warning, TEXT("xxxJack UCwipcSource constructor 0x%x"),(uint64) this);
}

void UCwipcSource::_initialize()
{
    if (source) {
        source->free();
        source = nullptr;
    }
    if (pc) {
        pc->free();
        pc = nullptr;
    }
    if (pc_points) {
        free(pc_points);
        pc_points = nullptr;
    }
    pc_count = 0;
    pc_timestamp = 0;
    source = cwipc_synthetic(15, 1000, nullptr, CWIPC_API_VERSION);
    if (source == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("xxxjack UCwpicSource: cwipc_synthetic returned null"));
    }
    
    UE_LOG(LogTemp, Warning, TEXT("xxxJack UcwipcSource: created cwipc_synthetic() source"));
}

int32 UCwipcSource::GetNumberOfPoints()
{
    //UE_LOG(LogTemp, Warning, TEXT("xxxJack UCwpicSource::GetNumberOfPoints: is called on 0x%x"),(uint64) this);
    if (source == nullptr) {
        _initialize();
    }
    if (source == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("xxxjack UCwpicSource::GetNumberOfPoints: source is null"));
        return 0;
    }
    if (source->available(false)) {
        // If a new pointcloud is available we get it. Otherwise we use the old one.
        if (pc != nullptr) {
            cwipc_free(pc);
            pc = nullptr;
        }
        pc = source->get();
        if (pc == nullptr) {
            UE_LOG(LogTemp, Error, TEXT("xxxjack UCwpicSource::GetNumberOfPoints: available returned true but no point cloud available"));
            return 0;
        }
    }
    
    pc_count = pc->count();
    pc_timestamp = pc->timestamp();
    //UE_LOG(LogTemp, Display, TEXT("xxxjack UCwpicSource::GetNumberOfPoints: got pointcloud with %d points"), pc->count());
    if (pc_points) {
        free(pc_points);
        pc_points = nullptr;
    }
    int32 byte_count = pc->get_uncompressed_size();
    pc_points = (cwipc_point*)malloc(byte_count);
    if (pc_points == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("xxxjack UCwpicSource::GetNumberOfPoints: malloc(%d) failed"), byte_count);
        return 0;
    }
    int32 copied_count = pc->copy_uncompressed(pc_points, byte_count);
    if (pc_count != copied_count) {
        UE_LOG(LogTemp, Error, TEXT("xxxjack UCwpicSource::GetNumberOfPoints: copy_uncompressed copied wrong number of points. Wanted %d, got %d"), pc_count, copied_count);
        free(pc_points);
        pc_points = nullptr;
        return 0;
    }
    //UE_LOG(LogTemp, Warning, TEXT("xxxJack UCwpicSource::GetNumberOfPoints:  pc_count is %d"), pc_count);
    //AllPoints(); // Call the AllPoints function
    return pc_count;
}



cwipc_point* UCwipcSource::GetPoint(int32 index) const
{
    if (pc_points == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("xxxjack UCwipcSource::GetPoint: pc_points is null"));
		return nullptr;
	}
    if (index < 0 || index >= pc_count) {
		UE_LOG(LogTemp, Error, TEXT("xxxjack UCwipcSource::GetPoint: index %d out of range %d"), index, pc_count);
		return nullptr;
	}
    return &pc_points[index];
}

void UCwipcSource::PostInitProperties()
{
	Super::PostInitProperties();
	//UE_LOG(LogTemp, Warning, TEXT("xxxJack Cwipcsource PostInitProperties"));
}

void UCwipcSource::PostLoad()
{
	Super::PostLoad();
	//UE_LOG(LogTemp, Warning, TEXT("xxxJack Cwipcsource PostLoad"));
	if (source == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("xxxjack UCwipcNiagaraDataInterface::PostLoad: cwipc_synthetic failed"));
	}
}
