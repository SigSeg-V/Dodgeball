// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "TimerManager.h"
#include "DodgeballProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SightSource = CreateDefaultSubobject<USceneComponent>(TEXT("SightSource"));
	SightSource->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

bool AEnemyCharacter::LookAtActor(const AActor* TargetActor)
{
	if (TargetActor == nullptr) {return false;} // we can't process without a target

	// check to see if we can see the actor
	if (CanSeeActor(TargetActor))
	{
		// get rotation needed to target TargetActor
		const FVector Start = GetActorLocation();
		const FVector End = TargetActor->GetActorLocation();

		// calc rotation transform
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

		// set new rotation
		SetActorRotation(LookAtRotation);
		return true;
	}
	return false;
}


bool AEnemyCharacter::CanSeeActor(const AActor* TargetActor) const
{
	// No valid character
	if (TargetActor == nullptr)
	{
		return false;
	}

	// Storage for the line hit result
	FHitResult Hit;

	// get start and end of line trace
	FVector Start = SightSource->GetComponentLocation();
	FVector End = TargetActor->GetActorLocation();

	// the trace channel we want to compare against
	ECollisionChannel Channel = ECC_GameTraceChannel1;

	// Rotation of the shape used in sweep trace (quaternion)
	FQuat Rotation = FQuat::Identity;

	// shape to use for the sweep trace
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(20.f, 20.f, 20.f));
	
	// ignore the casting actor in case the line trace stops as soon as it is cast
	// happens when the line is cast inside the model -- it is already touching
	FCollisionQueryParams QueryParams;

	// ignore casting actor
	QueryParams.AddIgnoredActor(this);

	// ignore the target actor -- model can block visibility calcs
	QueryParams.AddIgnoredActor(TargetActor);

	// feed query params to our line trace
	// set the trace
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
	
	// set sweep trace
	// GetWorld()->SweepSingleByChannel(Hit, Start, End, Rotation, Channel, Shape);
	
	// drawing the debug line along with the actual trace
	DrawDebugLine(GetWorld(), Start, Hit.bBlockingHit ? Hit.Location : End, FColor::Red);

	// return true if it was non blocking
	return !Hit.bBlockingHit;
}

void AEnemyCharacter::ThrowDodgeball() const
{
	if (DodgeballClass == nullptr) {return;}
	
	const FVector FVec = GetActorForwardVector();
	constexpr float SpawnDistance = 40.f;

	const FVector SpawnLocation = GetActorLocation() + (FVec * SpawnDistance);

	GetWorld()->SpawnActor<ADodgeballProjectile>(DodgeballClass, SpawnLocation, GetActorRotation());
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Get character currently controlled by the player
	const ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(this, 0); 

	// Look at player object
	bCanSeePlayer = LookAtActor(PlayerCharacter);

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

