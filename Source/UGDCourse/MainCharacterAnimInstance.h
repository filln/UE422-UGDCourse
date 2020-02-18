// Fill out your copyright notice in the Description page of Project Settings.

/*Update values for animations. First initialization.*/

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "MainCharacterAnimInstance.generated.h"

class APawn;
class AMainCharacter;

/**
 *
 */
UCLASS()
class UGDCOURSE_API UMainCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	/*Normal speed of MainCharacter.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float MovementSpeed;

	/*True if MainCharacter be in air. Use for jump animation.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		bool bIsInAir;

	/*References for get some status.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		APawn* Pawn;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		AMainCharacter* MainCharacter;

public:

	/*Update values for animation.*/
	UFUNCTION(BlueprintCallable, Category = "AnimationProperties")
		void UpdateAnimationProperties();

	/*First initialization.*/
	virtual void NativeInitializeAnimation() override;


};
