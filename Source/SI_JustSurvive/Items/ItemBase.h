// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

USTRUCT()
struct SI_JUSTSURVIVE_API FShopData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "ShopData")
		FName m_ItemName; 

	UPROPERTY(EditAnywhere, Category = "ShopData")
		int m_Cost; 

	UPROPERTY(EditAnywhere, Category = "ShopData")
		class UStaticMesh* m_StaticMesh;

	UPROPERTY(EditAnywhere, Category = "ShopData")
		class UTexture2D* m_Texture; 

	bool bIsInShop; 
};

UCLASS()
class SI_JUSTSURVIVE_API AItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItemBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual bool Purchase(class ASI_JustSurviveCharacter* character);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		class UStaticMeshComponent* m_StaticMesh; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		class UBoxComponent* m_BoxComponent; 
};
