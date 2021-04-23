// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PhysicsGameInstance.generated.h"

class USubstepPhysComponent;

/**
 * 
 */
UCLASS()
class PHYSICSDEMO_API UPhysicsGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	TArray<USubstepPhysComponent*> PhysicsBodies;
	int32 GravityCounter = 0;

	friend USubstepPhysComponent;
};
