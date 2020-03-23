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
#include "SI_JustSurvive/SI_JustSurviveProjectile.h"
#include <TimerManager.h>

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
	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemyBase::OnHit);

	GetMesh()->SetCollisionProfileName("NoCollision"); 
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	//TODO(Anyone): Networking - Wait it is a Character, are they networked?
	//SetReplicates(true)
	//SetReplicatesMovement(true);

	Tags.Add("Enemy"); 

	//TODO: Have enemies take damage
	SetCanBeDamaged(true);


	//TODO: @Vanessa Add a noise emitter to enemy and make it so the tower is instigated by by the enemy's noise emitter. Sense the player and generator with Pawn Sensing.  
	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("Noise Emitter"));

	//SetMakeNoiseDelegate();
	//TODO: @Anthony Make the enemy spawn a proectile based on forward vector.  

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>("Pawn Sensor");
	OnTakeAnyDamage.AddDynamic(this, &AEnemyBase::TakeAnyDamage);

	PawnSensing->OnSeePawn.AddDynamic(this, &AEnemyBase::OnPawnSeen);

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AEnemyBase::SetWaypointManager(AWaypointManager* wayMan)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		check(wayMan && "Waypoint Manager was null");

		if (wayMan)
		{
			m_WaypointManager = wayMan;
		}
	}
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();
	//Make noise test
	//MakeNoise(1.0f, this, GetActorLocation());
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 32.0f, 12, FColor::Green, false, 10.0f);

	if (GetLocalRole() == ROLE_Authority)
	{
		//Couldn't add delegates here
	}

	if (GetLocalRole() == ROLE_Authority)
	{

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
}

void AEnemyBase::FindWaypointManager()
{
	//We only want to do this if the waypoint manager is null
	if (GetLocalRole() != ROLE_Authority) return;

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
	if (GetLocalRole() == ROLE_Authority)
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
}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (GetLocalRole() != ROLE_Authority) return;

	//test
	MakeNoise(6.0f, this, GetActorLocation());
	//DrawDebugSphere(GetWorld(), GetActorLocation(), 32.0f, 12, FColor::Green, false, 10.0f);

	if (GetLocalRole() == ROLE_Authority)
	{
		if (m_WaypointManager == nullptr)
		{
			FindWaypointManager();
		}
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

    if (bCanShoot)
    {
        m_CanShootTimer += DeltaTime; 

        if (m_CanShootTimer > 3.0f)
        {
            bCanShoot = false; 
        }
    }

}

void AEnemyBase::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	//if (Cast<ASI_>)
}

void AEnemyBase::OnPawnSeen(APawn* pawn)
{
	if (GetLocalRole() != ROLE_Authority) return;
	//Do we want the enemy to make sure we get the player over generator or vice versa
	if (Cast<ASI_JustSurviveCharacter>(pawn) || Cast<AGeneratorBase>(pawn))
	{
        TargetPawn = pawn;
		//TODO: Later
        //m_TargetWaypoint->SetIsWaypointTaken(false); 
        //m_TargetWaypoint = nullptr; 

        bCanShoot = true; 
        FVector Direction = pawn->GetActorLocation() - GetActorLocation();
        Direction.Normalize();

        FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
        NewLookAt.Pitch = 0.0f;
        NewLookAt.Roll = 0.0f;
        SetActorRotation(NewLookAt);

        if (bCanShoot)
        {
            Shoot(); 
            bCanShoot = false; 
      
        }
	}
	
    if (pawn == nullptr)
    {
        ClearShootTimer(); 
    }
}

void AEnemyBase::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (GetLocalRole() != ROLE_Authority) return;

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
	if (GetLocalRole() != ROLE_Authority) return;

	if (m_TargetWaypoint != nullptr)
	{
		//When the enemy dies we want him to give up his waypoint
		m_TargetWaypoint->SetIsWaypointTaken(false);
	}

	m_RoundManager->RemoveEnemy();

	this->Destroy(); 
}

void AEnemyBase::Shoot()
{
	if (GetLocalRole() != ROLE_Authority) return;

    FTimerManager& Timer = GetWorldTimerManager(); 
    Timer.SetTimer(m_EnemyFireTimer, this, &AEnemyBase::SpawnProjectile, m_EnemyFireRate, false, 0.2);
}

void AEnemyBase::SpawnProjectile()
{
	if (GetLocalRole() != ROLE_Authority) return;

    if (ProjectileClass != nullptr)
    {
        UWorld* const World = GetWorld(); 
        if (World != nullptr)
        {
            const FRotator SpawnRotation = GetControlRotation(); 

            //TODO: @Anthony Add a socket to the enemy meshes called "Muzzle"
            const FVector SpawnLocation = GetMesh()->GetSocketLocation("Muzzle"); 

            FActorSpawnParameters ActorSpawnParams; 
            ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn; 

            ASI_JustSurviveProjectile* projectile = World->SpawnActor<ASI_JustSurviveProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams); 
            projectile->SetOwner(this); 

            bCanShoot = false; 

        }
    }
}

void AEnemyBase::ClearShootTimer()
{
    GetWorld()->GetTimerManager().ClearTimer(m_EnemyFireTimer); 
}

void AEnemyBase::IncrementCurrentWaypointGroup()
{
	if (GetLocalRole() != ROLE_Authority) return;

	m_CurrentWaypointGroup++;

	if (m_CurrentWaypointGroup >= m_WaypointManager->GetWaypointGroupSize())
	{
		//Reset the group int to 0
		m_CurrentWaypointGroup = 0;
	}
}

void AEnemyBase::DecrementCurrentWaypointGroup()
{
	if (GetLocalRole() != ROLE_Authority) return;

	m_CurrentWaypointGroup--;

	if (m_CurrentWaypointGroup < 0)
	{
		//Reset the group int to Max-1
		m_CurrentWaypointGroup = m_WaypointManager->GetWaypointGroupSize() - 1;
	}
}