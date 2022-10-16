// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeballFunctionLibrary.h"

bool UDodgeballFunctionLibrary::CanSeeActor(const UWorld* World, FVector Location, const AActor* TargetActor,
		TArray<const AActor*> IgnoreActors) 
{
	// No valid character
	if (TargetActor == nullptr)
	{
		return false;
	}

	// Storage for the line hit result
	FHitResult Hit;

	// get start and end of line trace
	FVector Start = Location;
	FVector End = TargetActor->GetActorLocation();

	// the trace channel we want to compare against -> dodgeball channel
	ECollisionChannel Channel = ECC_GameTraceChannel1;

	// Rotation of the shape used in sweep trace (quaternion)
	FQuat Rotation = FQuat::Identity;

	// shape to use for the sweep trace
	FCollisionShape Shape = FCollisionShape::MakeBox(FVector(20.f, 20.f, 20.f));
	
	// ignore the casting actor in case the line trace stops as soon as it is cast
	// happens when the line is cast inside the model -- it is already touching
	FCollisionQueryParams QueryParams;

	
	// ignore unwanted actors
	QueryParams.AddIgnoredActors(IgnoreActors);
	
	// feed query params to our line trace
	// set the trace
	World->LineTraceSingleByChannel(Hit, Start, End, Channel, QueryParams);
	
	// set sweep trace
	//World->SweepSingleByChannel(Hit, Start, End, Rotation, Channel, Shape);
	
	// drawing the debug line along with the actual trace
	DrawDebugLine(World, Start, Hit.bBlockingHit ? Hit.Location : End, FColor::Red);

	// return true if it was non blocking
	return !Hit.bBlockingHit;
}
