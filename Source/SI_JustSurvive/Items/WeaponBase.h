// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
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

public:
	virtual void Tick(float DeltaTime) override;

	void PickUp(class ASI_JustSurviveCharacter* character);
	void Equip();
	void Unequip();

	void Shoot();

};
