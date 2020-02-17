// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeneratorBase.generated.h"

UCLASS()
class SI_JUSTSURVIVE_API AGeneratorBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGeneratorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DamageMesh")
		class USceneComponent* m_ActorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DamageMesh")
		class UStaticMeshComponent* m_GenMesh;

	int m_HP = 150;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
