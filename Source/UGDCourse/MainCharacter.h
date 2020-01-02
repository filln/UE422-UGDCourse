// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

/*Status of character movement.*/
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal		 UMETA(DisplayName = "Normal"),    //Normal speed (normal animation).
	EMS_Sprinting	 UMETA(DisplayName = "Sprinting"), //High speed (high-speed animation).
};

/*Status of stamina (resource for sprinting).*/
UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal					UMETA(DisplayName = "Normal"),				//Normal level, allow sprinting.
	ESS_BelowMinimum			UMETA(DisplayName = "BelowMinimum"),		//Minimum level, allow sprinting.
	ESS_Exhausted				UMETA(DisplayName = "Exhausted"),			//Zero level, does not allow.
	ESS_ExhaustedRecovering		UMETA(DisplayName = "ExhaustedRecovering"), //From zero to minimum level, does not allow.
};

UCLASS()
class UGDCOURSE_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		USpringArmComponent* CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FollowCamera;

	/*Scale turning functions for the camera.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseTurnRate;
	/*Scale look up functions for the camera.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		float BaseLookUpRate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player stats")
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		float Health;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player stats")
		float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		float Stamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		int32 Coins;

	/*For status of movement*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
		EMovementStatus MovementStatus;

	/*For status of stamina.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
		EStaminaStatus StaminaStatus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float RunningSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float SprintingSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		float StaminaDrainRate;

	/*Level of stamina that allow sprint mode.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		float MinSprintStamina;


public:

	bool bShiftKeyDown;

public:


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*Called for forwards/backwards input.*/
	void MoveForward(float Value);

	/*Called for side to side input.*/
	void MoveRight(float Value);

	/**Called via input to turn at a given input.
	* @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate.
	*/
	void TurnAtRate(float Rate);

	/**Called via input to look up/down at a given input.
	* @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired look up/down rate.
	*/
	void LookUpAtRate(float Rate);

	FORCEINLINE USpringArmComponent* GetCameraBoom() { return CameraBoom; }
	FORCEINLINE UCameraComponent* GetFollowCamera() { return FollowCamera; }

	/*Set movement status and running speed.*/
	void SetMovementStatus(EMovementStatus NewStatus);

	/*Set new status of stamina.*/
	FORCEINLINE void SetStaminaStatus(EStaminaStatus NewStaminaStatus) { StaminaStatus = NewStaminaStatus; }

	/*Decrease Health when take damage.*/
	void DecrementHealth(float Amount);

	/*Die when Health == 0.*/
	void Die();

	/*Increase coins.*/
	void IncrementCoins(int32 Amount);

	/*Pressed down to enable sprinting.*/
	void ShiftKeyDown();

	/*Released up to disable sprinting.*/
	void ShiftKeyUp();

};
