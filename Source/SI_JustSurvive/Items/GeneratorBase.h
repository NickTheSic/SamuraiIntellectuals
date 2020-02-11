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

//ADDED START
	UFUNCTION()
		void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageInflictor);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DamageMesh")
		class USceneComponent* GeneratorRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "DamageMesh")
		class UStaticMeshComponent* GeneratorBody;

	int m_HP = 100;

//ADDED END

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
