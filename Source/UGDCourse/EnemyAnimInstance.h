// Fill out your copyright notice in the Description page of Project Settings.

/*Update values for animations. First initialization.*/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimInstance.generated.h"

class AEnemy;

/**
 * 
 */
UCLASS()
class UGDCOURSE_API UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	/*Speed of Enemy.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float MovementSpeed;

	/*Enemy as Pawn. For reference to Enemy after cast. For receiving Enemy speed.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		APawn* Pawn;

	/*Enemy after cast from Pawn. For receiving some Enemy variables.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		AEnemy* Enemy;

public:

	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
		void UpdateAnimationProperties();

	virtual void NativeInitializeAnimation() override;
};
