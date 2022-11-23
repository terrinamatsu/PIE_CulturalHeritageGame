// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "CulturalHeritageCPPHUD.generated.h"

UCLASS()
class ACulturalHeritageCPPHUD : public AHUD
{
	GENERATED_BODY()

public:
	ACulturalHeritageCPPHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

