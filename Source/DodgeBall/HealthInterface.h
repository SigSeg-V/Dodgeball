// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "HealthInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UHealthInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DODGEBALL_API IHealthInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	// blueprint interface of the death commands
	UFUNCTION(BlueprintNativeEvent, Category = Health)
	void OnDeath();

	// C++ impl of blueprint interface
	virtual void OnDeath_Implementation() = 0;

	// blueprint interface of the dmg commands
	UFUNCTION(BlueprintNativeEvent, Category = Health)
	void OnTakeDamage();

	// C++ impl of blueprint interface
	virtual void OnTakeDamage_Implementation() = 0;
	
};
