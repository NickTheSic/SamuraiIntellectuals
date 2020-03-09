// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SI_JustSurviveHUD.generated.h"

UCLASS()
class ASI_JustSurviveHUD : public AHUD
{
	GENERATED_BODY()

public:
	ASI_JustSurviveHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

	UPROPERTY(EditAnywhere, Category = "Game Hud")
		TSubclassOf<class UUserWidget> PlayerGuiClass;

	UPROPERTY()
		class UUserWidget* PlayerGui;

protected:
	virtual void BeginPlay() override;

private:
	/** Crosshair asset pointer */
	class UTexture2D* CrosshairTex;

};

