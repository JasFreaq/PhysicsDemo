// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/SceneComponent.h"
#include "PhysicsUtilities.h"

void PhysicsUtilities::GetBoneTransformRecursive(FTransform& Transform,
	const USkeletalMeshComponent* SkeletalMeshComponent, const FName& Bone)
{
	Transform *= FTransform(SkeletalMeshComponent->SkeletalMesh->GetRefPoseMatrix(SkeletalMeshComponent->GetBoneIndex(Bone)));
	const FName ParentBone = SkeletalMeshComponent->GetParentBone(Bone);
	if (ParentBone != NAME_None)
	{
		GetBoneTransformRecursive(Transform, SkeletalMeshComponent, ParentBone);
	}
}

FTransform PhysicsUtilities::GetBoneRefTransformInComponentSpace(const USkeletalMeshComponent* SkeletalMeshComponent,
	const FName& Bone)
{
	FTransform OutTransform = FTransform::Identity;
	GetBoneTransformRecursive(OutTransform, SkeletalMeshComponent, Bone);
	return OutTransform;
}

FTransform PhysicsUtilities::GetSubstepComponentWorldTransform(const USceneComponent* SceneComponent,
	FBodyInstance* ParentBodyInstance)
{
	const auto ParentBodyInstanceTransform = [&]()
	{
		if (SceneComponent->GetAttachSocketName() == NAME_None)
		{
			// If we are attached to a skel mesh, and the socket is None, we are attached to the skel mesh component, and not one of its bones.
			// If this is the case, then we need to adjust for the fact that bodyInstance->GetUnrealWorldTransform() is going to give us the transform
			// of the root bone, and NOT the transform of the skeletal mesh component.
			//
			// This can be done by getting the reference transform of the root bone in component space, and then "un-rotate" the relative component transform with this ref transform.
			if (USkeletalMeshComponent* SkelMeshComponent = Cast<USkeletalMeshComponent>(ParentBodyInstance->OwnerComponent.Get()))
			{
				const auto RootBoneName = SkelMeshComponent->GetBoneName(0);
				const auto BodyInstanceRefPosition = GetBoneRefTransformInComponentSpace(SkelMeshComponent, RootBoneName);
				return BodyInstanceRefPosition.Inverse() * ParentBodyInstance->GetUnrealWorldTransform();
			}
		}


		return ParentBodyInstance->GetUnrealWorldTransform();
	}();

	// We might not be directly attached to the body instance, in this case walk the component chain until we find it
	struct RetV { FTransform Transform; bool bAttachedToBodyInstance; };
	const auto RelativeToParentInstanceTransform = [&]()->RetV
	{
		auto relativeTransform = SceneComponent->GetRelativeTransform();
		auto sceneComp = SceneComponent;
		while (sceneComp->GetAttachParent() != nullptr && sceneComp->GetAttachParent() != ParentBodyInstance->OwnerComponent)
		{
			sceneComp = sceneComp->GetAttachParent();
			relativeTransform *= sceneComp->GetRelativeTransform();
			if (sceneComp->GetAttachParent() == nullptr)
				return { relativeTransform, false };
		}

		return { relativeTransform, true };
	}();

	auto RelativeTransform = RelativeToParentInstanceTransform.bAttachedToBodyInstance
		? RelativeToParentInstanceTransform.Transform * ParentBodyInstanceTransform
		: RelativeToParentInstanceTransform.Transform;

	// Take into account Absolute transforms
	if (SceneComponent->IsUsingAbsoluteLocation())
		RelativeTransform.SetLocation(SceneComponent->GetComponentLocation());
	if (SceneComponent->IsUsingAbsoluteRotation())
		RelativeTransform.SetRotation(SceneComponent->GetComponentQuat());
	if (SceneComponent->IsUsingAbsoluteScale())
		RelativeTransform.SetScale3D(SceneComponent->GetComponentScale());

	return RelativeTransform;
}

