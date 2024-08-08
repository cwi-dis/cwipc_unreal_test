// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>

#include "CoreMinimal.h"
#include "CwipcSource.h"
#include "CwipcSourceFiles.generated.h"

class cwipc;

class FCwipcFileReaderThread : public FCwipcReaderThread
{
public:
	FCwipcFileReaderThread(FString _pathName, TCircularQueue<cwipc*>& _queue);
	virtual void StartThread() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	virtual cwipc* ReadNextPointCloud();
protected:
	std::string pathName;
	std::string nextFileName;
};
/**
 * 
 */
UCLASS()
class CWIPC_UNREAL_TEST_API UCwipcSourceFiles : public UCwipcSource
{
	GENERATED_BODY()

public:
	virtual bool _AllocateReaderThread() override;

	UPROPERTY(EditAnywhere, Category = "Cwipc Niagara", meta = (DisplayName = "Path to point cloud file or directory"))
	FString PathName;
};
