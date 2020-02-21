// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "../Waypoints/Waypoint.h"
#include "../Waypoints/WaypointGroup.h"
#include "../Waypoints/WaypointManager.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "../RoundManager/RoundManager.h"
#include "Components/PawnNoiseEmitterComponent.h"
#include "SI_JustSurvive/Items/TowerBase.h"
#include "Perception/PawnSensingComponent.h"
#include "SI_JustSurvive/SI_JustSurviveCharacter.h"
#include "SI_JustSurvive/Items/GeneratorBase.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionProfileName("BlockAllDynamic"); 
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true); 
	//GetCapsuleComponent()->SetSimulatePhysics(true); 
	//GetCapsuleComponent()->SetEnableGravity(false); 

	GetMesh()->SetCollisionProfileName("NoCollision"); 
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyBase::OnHit); 
	//TODO(Anyone): Networking - Wait it is a Character, are they networked?
	//SetReplicates(true)
	//SetReplicatesMovement(true);

	Tags.Add("Enemy"); 

	//TODO: Have enemies take damage
	SetCanBeDamaged(true);
	OnTakeAnyDamage.AddDynamic(this, &AEnemyBase::TakeAnyDamage);

	//TODO: @Vanessa Add a noise emitter to enemy and make it so the tower is instigated by by the enemy's noise emitter. Sense the player and generator with Pawn Sensing.  
	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));
	//SetMakeNoiseDelegate();
	//TODO: @Anthony Make the enemy spawn a proectile based on forward vector.  

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensor");
	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyBase::OnPawnSeen);
}

void AEnemyBase::SetWaypointManager(AWaypointManager* wayMan)
{
	check(wayMan && "Waypoint Manager was null");

	if (wayMan)
	{
		m_WaypointManager = wayMan;
	}
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	//Make noise test
	//MakeNoise(1.0f, this, GetActorLocation());
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 32.0f, 12, FColor::Green, false, 10.0f);

	TArray<AActor*> roundManagers; 
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoundManager::StaticClass(), roundManagers);

	m_RoundManager = Cast<ARoundManager>(roundManagers[0]); 

	//TODO: check that there is no more than one Round Manager. 

	if (m_WaypointManager == nullptr)
	{
		FindWaypointManager();
	}

	if (EnemyData.m_EnemyMesh != nullptr)
	{
		GetMesh()->SetSkeletalMesh(EnemyData.m_EnemyMesh);
	}
}

void AEnemyBase::FindWaypointManager()
{
	//We only want to do this if the waypoint manager is null
	if (m_WaypointManager == nullptr)
	{
		TArray<AActor*> singleWaypointManager;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypointManager::StaticClass(), singleWaypointManager);

		check(singleWaypointManager.Num() == 1 && "There was either 0 or 2+ Waypoint managers in this scene");

		AWaypointManager* newManager = Cast<AWaypointManager>(singleWaypointManager[0]);

		check(newManager && "New manager ended up being null, this probably shouldn't happen");

		if (newManager)
		{
			SetWaypointManager(newManager); 
		}
	}
}

void AEnemyBase::GetNewWaypoint()
{

	//TODO: Networking - GetLocalRole() == ROLE_Authority ??
	check(m_WaypointManager && "Waypoint manager was null");

	//check(FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()));

	if (m_TargetWaypoint != nullptr)
	{
		//Set the old waypoint to Not Taken
		m_TargetWaypoint->SetIsWaypointTaken(false);
	}

	//TODO(Anyone): This will work for looping through.  But when we get to the generator it will need to be refactored
	if (m_WaypointManager)
	{
		check(m_WaypointManager->GetWaypointGroupSize() != 0);

		if (m_TargetWaypoint == nullptr || m_CurrentWaypointGroup >= m_WaypointManager->GetWaypointGroupSize())
		{
			m_CurrentWaypointGroup = 0;
			m_TargetWaypoint = m_WaypointManager->GetRandomWaypoint(m_CurrentWaypointGroup);
		}
		else
		{
			m_TargetWaypoint = m_WaypointManager->GetRandomWaypoint(m_CurrentWaypointGroup);
		}

		if (m_TargetWaypoint != nullptr)
		{
			m_TargetWaypoint->SetIsWaypointTaken(true);
			IncrementCurrentWaypointGroup();
			UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_TargetWaypoint);
		}
	}

	if (m_TargetWaypoint == nullptr)
	{
		//If we didn't find a point in the next group we fall back
		DecrementCurrentWaypointGroup();
	}

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//test
	MakeNoise(6.0f, this, GetActorLocation());
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 32.0f, 12, FColor::Green, false, 10.0f);


	if (m_WaypointManager == nullptr)
	{
		FindWaypointManager();
	}

	if (m_TargetWaypoint != nullptr)
	{

		FVector DistanceVector = GetActorLocation() - m_TargetWaypoint->GetActorLocation();

		float DistanceSize = DistanceVector.Size();

		if (DistanceSize < EnemyData.m_DistanceToPoint)
		{
			//TODO: Make this change states instead of getting a new Waypoint
			GetNewWaypoint();
		}
	}
	else
	{
		GetNewWaypoint(); //If we don't have a waypoint we want one.  There is a check within GetNewWaypoint
	}

}

void AEnemyBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (Cast<ASI_>)
}

void AEnemyBase::OnPawnSeen(APawn* pawn)
{
	//Do we want the enemy to make sure we get the player over generator or vice versa
	if (Cast<ASI_JustSurviveCharacter>(pawn) || Cast<AGeneratorBase>(pawn))
	{
		TargetPawn = pawn;
		//TODO: Rotate towards pawn
	}
	else
		TargetPawn = nullptr;
}

void AEnemyBase::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	float m_DamageAmount = 0;
	m_DamageAmount = Damage;

	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Damage Received - " + FString::FromInt(Damage));
	m_EnemyHP -= m_DamageAmount;

	if (m_EnemyHP <= 0)
	{
		KillEnemy();
	}
}

void AEnemyBase::KillEnemy()
{
	if (m_TargetWaypoint != nullptr)
	{
		//When the enemy dies we want him to give up his waypoint
		m_TargetWaypoint->SetIsWaypointTaken(false);
	}

	m_RoundManager->RemoveEnemy();

	this->Destroy(); 
}

void AEnemyBase::IncrementCurrentWaypointGroup()
{
	m_CurrentWaypointGroup++;

	if (m_CurrentWaypointGroup >= m_WaypointManager->GetWaypointGroupSize())
	{
		//Reset the group int to 0
		m_CurrentWaypointGroup = 0;
	}
}

void AEnemyBase::DecrementCurrentWaypointGroup()
{
	m_CurrentWaypointGroup--;

	if (m_CurrentWaypointGroup < 0)
	{
		//Reset the group int to Max-1
		m_CurrentWaypointGroup = m_WaypointManager->GetWaypointGroupSize() - 1;
	}
}