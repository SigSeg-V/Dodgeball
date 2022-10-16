// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/World.h"

#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class DODGEBALL_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LookAt, meta = (AllowPrivateAccess = "true"))
	USceneComponent* SightSource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Dodgeball)
	TSubclassOf<class ADodgeballProjectile> DodgeballClass;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Rotate enemy player to always be facing the player
	bool LookAtActor(const AActor* TargetActor);

	// enemy can see player on current frame
	bool bCanSeePlayer = false;
	
	// enemy can see player on previous frame
	bool bPreviousCanSeePlayer = false;

	// Identifies timer to start
	FTimerHandle ThrowTimerHandle;

	// amount of time to wait between throws
	float ThrowingInterval = 2.f;

	// Initial delay before the loop begins
	float ThrowingDelay = 0.5f;

	// Throw dodgeball when the timer ends
	void ThrowDodgeball() const;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
