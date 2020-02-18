// Fill out your copyright notice in the Description page of Project Settings.


#include "ColliderMovementComponent.h"

UColliderMovementComponent::UColliderMovementComponent()
{

}

void UColliderMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return;
	}

	/*Remember the motion vector and clear the source vector.
	GetClampedToMaxSize(1.f) allows you to move with the same speed on a straight and diagonal trajectory.
	DeltaTime smoothes the difference in speed at different FPS.
	*/
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.f) * DeltaTime * 150.f;

	/*If the vector is not close to zero, then move.*/
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		/*If we run into a surface, slide on it.*/
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}
