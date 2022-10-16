// Copyright Epic Games, Inc. All Rights Reserved.

#include "DodgeBallCharacter.h"

#include "DodgeBallGameMode.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ADodgeBallCharacter
//////////////////////////////////////////////////////////////////////////

ADodgeBallCharacter::ADodgeBallCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 900.0f; // The camera follows at this distance behind the character	
	

	// Rotate the camera to look down on the player MGS-style
	CameraBoom->SetRelativeRotation(FRotator(-70.f, 0.f ,0.f));

	// Ignore the rotation inherited from the pawn
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bInheritRoll = false;
	
	// Don't rotate the camera from the PawnController rotation
	CameraBoom->bUsePawnControlRotation = false;
	
	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ADodgeBallCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Get bindings, and add Mapping context to them

	if (UEnhancedInputComponent* EInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
		EInputComponent != nullptr)
	{
		if (const APlayerController* PlayerController = Cast<APlayerController>(GetController());
			PlayerController != nullptr)
		{
			UEnhancedInputLocalPlayerSubsystem* ESubsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
			if (ESubsystem != nullptr)
			{
				ESubsystem->AddMappingContext(IC_Character, 1);
			}	
		}
		
		// Add the input actions
		EInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ADodgeBallCharacter::Move);
	}
	
	// PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ADodgeBallCharacter::MoveForward);
	// PlayerInputComponent->BindAxis("Move Right / Left", this, &ADodgeBallCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turn rate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	

	
	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ADodgeBallCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ADodgeBallCharacter::TouchStopped);
}

void ADodgeBallCharacter::Move(const FInputActionValue& Value)
{
	if (const FVector2D InputValue = Value.Get<FVector2D>(); Controller != nullptr && (InputValue.X || InputValue.Y))
	{
		// Get rotation of the camera to decide which direction the character should move
		// (movement is not based on the character's direction but the camera's)

		const FRotator YawRotation(0, Controller->GetControlRotation().Yaw, 0);

		// Got movement horizontally
		if (InputValue.X)
		{
			// Get right perpendicular vec to what camera is pointing at
			// eg.
			//          cam dir.  r. vec. 
			//            ||       ||
			//            V        V 
			//            O       -->
			const FVector RDir = UKismetMathLibrary::GetRightVector(YawRotation);

			AddMovementInput(RDir, InputValue.X);
		}
		// Got movement vertically
		if(InputValue.Y)
		{
			AddMovementInput(YawRotation.Vector(), InputValue.Y);
		}
	}
}

void ADodgeBallCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ADodgeBallCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ADodgeBallCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ADodgeBallCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}


void ADodgeBallCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADodgeBallCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
