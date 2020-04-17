// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "TowerBase.generated.h"

/*
	Change Log:
			2/17/2020: Nick - I added an InitialTower function.  The purpose of this is to set make sure all the data is set
							- I added it because the way that I have things setup is odd

			2/10/2020: Nick - I changed purchase to take a player controller
*/

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
		class UStaticMeshComponent* m_TowerMesh;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UPawnSensingComponent* m_PawnSensingComp;

	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
		class USceneComponent* m_MuzzleLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectiles")
		TSubclassOf<class ASI_JustSurviveProjectile>m_ProjectileTemplate;

	FTimerHandle SpawnProjectileTimer;

	bool bCanShoot;

	ATowerBase();

	void InitializeTower();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);
	
public:
	void ShootProjectile();

	virtual bool Purchase(ASI_PlayerController* player);
};
