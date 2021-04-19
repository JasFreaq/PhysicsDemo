// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PHYSICSDEMO_API PhysicsUtilities
{
public:
	static void GetBoneTransformRecursive(FTransform& Transform, const USkeletalMeshComponent* SkeletalMeshComponent, const FName& Bone);

	static FTransform GetBoneRefTransformInComponentSpace(const USkeletalMeshComponent* SkeletalMeshComponent, const FName& Bone);

	static FTransform GetSubstepComponentWorldTransform(const USceneComponent* SceneComponent, FBodyInstance* ParentBodyInstance);
};
