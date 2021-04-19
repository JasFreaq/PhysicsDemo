// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SubstepPhysComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PHYSICSDEMO_API USubstepPhysComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	USubstepPhysComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Sub-step Tick Function
	virtual void PhysicsTick(float DeltaTime, FBodyInstance* BodyInstance);

	UFUNCTION(BlueprintCallable, Category = "Custom Physics")
		void ApplyForce(FVector Force);
	
private:
	UPrimitiveComponent* PrimitiveParent;	
	FBodyInstance* BodyInst;
	FCalculateCustomPhysics OnCalculateCustomPhysics;

	FVector CurrentLinearVelocity = FVector::ZeroVector;
	FVector CurrentLinearAcceleration = FVector::ZeroVector;
	FVector CurrentResultantForce = FVector::ZeroVector;

	void SetLocation(FVector NewLocation);

	void SetRotation(FRotator NewRotation);

	FORCEINLINE FTransform GetTransform();
	
	FORCEINLINE FVector GetLocation();

	FORCEINLINE FRotator GetRotation();
};
