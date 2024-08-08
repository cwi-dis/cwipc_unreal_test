// Fill out your copyright notice in the Description page of Project Settings.


#include "CwipcSourceCamera.h"
#include "cwipc_realsense2/api.h"
#undef cwipc_realsense_api_h
#include "cwipc_kinect/api.h"
#include <filesystem>

cwipc_source* UCwipcSourceCamera::_AllocateSource()
{
    char* errorMessage = nullptr;
    std::string configFilename_s;
    const char* configFileName = nullptr;
    if(!ConfigFileName.IsEmpty())
	{
		std::filesystem::path partial(TCHAR_TO_UTF8(*ConfigFileName));
        if (partial.is_relative())
        {
            FString projectDir = FPaths::ProjectDir();
            std::filesystem::path base(TCHAR_TO_UTF8(*projectDir));
            partial = base / partial;
        }
        std::filesystem::path abs = std::filesystem::absolute(partial);
        configFilename_s = abs.string();
        configFileName = configFilename_s.c_str();
	}

    cwipc_kinect(nullptr, nullptr, 0); // Force loading of kinect dlls
    cwipc_realsense2(nullptr, nullptr, 0);
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
