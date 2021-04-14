// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysMeshComponent.h"
#include "PhysicsInterfaceDeclaresCore.h"
#include <PxRigidActor.h>

UPhysMeshComponent::UPhysMeshComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	OnCalculateCustomPhysics.BindUObject(this, &UPhysMeshComponent::PhysicsTick);
}

void UPhysMeshComponent::BeginPlay()
{
	Super::BeginPlay();

	//Get the BodyInstance PxRigidbody
	PhysicsActorHandle = &GetBodyInstance()->ActorHandle;
}

void UPhysMeshComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsSimulatingPhysics())
	{
		GetBodyInstance()->AddCustomPhysics(OnCalculateCustomPhysics);
	}
}

void UPhysMeshComponent::PhysicsTick(float DeltaTime, FBodyInstance* BodyInst)
{
	FVector CurrentLocation = GetLocation();
	FVector UpdatedLocation = CurrentLocation + CurrentLinearVelocity * DeltaTime;
	SetLocation(UpdatedLocation);
}

void UPhysMeshComponent::ApplyLinearVelocity(FVector NewLinearVelocity)
{
	CurrentLinearVelocity = NewLinearVelocity;
}

void UPhysMeshComponent::SetLocation(FVector NewLocation)
{
	PxTransform PhysicsTransform = PhysicsActorHandle->SyncActor->getGlobalPose();
	PxVec3 NewPhysicsLocation(NewLocation.X, NewLocation.Y, NewLocation.Z);

	PhysicsTransform.p = NewPhysicsLocation;
	PhysicsActorHandle->SyncActor->setGlobalPose(PhysicsTransform);
}

void UPhysMeshComponent::SetRotation(FRotator NewRotation)
{
	PxTransform PhysicsTransform = PhysicsActorHandle->SyncActor->getGlobalPose();
	FQuat NewQuaternion = NewRotation.Quaternion();
	PxQuat NewPhysicsQuaternion(NewQuaternion.X, NewQuaternion.Y, NewQuaternion.Z, NewQuaternion.W);

	PhysicsTransform.q = NewPhysicsQuaternion;
	PhysicsActorHandle->SyncActor->setGlobalPose(PhysicsTransform);
}

FVector UPhysMeshComponent::GetLocation()
{
	PxTransform PhysicsTransform = PhysicsActorHandle->SyncActor->getGlobalPose();
	return FVector(PhysicsTransform.p.x, PhysicsTransform.p.y, PhysicsTransform.p.z);
}

FRotator UPhysMeshComponent::GetRotation()
{
	PxTransform PhysicsTransform = PhysicsActorHandle->SyncActor->getGlobalPose();
	return FRotator(FQuat(PhysicsTransform.q.x, PhysicsTransform.q.y, PhysicsTransform.q.z, PhysicsTransform.q.w));
}

FVector UPhysMeshComponent::GetLinearVelocity()
{
	return FPhysicsInterface::GetLinearVelocity_AssumesLocked(*PhysicsActorHandle);
}

FVector UPhysMeshComponent::GetAngularVelocity()
{
	FVector PhysicsAngularVelocity = FPhysicsInterface::GetAngularVelocity_AssumesLocked(*PhysicsActorHandle);
	return FMath::RadiansToDegrees(PhysicsAngularVelocity);
}
