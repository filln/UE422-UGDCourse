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

	/*Запомнить вектор движения и очистить вектор-источник. 
	GetClampedToMaxSize(1.f) позволяет двигаться с одинаковой скоростью по прямой и диагональной траектории. 
	DeltaTime сглаживает разницу в скорости при разном ФПС.
	*/
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.f) * DeltaTime * 150.f;

	/*Если вектор не близок к нулю, то двигаться.*/
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		/*Если сталкиваемся с поверхностью, то скользить по ней.*/
		if (Hit.IsValidBlockingHit())
		{
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
		}
	}
}
