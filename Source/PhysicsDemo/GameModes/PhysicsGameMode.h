// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PhysicsGameMode.generated.h"

#define UNIVERSAL_GRAVITATIONAL_CONSTANT 6.67408e-11f

class USubstepPhysComponent;

/**
 * 
 */
UCLASS()
class PHYSICSDEMO_API APhysicsGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	void ApplyGravity();
	
	TArray<USubstepPhysComponent*> PhysicsBodies;
	int32 GravityCounter = 0;

	friend USubstepPhysComponent;
};
