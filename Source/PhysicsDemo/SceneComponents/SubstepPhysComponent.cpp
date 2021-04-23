// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "PhysicsDemo/GameInstances/PhysicsGameInstance.h"
#include "PhysicsDemo/Utility/PhysicsUtilities.h"
#include "SubstepPhysComponent.h"

//TArray<USubstepPhysComponent*> USubstepPhysComponent::PhysicsBodies;
//int32 USubstepPhysComponent::GravityCounter = 0;

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

	PhysicsGameInstance = Cast<UPhysicsGameInstance>(UGameplayStatics::GetGameInstance(this));
	PhysicsGameInstance->PhysicsBodies.Add(this);
	
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
	ApplyGravity();
	
	//Update Acceleration
	CurrentLinearAcceleration = CurrentResultantForce / BodyInstance->GetBodyMass();

	//Update Velocity
	CurrentLinearVelocity += CurrentLinearAcceleration * DeltaTime;

	//Update Location
	FVector NewLocation = GetPhysicsLocation() + CurrentLinearVelocity * 100 * DeltaTime;
	SetPhysicsLocation(NewLocation);

	//Reset Forces
	CurrentResultantForce = FVector::ZeroVector;

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
	if (PhysicsGameInstance->PhysicsBodies.Num() > 1)
	{
		if (PhysicsGameInstance->GravityCounter == 0)
		{
			for (int32 i = 0, n = PhysicsGameInstance->PhysicsBodies.Num(); i < n - 1; i++)
			{
				USubstepPhysComponent* BodyA = PhysicsGameInstance->PhysicsBodies[i];
				
				for (int32 j = i + 1; j < n; j++)
				{
					USubstepPhysComponent* BodyB = PhysicsGameInstance->PhysicsBodies[j];

					FVector DirectionAToB = BodyB->GetPhysicsLocation() - BodyA->GetPhysicsLocation();
					float Distance = DirectionAToB.Size() / 100.f;
					
					DirectionAToB.Normalize();
					float DistanceSquared = FMath::Square(Distance);
					
					float ForceMagnitude = (UNIVERSAL_GRAVITATIONAL_CONSTANT * BodyA->GetPhysicsMass() * BodyB->GetPhysicsMass()) / DistanceSquared;

					BodyA->ApplyForce(DirectionAToB * ForceMagnitude);
					BodyB->ApplyForce(-DirectionAToB * ForceMagnitude);
				}
			}
		}

		PhysicsGameInstance->GravityCounter++;

		if (PhysicsGameInstance->GravityCounter == PhysicsGameInstance->PhysicsBodies.Num())
		{
			PhysicsGameInstance->GravityCounter = 0;
		}
	}
}

