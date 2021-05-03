// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "PhysicsDemo/GameModes/PhysicsGameMode.h"
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

	PhysicsGameMode = Cast<APhysicsGameMode>(UGameplayStatics::GetGameMode(this));
	PhysicsGameMode->PhysicsBodies.Add(this);
	
	PrimitiveParent = Cast<UPrimitiveComponent>(GetAttachParent());
	if (PrimitiveParent) 
	{
		if (PrimitiveParent->IsSimulatingPhysics(GetAttachSocketName())) 
		{
			BodyInst = PrimitiveParent->GetBodyInstance(GetAttachSocketName());
		}
	}

	SetRelativeLocation(FVector::ZeroVector);
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

void USubstepPhysComponent::UpdateTranslationalMotion(float DeltaTime)
{
	ApplyGravity();
	ApplyLinearDrag(CurrentLinearVelocity);
	
	//Update Acceleration
	CurrentLinearAcceleration = CurrentResultantForce / BodyInst->GetBodyMass();

	//Update Velocity
	CurrentLinearVelocity += CurrentLinearAcceleration * DeltaTime;

	//Update Location
	FVector NewLocation = GetPhysicsLocation() + CurrentLinearVelocity * 100 * DeltaTime;
	SetPhysicsLocation(NewLocation);
	
	//Reset Forces
	CurrentResultantForce = FVector::ZeroVector;
}

void USubstepPhysComponent::PhysicsTick(float DeltaTime, FBodyInstance* BodyInstance)
{
	UpdateTranslationalMotion(DeltaTime);

	OnPhysicsTick.Broadcast(DeltaTime);
}

void USubstepPhysComponent::ApplyForce(FVector Force)
{
	CurrentResultantForce += Force;
}

float USubstepPhysComponent::GetPhysicsMass()
{
	return BodyInst->GetBodyMass();
}

void USubstepPhysComponent::SetPhysicsLocation(FVector NewLocation)
{
	FTransform NewTransform = GetPhysicsTransform();
	NewTransform.SetLocation(NewLocation);

	BodyInst->SetBodyTransform(NewTransform, ETeleportType::None);
}

void USubstepPhysComponent::SetPhysicsRotation(FRotator NewRotation)
{
	FTransform NewTransform = GetPhysicsTransform();
	NewTransform.SetRotation(NewRotation.Quaternion());

	BodyInst->SetBodyTransform(NewTransform, ETeleportType::None);
}

FTransform USubstepPhysComponent::GetPhysicsTransform()
{
	return PhysicsUtilities::GetSubstepComponentWorldTransform(this, BodyInst);
}

FVector USubstepPhysComponent::GetPhysicsLocation()
{
	return GetPhysicsTransform().GetLocation();
}

FRotator USubstepPhysComponent::GetPhysicsRotation()
{
	return GetPhysicsTransform().GetRotation().Rotator();
}

void USubstepPhysComponent::ApplyGravity()
{
	PhysicsGameMode->ApplyGravity();

	if (bSimulateEarthGravity)
	{
		float Distance = (EARTH_RADIUS + GetPhysicsLocation().Z) / 100.f;
		float DistanceSquared = FMath::Square(Distance);

		float ForceMagnitude = (UNIVERSAL_GRAVITATIONAL_CONSTANT * EARTH_MASS * GetPhysicsMass()) / DistanceSquared;
		ApplyForce(FVector::DownVector * ForceMagnitude);
	}
}

void USubstepPhysComponent::ApplyLinearDrag(FVector Velocity)
{
	if (bSimulateLinearDrag)
	{
		float SpeedTerm = FMath::Pow(Velocity.Size(), DragVelocityExponent);
		Velocity.Normalize();

		ApplyForce(-Velocity * SpeedTerm * DragConstant);
	}
}
