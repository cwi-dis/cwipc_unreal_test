// Copyright Epic Games, Inc. All Rights Reserved.

#include "cwipc_unreal_testGameMode.h"
#include "cwipc_unreal_testCharacter.h"
#include "UObject/ConstructorHelpers.h"

Acwipc_unreal_testGameMode::Acwipc_unreal_testGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
