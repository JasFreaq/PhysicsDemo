// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsDemo/SceneComponents/SubstepPhysComponent.h"
#include "RocketEngineComponent.generated.h"

#define OXYGEN_HYDROGEN_EFFECTIVE_EXHAUST_VELOCITY 4400.f

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PHYSICSDEMO_API URocketEngineComponent : public USubstepPhysComponent
{
	GENERATED_BODY()

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Physics", meta = (AllowPrivateAccess = true, Tooltip = "in kg."))
		float FuelMass = 1000.f; //in kg

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Physics", meta = (AllowPrivateAccess = true, Tooltip = "in kN."))
		float MaxThrust = 2278.f; //in kN

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Physics", meta = (AllowPrivateAccess = true, ClampMin = 0.f, ClampMax = 1.f))
		float ThrustPercent = 0.5f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Custom Physics", meta = (AllowPrivateAccess = true, ClampMin = 0.f, ClampMax = 1.f))
		FVector ThrustUnitVector = FVector::UpVector;
		
	UFUNCTION()
		void GenerateThrust(float DeltaTime);
	
	float FuelRequiredThisSubstep(float DeltaTime);
};
