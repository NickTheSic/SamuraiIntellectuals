// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemBase.generated.h"

/*
	Change Log:
			2/17/2020: Nick - I added a setter for the shop information
							- I changed bIsInShop to be true

			2/11/2020: Nick - I added to the purchase funtion - It should now check the players money vs item cost. Needs to be tested
							- I added a sell function based of the purchase funtion logic
			2/10/2020: Nick - I changed purchase to take a player controller
*/

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
		class UTexture* m_Texture; 

	bool bIsInShop = true; //Items start in the shop
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

	virtual bool Purchase(class ASI_PlayerController* character);

	virtual void SellItem(class ASI_PlayerController* character);

	FShopData GetShopData();
	void SetShopData(FShopData newData);

	void SetIsInShop(bool isInShop);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		class UStaticMeshComponent* m_StaticMesh; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
		class UBoxComponent* m_BoxComponent; 

	UPROPERTY(EditAnywhere, Category = "Item")
		FShopData ItemShopData;
};
