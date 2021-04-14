// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MeshComponent.h"
#include "PhysMeshComponent.generated.h"

/**
 * 
 */
UCLASS()
class PHYSICSDEMO_API UPhysMeshComponent : public UMeshComponent
{
	GENERATED_BODY()

public:
	UPhysMeshComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Sub-step Tick Function
	virtual void PhysicsTick(float DeltaTime, FBodyInstance* BodyInstance);

	UFUNCTION(BlueprintCallable, Category = "Custom Physics")
		void ApplyLinearVelocity(FVector NewLinearVelocity);
	
private:
	FCalculateCustomPhysics OnCalculateCustomPhysics;

	//Rigidbody Cache
	const FPhysicsActorHandle* PhysicsActorHandle;

	FVector CurrentLinearVelocity = FVector::ZeroVector;
	
	void SetLocation(FVector NewLocation);

	void SetRotation(FRotator NewRotation);
	
	FORCEINLINE FVector GetLocation();

	FORCEINLINE FRotator GetRotation();

	FORCEINLINE FVector GetLinearVelocity();

	FORCEINLINE FVector GetAngularVelocity();
};
