// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

#include "HealthInterface.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UHealthComponent::LoseHealth(int Damage)
{
	Health -= Damage;

	// cap to +ve nums to make sure no strange bugs can occur from underflow
	Health = (Health > 0) * Health;

	if (!Health && GetOwner()->Implements<UHealthInterface>())
	{
		IHealthInterface::Execute_OnDeath(GetOwner());
	}
}

// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

