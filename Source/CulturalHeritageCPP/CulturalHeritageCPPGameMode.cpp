// Copyright Epic Games, Inc. All Rights Reserved.

#include "CulturalHeritageCPPGameMode.h"
#include "CulturalHeritageCPPHUD.h"
#include "CulturalHeritageCPPCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACulturalHeritageCPPGameMode::ACulturalHeritageCPPGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACulturalHeritageCPPHUD::StaticClass();
}
