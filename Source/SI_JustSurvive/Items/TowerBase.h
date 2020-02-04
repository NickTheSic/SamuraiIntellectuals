// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "TowerBase.generated.h"

USTRUCT()
struct FTowerData
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY()
		int m_Damage;

	/* UPROPERTY()
		 EElementType m_DamageType;*/

	UPROPERTY()
		int m_FireRate;

	UPROPERTY()
		float m_TowerRange;

	UPROPERTY()
		int m_ResellValue;

	UPROPERTY()
		int m_UpgradeCost;

	/*UPROPERTY()
		ETowerType m_TowerType;*/

};

/**
 * 
 */
UCLASS()
class SI_JUSTSURVIVE_API ATowerBase : public AItemBase
{
	GENERATED_BODY()

public:
	FTowerData m_TowerData;

	UPROPERTY(Category = Mesh, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UStaticMeshComponent* TowerMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UPawnSensingComponent* m_PawnSensingComp;


	ATowerBase();

protected:
	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);
	
public:
	void ShootProjectile();

	virtual bool Purchase(ASI_JustSurviveCharacter* player);
};
