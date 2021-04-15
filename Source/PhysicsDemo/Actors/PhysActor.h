// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhysActor.generated.h"

UCLASS()
class PHYSICSDEMO_API APhysActor : public AActor
{
	GENERATED_BODY()
	
public:	
	APhysActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	//Sub-step Tick Function
	virtual void SubstepTick(float DeltaTime, FBodyInstance* BodyInstance);

	UFUNCTION(BlueprintCallable, Category = "Custom Physics")
		void ApplyLinearVelocity(FVector NewLinearVelocity);
	
private:
	FCalculateCustomPhysics OnCalculateCustomPhysics;

	FBodyInstance* BodyInst;
	FPhysicsActorHandle* PhysicsActorHandle;

	FVector CurrentLinearVelocity = FVector::ZeroVector;

	void SetLocation(FVector NewLocation);

	void SetRotation(FRotator NewRotation);

	FORCEINLINE FVector GetLocation();

	FORCEINLINE FRotator GetRotation();

	FORCEINLINE FVector GetLinearVelocity();

	FORCEINLINE FVector GetAngularVelocity();
};
