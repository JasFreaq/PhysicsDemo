// Fill out your copyright notice in the Description page of Project Settings.

#include "PhysActor.h"
#include "PhysicsInterfaceDeclaresCore.h"
#include <PxRigidActor.h>

// Sets default values
APhysActor::APhysActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	OnCalculateCustomPhysics.BindUObject(this, &APhysActor::SubstepTick);
}

// Called when the game starts or when spawned
void APhysActor::BeginPlay()
{
	Super::BeginPlay();

	//OnCalculateCustomPhysics.BindUObject(this, &APhysActor::PhysicsTick);
	
	//Get the BodyInstance PxRigidbody
	UPrimitiveComponent* Primitive = Cast<UPrimitiveComponent>(RootComponent);
	BodyInst = Primitive->GetBodyInstance();
	PhysicsActorHandle = &BodyInst->ActorHandle;
}

// Called every frame
void APhysActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (RootComponent->IsSimulatingPhysics())
	{
		BodyInst->AddCustomPhysics(OnCalculateCustomPhysics);
	}
}

void APhysActor::SubstepTick(float DeltaTime, FBodyInstance* BodyInstance)
{
	FVector CurrentLocation = GetLocation();
	FVector UpdatedLocation = CurrentLocation + CurrentLinearVelocity * DeltaTime;
	SetLocation(UpdatedLocation);
}

void APhysActor::ApplyLinearVelocity(FVector NewLinearVelocity)
{
	CurrentLinearVelocity = NewLinearVelocity;
}

void APhysActor::SetLocation(FVector NewLocation)
{
	PxTransform PhysicsTransform = PhysicsActorHandle->SyncActor->getGlobalPose();
	PxVec3 NewPhysicsLocation(NewLocation.X, NewLocation.Y, NewLocation.Z);

	PhysicsTransform.p = NewPhysicsLocation;
	PhysicsActorHandle->SyncActor->setGlobalPose(PhysicsTransform);
}

void APhysActor::SetRotation(FRotator NewRotation)
{
	PxTransform PhysicsTransform = PhysicsActorHandle->SyncActor->getGlobalPose();
	FQuat NewQuaternion = NewRotation.Quaternion();
	PxQuat NewPhysicsQuaternion(NewQuaternion.X, NewQuaternion.Y, NewQuaternion.Z, NewQuaternion.W);

	PhysicsTransform.q = NewPhysicsQuaternion;
	PhysicsActorHandle->SyncActor->setGlobalPose(PhysicsTransform);
}

FVector APhysActor::GetLocation()
{
	PxTransform PhysicsTransform = PhysicsActorHandle->SyncActor->getGlobalPose();
	return FVector(PhysicsTransform.p.x, PhysicsTransform.p.y, PhysicsTransform.p.z);
}

FRotator APhysActor::GetRotation()
{
	PxTransform PhysicsTransform = PhysicsActorHandle->SyncActor->getGlobalPose();
	return FRotator(FQuat(PhysicsTransform.q.x, PhysicsTransform.q.y, PhysicsTransform.q.z, PhysicsTransform.q.w));
}



