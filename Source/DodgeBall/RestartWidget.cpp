// Fill out your copyright notice in the Description page of Project Settings.


#include "RestartWidget.h"

#include "DodgeballPlayerController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void URestartWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// can't proceed without a button to receive events from
	if (RestartButton != nullptr)
	{
		RestartButton->OnClicked.AddDynamic(this, &URestartWidget::OnRestartClicked);
	}
}


// reload the level
// ReSharper disable once CppMemberFunctionMayBeConst
void URestartWidget::OnRestartClicked()
{
	ADodgeballPlayerController* PlayerController = Cast<ADodgeballPlayerController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->HideRestartWidget();
	}

	UGameplayStatics::OpenLevel(this, FName(*UGameplayStatics::GetCurrentLevelName(this)));
}