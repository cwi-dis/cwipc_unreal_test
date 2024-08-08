// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <string>
#include <type_traits>

#include "CoreMinimal.h"
#include "CwipcSource.h"
#include "cwipc_codec/api.h"
#include "CwipcSourceFiles.generated.h"

#define ENUM_FLAG_OPERATORS(T)                                                                                                                                            \
    inline T operator~ (T a) { return static_cast<T>( ~static_cast<std::underlying_type<T>::type>(a) ); }                                                                       \
    inline T operator| (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) | static_cast<std::underlying_type<T>::type>(b) ); }                   \
    inline T operator& (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) & static_cast<std::underlying_type<T>::type>(b) ); }                   \
    inline T operator^ (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) ^ static_cast<std::underlying_type<T>::type>(b) ); }                   \
    inline T& operator|= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) |= static_cast<std::underlying_type<T>::type>(b) ); }   \
    inline T& operator&= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) &= static_cast<std::underlying_type<T>::type>(b) ); }   \
    inline T& operator^= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) ^= static_cast<std::underlying_type<T>::type>(b) ); }

class cwipc;

enum CwipcFileType { unknown = 0, ply = 1, cwipcdump = 2, cwicpc = 4 };
ENUM_FLAG_OPERATORS(CwipcFileType)

class FCwipcFileReaderThread : public FCwipcReaderThread
{
public:
	FCwipcFileReaderThread(FString _pathName, TCircularQueue<cwipc*>& _queue);
	virtual void StartThread() override;
	virtual uint32 Run() override;
	virtual void Exit() override;
	
protected:
	void _Initialize();
	enum CwipcFileType _GetFileType(std::string filename);
	cwipc* _ReadNextPointCloud(std::string filename);
	
	std::string pathName;
	CwipcFileType fileType;
	std::list<std::string> allFileNames;
	cwipc_decoder* decoder;


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
