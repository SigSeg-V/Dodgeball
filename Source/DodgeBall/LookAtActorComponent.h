// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "LookAtActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DODGEBALL_API ULookAtActorComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULookAtActorComponent();

	FORCEINLINE void SetTarget(AActor* NewActor) { TargetActor = NewActor; }

	FORCEINLINE bool CanSeeTarget() const { return bCanSeeTarget; }
	
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Rotate enemy player to always be facing the player
	bool LookAtActor() const;

	UPROPERTY(EditDefaultsOnly, Category = "Look At Actor")
	AActor* TargetActor;

	bool bCanSeeTarget = false;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
