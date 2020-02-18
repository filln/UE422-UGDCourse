// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAnimInstance.h"
#include "Enemy.h"

void UEnemyAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		/*Speed.*/
		FVector Speed = Pawn->GetVelocity();

		/*Speed at XY axis.*/
		FVector LateralSpeed = FVector(Speed.X, Speed.Y, 0.f);

		/*Scalar of speed.*/
		MovementSpeed = LateralSpeed.Size();

		if (Enemy == nullptr)
		{
			Enemy = Cast<AEnemy>(Pawn);
		}
	}
}

void UEnemyAnimInstance::NativeInitializeAnimation()
{

	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	if (Pawn)
	{
		Enemy = Cast<AEnemy>(Pawn);
	}

}
