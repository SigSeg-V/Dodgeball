// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "DodgeballFunctionLibrary.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "DodgeballProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// look at character component init
	LookAtActorComponent = CreateDefaultSubobject<ULookAtActorComponent>(TEXT("Look At Actor"));
	LookAtActorComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Get character currently controlled by the player
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);
	
	// Set target to the player
	LookAtActorComponent->SetTarget(PlayerCharacter);
}

void AEnemyCharacter::ThrowDodgeball() const
{
	if (DodgeballClass == nullptr) {return;}
	
	const FVector FVec = GetActorForwardVector();
	constexpr float SpawnDistance = 40.f;
	
	
	const FVector SpawnLocation = GetActorLocation() + (FVec * SpawnDistance);
	const FTransform SpawnTransform (GetActorRotation(), SpawnLocation);
	
	ADodgeballProjectile* Projectile = GetWorld()->SpawnActorDeferred<ADodgeballProjectile>(DodgeballClass, SpawnTransform);
	Projectile->GetProjectileMovementComponent()->InitialSpeed = 2200.f;
	Projectile->FinishSpawning(SpawnTransform);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Look at player object
	bCanSeePlayer = LookAtActorComponent->CanSeeTarget();

	if (bCanSeePlayer != bPreviousCanSeePlayer)
	{
		if (bCanSeePlayer)
		{
			// Start throwing dodgeballs
			GetWorldTimerManager().SetTimer(ThrowTimerHandle, this, &AEnemyCharacter::ThrowDodgeball,
				ThrowingInterval, true, ThrowingDelay);
		}
		else
		{
			//Stop throwing dodgeballs
			GetWorldTimerManager().ClearTimer(ThrowTimerHandle);
		}
	}
	
	bPreviousCanSeePlayer = bCanSeePlayer;
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

