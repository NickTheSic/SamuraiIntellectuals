// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Round.generated.h"

UCLASS()
class SI_JUSTSURVIVE_API ARound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARound();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Waves")
		TArray<TSubclassOf<class AWave>> m_Waves;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
