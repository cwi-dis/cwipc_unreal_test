// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSource.h"

UCwipcSource::UCwipcSource(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
      source(nullptr),
      pc(nullptr),  
      pc_points(nullptr), 
      pc_count(0)

{
	UE_LOG(LogTemp, Warning, TEXT("xxxJack UCwipcSource constructor 0x%x"),(uint64) this);
}

void UCwipcSource::_initialize()
{
    source = cwipc_synthetic(15, 100, nullptr, CWIPC_API_VERSION);
    if (source == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("xxxjack UCwpicSource: cwipc_synthetic returned null"));
    }
    pc = nullptr;
    pc_points = nullptr;
    pc_count = 0;

    UE_LOG(LogTemp, Warning, TEXT("xxxJack UcwipcSource has been initialized"));
}

int32 UCwipcSource::GetNumberOfPoints()
{
    UE_LOG(LogTemp, Warning, TEXT("xxxJack GetNumberOfPoints is called on 0x%x"),(uint64) this);
    if (source == nullptr) {
        _initialize();
    }
    if (source == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("xxxjack UCwpicSource::GetNumberOfPoints: source is null"));
        return 0;
    }
    if ( pc != nullptr) {
		cwipc_free(pc);
		pc = nullptr;
	}
    pc = source->get();
    if (pc == nullptr) {
        UE_LOG(LogTemp, Error, TEXT("xxxjack available returned true but no point cloud available"));
        return 0;
    }
    pc_count = pc->count();
    pc_timestamp = pc->timestamp();
    UE_LOG(LogTemp, Display, TEXT("xxxjack UCwipcNiagaraDataInterface::PerInstanceTick: got pointcloud with %d points"), pc->count());
#if 0
    int32 byte_count = pc->get_uncompressed_size();
    pc_points = (cwipc_point*)malloc(byte_count);
    int32 copied_count = pc->copy_uncompressed(pc_points, byte_count);
    if (byte_count != copied_count) {
        UE_LOG(LogTemp, Error, TEXT("xxxjack copy_uncompressed copied wrong number of bytes"));
        free(pc_points);
        pc_points = nullptr;
        return 0;
    }
#endif
    UE_LOG(LogTemp, Warning, TEXT("xxxJack  pc_count is %d"), pc_count);
    return pc_count;
}

void UCwipcSource::PostInitProperties()
{
	Super::PostInitProperties();
	UE_LOG(LogTemp, Warning, TEXT("xxxJack Cwipcsource PostInitProperties"));

    source = cwipc_synthetic(15, 100, nullptr, CWIPC_API_VERSION);
    if (pc != nullptr) {
        cwipc_free(pc);
        pc = nullptr;
    }
    if (pc_points != nullptr) {
        free(pc_points);
        pc_points = nullptr;
    }
}

void UCwipcSource::PostLoad()
{
	Super::PostLoad();
	UE_LOG(LogTemp, Warning, TEXT("xxxJack Cwipcsource PostLoad"));
	if (source == nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("xxxjack UCwipcNiagaraDataInterface::PostLoad: cwipc_synthetic failed"));
	}
}