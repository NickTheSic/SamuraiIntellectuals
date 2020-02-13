// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundManager.generated.h"

UCLASS()
class SI_JUSTSURVIVE_API ARoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Rounds")
		TArray<TSubclassOf<class ARound>> m_Rounds;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
