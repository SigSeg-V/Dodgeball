// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryBox.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DodgeBallCharacter.h"
#include "Components/BoxComponent.h"

// Sets default values
AVictoryBox::AVictoryBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("Victory Box"));
	RootComponent = RootScene;

	// create the collision box and attach to the scene
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Box"));
	CollisionBox->SetupAttachment(RootComponent);

	// Setup box size
	CollisionBox->SetBoxExtent(FVector(60.f, 60.f, 60.f));

	// Offset height off the ground to guarantee correct collision
	CollisionBox->SetRelativeLocation(FVector(0,0,120.f));

	// Listen to overlap events so we can end the game when the player touches box
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AVictoryBox::OnBeginOverlap);
}

// Called when the game starts or when spawned
void AVictoryBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVictoryBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVictoryBox::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& Hit)
{
	if(Cast<ADodgeBallCharacter>(OtherActor))
	{
		UKismetSystemLibrary::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, true);
	}
}

