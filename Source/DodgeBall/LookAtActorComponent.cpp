// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtActorComponent.h"

#include "DodgeballFunctionLibrary.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
ULookAtActorComponent::ULookAtActorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULookAtActorComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

bool ULookAtActorComponent::LookAtActor() const
{
	if (TargetActor == nullptr) { return false; } // we can't process without a target
	
	// create list of actors to ignore, namely target and source actor
	TArray<const AActor*> IgnoreList;
	IgnoreList.Add(GetOwner());
	IgnoreList.Add(TargetActor);
	
	// check to see if we can see the actor
	if (UDodgeballFunctionLibrary::CanSeeActor(GetWorld(), GetComponentLocation(), TargetActor, IgnoreList))
	{
		// get rotation needed to target TargetActor
		const FVector Start = GetOwner()->GetActorLocation();
		const FVector End = TargetActor->GetActorLocation();

		// calc rotation transform
		const FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);

		// set new rotation
		GetOwner()->SetActorRotation(LookAtRotation);
		return true;
	}
	return false;
}


// Called every frame
void ULookAtActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bCanSeeTarget = LookAtActor();
}

