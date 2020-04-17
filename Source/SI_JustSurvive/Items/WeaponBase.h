// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include <Engine/Texture.h>
#include "WeaponBase.generated.h"

/*
	Change Log:
			2/10/2020: Nick - I changed purchase to take a player controller
*/

UENUM()
enum ElementType
{
	Element0 UMETA(DisplayName = "ElementZero"), 
	Element1 UMETA(DisplayName = "ElementOne"), 
	Element2 UMETA(DisplayName = "ElementTwo")
};

USTRUCT(BlueprintType)
struct FGunData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunData")
		int32 m_Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunData")
		int32 m_AmmoInClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunData")
		int32 m_ClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunData")
		int32 m_TotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GunData")
		float m_FireRate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GunData")
		UTexture* m_UITexture;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GunData")
		TEnumAsByte<ElementType> m_Element; 
};

UCLASS()
class SI_JUSTSURVIVE_API AWeaponBase : public AItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWeaponBase();

	UPROPERTY(VisibleAnywhere, Category = "Gun")
		bool bIsActive; 

protected:

	virtual void BeginPlay() override;

	//TODO: Change the projectile subclass of template to the actual projectile we plan to use.
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class ASI_JustSurviveProjectile> ProjectileClass;

	class ASI_JustSurviveCharacter* m_Character; 

	UPROPERTY(EditAnywhere, Category = "Gun")
		int m_AmmoInClip; 

	UPROPERTY(EditAnywhere, Category = "Gun")
		int m_ClipSize;

	UPROPERTY(EditAnywhere, Category = "Gun")
		int m_TotalAmmo;

	UPROPERTY(EditAnywhere, Category = "Gun")
		float m_FireRate; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gun")
		FGunData m_GunData;

	FTimerHandle m_FireRateTimer; 

	//TODO: Add A Gun Data struct to represent: cost, image for ui, ammo in clip, clip size, fire rate, total ammo

public:
	virtual void Tick(float DeltaTime) override;

	void PickUp(class ASI_JustSurviveCharacter* character);
	void Equip();
	void Unequip();

	void Shoot();
	void PullTrigger(); 
	void ReleaseTrigger(); 
	void Reload(); 

	void AddAmmo(int32 amount); 

	bool Purchase(class ASI_PlayerController* character) override;

	UFUNCTION(BlueprintCallable)
		UTexture* GetGunTexture() { return m_GunData.m_UITexture; }

	UFUNCTION(BlueprintCallable)
		int32 GetTotalAmmo() { return m_GunData.m_TotalAmmo; }

	UFUNCTION(BlueprintCallable)
		int32 GetAmmoInClip() { return m_GunData.m_AmmoInClip; }

	UFUNCTION(BlueprintCallable)
		int32 GetClipSize() { return m_GunData.m_ClipSize; }

};
