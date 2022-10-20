// Fill out your copyright notice in the Description page of Project Settings.


#include "DodgeballPlayerController.h"

#include "HUDWidget.h"
#include "RestartWidget.h"

void ADodgeballPlayerController::ShowRestartWidget()
{
	// can't proceed without a widget to get events from
	if (BP_RestartWidget == nullptr) {return;}

	// freeze current game
	SetPause(true);

	// change input mode to stop player character receiving input
	SetInputMode(FInputModeUIOnly());

	// get mouse back
	bShowMouseCursor = true;

	// instantiate our widget
	RestartWidget = CreateWidget<URestartWidget>(this, BP_RestartWidget);
	RestartWidget->AddToViewport();
}

void ADodgeballPlayerController::HideRestartWidget()
{
	// destroy our UI widget
	RestartWidget->RemoveFromParent();
	RestartWidget->Destruct();

	// unpause the game
	SetPause(false);

	// give control back to the game
	SetInputMode(FInputModeGameOnly());
	
	// give mouse control back to the game
	bShowMouseCursor = false;
}

void ADodgeballPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// create the hud elements
	if (BP_HUDWidget == nullptr) {return;}

	HUDWidget = CreateWidget<UHUDWidget>(this, BP_HUDWidget);
	HUDWidget->AddToViewport();
}

void ADodgeballPlayerController::UpdateHealthPercent(const float HealthPercent) const
{
	if (HUDWidget == nullptr) {return;}

	HUDWidget->UpdateHealthBar(HealthPercent);
}
