// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class AWeaponBase; 

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SI_JUSTSURVIVE_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

protected:
	AWeaponBase* m_CurrentWeapon; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Inventory")
		TArray< AWeaponBase* > m_WeaponArray;

public:
	void NextWeapon(); 

	void PreviousWeapon(); 

	void SetCurrentWeapon( AWeaponBase* weapon );  

	int GetWeaponCount(); 

	void AddWeaponToInventory( AWeaponBase* weapon ); 

	UFUNCTION(BlueprintCallable)
		AWeaponBase* GetCurrentWeapon() { return m_CurrentWeapon; }
};
