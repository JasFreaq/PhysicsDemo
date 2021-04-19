// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysicsDemo/Utility/PhysicsUtilities.h"
#include "SubstepPhysComponent.h"

USubstepPhysComponent::USubstepPhysComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	OnCalculateCustomPhysics.BindUObject(this, &USubstepPhysComponent::PhysicsTick);
}


// Called when the game starts
void USubstepPhysComponent::BeginPlay()
{
	Super::BeginPlay();

	PrimitiveParent = Cast<UPrimitiveComponent>(GetAttachParent());
	if (PrimitiveParent) 
	{
		if (PrimitiveParent->IsSimulatingPhysics(GetAttachSocketName())) 
		{
			BodyInst = PrimitiveParent->GetBodyInstance(GetAttachSocketName());
		}
	}
}


// Called every frame
void USubstepPhysComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PrimitiveParent->IsSimulatingPhysics(GetAttachSocketName()) && BodyInst)
	{
		BodyInst->AddCustomPhysics(OnCalculateCustomPhysics);
	}
}

void USubstepPhysComponent::PhysicsTick(float DeltaTime, FBodyInstance* BodyInstance)
{
	//Update Acceleration
	CurrentLinearAcceleration = CurrentResultantForce / BodyInstance->GetBodyMass();

	//Update Velocity
	CurrentLinearVelocity += CurrentLinearAcceleration * DeltaTime;

	//Update Location
	FVector NewLocation = GetLocation() + CurrentLinearVelocity * 100 * DeltaTime;
	SetLocation(NewLocation);
}

void USubstepPhysComponent::ApplyForce(FVector Force)
{
	CurrentResultantForce += Force;
}

void USubstepPhysComponent::SetLocation(FVector NewLocation)
{
	FTransform NewTransform = GetTransform();
	NewTransform.SetLocation(NewLocation);

	BodyInst->SetBodyTransform(NewTransform, ETeleportType::None);
}

void USubstepPhysComponent::SetRotation(FRotator NewRotation)
{
	FTransform NewTransform = GetTransform();
	NewTransform.SetRotation(NewRotation.Quaternion());

	BodyInst->SetBodyTransform(NewTransform, ETeleportType::None);
}

FTransform USubstepPhysComponent::GetTransform()
{
	return PhysicsUtilities::GetSubstepComponentWorldTransform(this, BodyInst);
}

FVector USubstepPhysComponent::GetLocation()
{
	return GetTransform().GetLocation();
}

FRotator USubstepPhysComponent::GetRotation()
{
	return GetTransform().GetRotation().Rotator();
}

