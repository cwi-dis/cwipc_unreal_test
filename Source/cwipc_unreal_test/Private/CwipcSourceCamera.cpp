// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSourceCamera.h"
#include "cwipc_kinect/api.h"

cwipc_source* UCwipcSourceCamera::_AllocateSource()
{
    char* errorMessage = nullptr;
    char* configFileName = nullptr;
    if(!ConfigFileName.IsEmpty())
	{
		configFileName = TCHAR_TO_ANSI(*ConfigFileName);
	}

    cwipc_kinect(nullptr, nullptr, 0); // Force loading of kinect dlls
    cwipc_source* source = cwipc_capturer(configFileName, &errorMessage, CWIPC_API_VERSION);
    if (source == nullptr)
    {
        if (errorMessage)
        {
            FString message(errorMessage);
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource[%s]: cwipc_capturer() returned error: %s"), *GetPathNameSafe(this), *message);
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("UCwpicSource[%s]: cwipc_capturer() returned null, but no error message"), *GetPathNameSafe(this));
        }
    }
    return source;
}
