// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeballProjectile.h"
#include "DodgeBallCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "HealthComponent.h"

// Sets default values
ADodgeballProjectile::ADodgeballProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	SphereComponent->SetSphereRadius(35.f);
	SphereComponent->SetCollisionProfileName(FName("Dodgeball"));
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetNotifyRigidBodyCollision(true);

	// listen to OnComponentHit by binding it to OnHit
	SphereComponent->OnComponentHit.AddDynamic(this, &ADodgeballProjectile::OnHit);
	// set sphere as the root component so collision plays nicely
	RootComponent = SphereComponent;

	// set up projectile movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Movement"));
	ProjectileMovement->InitialSpeed = 1500.f;
	
}

void ADodgeballProjectile::OnHit(UPrimitiveComponent *HitComp, AActor *OtherActor, UPrimitiveComponent *OtherComp,
	FVector NormalImpulse, const FHitResult &Hit)
{
	// only want to deal with when we hit the player
	if (const ADodgeBallCharacter* Player = Cast<ADodgeBallCharacter>(OtherActor); Player == nullptr)
	{
		return;
	}
	else
	{
		if (UHealthComponent* HealthComponent = Player->FindComponentByClass<UHealthComponent>();
			HealthComponent != nullptr)
		{
			HealthComponent->LoseHealth(Damage);
		}
	}
	Destroy();

	// delete when hit player
	Destroy();
}

// Called when the game starts or when spawned
void ADodgeballProjectile::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(5.f);
	
}

// Called every frame
void ADodgeballProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

