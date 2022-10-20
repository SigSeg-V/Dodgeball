// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "DodgeballProjectile.generated.h"

UCLASS()
class ADodgeballProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADodgeballProjectile();


	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovementComponent() const { return ProjectileMovement; }
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dodgeball, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* SphereComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Dodgeball, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const
			   FHitResult& Hit);

	UPROPERTY(EditAnywhere, Category = Dodgeball)
	float Damage = 34.f;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* BounceSound;

	UPROPERTY(EditAnywhere, Category = Sound)
	class USoundBase* HitSound;
	
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	class USoundAttenuation* BounceSoundAttenuation;

	// The particles that spawn when the player gets hit
	UPROPERTY(EditAnywhere, Category = Particles)
	class UParticleSystem* HitParticles;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
