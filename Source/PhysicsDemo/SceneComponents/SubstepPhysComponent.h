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
	// Sets default values for this component's properties
	USubstepPhysComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	//Sub-step Tick Function
	virtual void PhysicsTick(float DeltaTime, FBodyInstance* BodyInstance);
	
private:
	FCalculateCustomPhysics OnCalculateCustomPhysics;

	UPrimitiveComponent* PrimitiveParent;
	
	FBodyInstance* BodyInst;

	FVector CurrentLinearVelocity = FVector::ZeroVector;

	void ApplyLinearVelocity(FVector NewLinearVelocity);

	void SetLocation(FVector NewLocation);

	void SetRotation(FRotator NewRotation);

	FORCEINLINE FTransform GetTransform();
	
	FORCEINLINE FVector GetLocation();

	FORCEINLINE FRotator GetRotation();
};
