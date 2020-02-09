// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TowerTerminal.generated.h"

UCLASS()
class SI_JUSTSURVIVE_API ATowerTerminal : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Terminal")
		class UBoxComponent* m_BoxComponent; 

	UPROPERTY(EditAnywhere, Category = "Terminal")
		class UStaticMeshComponent* m_StaticMesh;

public:	
	// Sets default values for this actor's properties
	ATowerTerminal();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
