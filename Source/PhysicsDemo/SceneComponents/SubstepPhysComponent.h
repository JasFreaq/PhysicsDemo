// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SubstepPhysComponent.generated.h"

#define UNIVERSAL_GRAVITATIONAL_CONSTANT 6.67408e-11f
#define EARTH_MASS 5.972e24f
#define EARTH_RADIUS 637800000 //6978 kms

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPhysicsTickSignature, float, DeltaTime);

class APhysicsGameMode;

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

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Custom Physics")
		FORCEINLINE float GetPhysicsMass();
	
protected:
	UPROPERTY(BlueprintAssignable, Category = "Custom Physics", meta = (AllowPrivateAccess = true))
		FPhysicsTickSignature OnPhysicsTick;

	UPROPERTY()
		UPrimitiveComponent* PrimitiveParent = nullptr;
	
	FBodyInstance* BodyInst = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Custom Physics", meta = (AllowPrivateAccess = true))
		void SetPhysicsLocation(FVector NewLocation);

	UFUNCTION(BlueprintCallable, Category = "Custom Physics", meta = (AllowPrivateAccess = true))
		void SetPhysicsRotation(FRotator NewRotation);

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Custom Physics", meta = (AllowPrivateAccess = true))
		FORCEINLINE FTransform GetPhysicsTransform();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Custom Physics", meta = (AllowPrivateAccess = true))
		FORCEINLINE FVector GetPhysicsLocation();

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Custom Physics", meta = (AllowPrivateAccess = true))
		FORCEINLINE FRotator GetPhysicsRotation();
	
private:
	UPROPERTY(EditAnywhere, Category = "Custom Physics")
		bool bSimulateEarthGravity = false;
	
	FCalculateCustomPhysics OnCalculateCustomPhysics;

	FVector CurrentLinearVelocity = FVector::ZeroVector;
	FVector CurrentLinearAcceleration = FVector::ZeroVector;
	FVector CurrentResultantForce = FVector::ZeroVector;

	APhysicsGameMode* PhysicsGameMode = nullptr;
	
	void ApplyGravity();
};