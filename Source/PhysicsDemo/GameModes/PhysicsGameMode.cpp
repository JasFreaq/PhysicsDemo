// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicsDemo/SceneComponents/SubstepPhysComponent.h"
#include "PhysicsGameMode.h"

void APhysicsGameMode::ApplyGravity()
{
	if (PhysicsBodies.Num() > 1)
	{
		if (GravityCounter == 0)
		{
			for (int32 i = 0, n = PhysicsBodies.Num(); i < n - 1; i++)
			{
				USubstepPhysComponent* BodyA = PhysicsBodies[i];

				for (int32 j = i + 1; j < n; j++)
				{
					USubstepPhysComponent* BodyB = PhysicsBodies[j];

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

		GravityCounter++;

		if (GravityCounter == PhysicsBodies.Num())
		{
			GravityCounter = 0;
		}
	}
}
