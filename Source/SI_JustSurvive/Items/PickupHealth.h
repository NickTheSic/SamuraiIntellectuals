// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PickupHealth.generated.h"

/**
 * 
 */
UCLASS()
class SI_JUSTSURVIVE_API APickupHealth : public AItemBase
{
	GENERATED_BODY()

public:
	APickupHealth(); 

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void AddHealth(AActor* Player); 

protected:
	//virtual void BeginPlay(); 
};
