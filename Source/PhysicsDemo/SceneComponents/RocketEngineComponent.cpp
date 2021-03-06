// Fill out your copyright notice in the Description page of Project Settings.


#include "RocketEngineComponent.h"

// Called when the game starts
void URocketEngineComponent::BeginPlay()
{
	Super::BeginPlay();

	PrimitiveParent->SetMassOverrideInKg(NAME_None,BodyInst->GetBodyMass() + FuelMass);

	OnPhysicsTick.AddDynamic(this, &URocketEngineComponent::GenerateThrust);

	if (!ThrustUnitVector.IsNormalized())
		ThrustUnitVector.Normalize();
}

void URocketEngineComponent::GenerateThrust(float DeltaTime)
{
	float FuelRequired = FuelRequiredThisSubstep(DeltaTime);
	if (FuelMass > FuelRequired)
	{
		FuelMass -= FuelRequired;
		PrimitiveParent->SetMassOverrideInKg(NAME_None, BodyInst->GetBodyMass() - FuelRequired);

		float CurrentThrust = MaxThrust * ThrustPercent * 1000.f;		
		ApplyForce(ThrustUnitVector * CurrentThrust);
	}
}

float URocketEngineComponent::FuelRequiredThisSubstep(float DeltaTime)
{
	float CurrentThrustRequired = MaxThrust * ThrustPercent * 1000.f;
	return (CurrentThrustRequired / OXYGEN_HYDROGEN_EFFECTIVE_EXHAUST_VELOCITY) * DeltaTime;
}

