// Fill out your copyright notice in the Description page of Project Settings.

/*General character. It can move and attack. */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AWeapon;
class AItem;
class UAnimMontage;
class USoundCue;
class UParticleSystem;
class AEnemy;
class AMainPlayerController;

/*Status of character movement.*/
UENUM(BlueprintType)
enum class EMovementStatus : uint8
{
	EMS_Normal		 UMETA(DisplayName = "Normal"),    //Normal speed (normal animation).
	EMS_Sprinting	 UMETA(DisplayName = "Sprinting"), //High speed (high-speed animation).
	EMS_Dead		 UMETA(DisplayName = "Dead")		//Not movable.
};

/*Status of stamina (resource for sprinting).*/
UENUM(BlueprintType)
enum class EStaminaStatus : uint8
{
	ESS_Normal					UMETA(DisplayName = "Normal"),				//Normal level, allow sprinting.
	ESS_BelowMinimum			UMETA(DisplayName = "BelowMinimum"),		//Minimum level, allow sprinting.
	ESS_Exhausted				UMETA(DisplayName = "Exhausted"),			//Zero level, does not allow sprinting.
	ESS_ExhaustedRecovering		UMETA(DisplayName = "ExhaustedRecovering"), //From zero to minimum level, does not allow sprinting.
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

	/*Maximum of Health.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player stats")
		float MaxHealth;

	/*Current Health.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		float Health;

	/*Maximum of Stamina.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player stats")
		float MaxStamina;

	/*Current Stamina.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		float Stamina;

	/*Current count of coins.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		int32 Coins;

	/*For status of movement*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
		EMovementStatus MovementStatus;

	/*For status of stamina.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enums")
		EStaminaStatus StaminaStatus;

	/*Standart speed of moving.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float RunningSpeed;

	/*Increased speed of moving.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float SprintingSpeed;

	/*The rate of stamina reduction in a sprint. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		float StaminaDrainRate;

	/*Level of stamina that allow sprint mode.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player stats")
		float MinSprintStamina;

	/*Weapon what the character has.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Item")
		AWeapon* EquippedWeapon;

	/*Current item that overlapping now.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item")
		AItem* ActiveOverlappingItem;

	/*True if now playing animation of attack.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Anims")
		bool bAttacking;

	/*Animation Montage for combat.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
		UAnimMontage* CombatMontage;

	/*Particles that show in attack.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		UParticleSystem* HitParticles;

	/*Sound that play in attack.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		USoundCue* HitSound;

	/*Reference to current enemy.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		AEnemy* CombatTarget;

	/*True if CombatTarget is valid. Use in Enemy.cpp and Enemy Health Bar Widget. Set by SetHasCombatTarget().*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool bHasCombatTarget;

	/*Use for set some variables in MainPlayerController.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Controller")
		AMainPlayerController* MainPlayerController;

	/*Location of current Enemy.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Combat")
		FVector CombatTargetLocation;

	/*Reference to class of Enemy for UpdateCombatTarget().*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<AEnemy> EnemyFilter;

public:

	/*True if Shift pressed. Use for management of stamina.*/
	bool bShiftKeyDown;

	/*True if Left Mouse Button pressed. Use for a repeat attack.*/
	bool bLMBDown;

	/*Speed of rotate to enemy after attack him.*/
	float InterpSpeed;

	/*If true than rotate to enemy after attack him.*/
	bool bInterpToEnemy;

	/*True if character moves.*/
	bool bMovingForward;

	/*True if character moves.*/
	bool bMovinRight;

	/*Set bInterpToEnemy.*/
	void SetInterpToEnemy(bool NewbInterpToEnemy);


public:

	/*Work after end of attack animation. Use for some events after attack.*/
	UFUNCTION(BlueprintCallable, Category = "Anims")
		void AttackEnd();

	/*Play sound of attack.*/
	UFUNCTION(BlueprintCallable, Category = "Sound")
		void PlaySwingSound();

	/*Work after end of death animation. Use for some events after death.*/
	UFUNCTION(BlueprintCallable)
		void DeathEnd();

	/*Increase coins.*/
	UFUNCTION(BlueprintCallable)
		void IncrementCoins(int32 Amount);

	/*Healing.*/
	UFUNCTION(BlueprintCallable)
		void IncrementHealth(float Amount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*Work in Tick(). Use for management sprint mode.*/
	void StaminaManagement(float DeltaTime);

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


	/*Set movement status and running speed.*/
	void SetMovementStatus(EMovementStatus NewStatus);


	/*Decrease Health when take damage.*/
	void DecrementHealth(float Amount);

	/*Decrement health after take damage.*/
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	/*Die when Health == 0.*/
	void Die();

	/*Pressed down to enable sprinting.*/
	void ShiftKeyDown();

	/*Released up to disable sprinting.*/
	void ShiftKeyUp();

	/*Call when the Left Mouse Button was pressed.*/
	void LMBDown();

	/*Call when the Left Mouse Button was released.*/
	void LMBUp();

	/*Change weapon or first set it.*/
	void SetEquippedWeapon(AWeapon* NewWeapon);

	/*Play attack animation.*/
	void Attack();

	/*Jumping.*/
	virtual void Jump() override;

	/*Return rotator to Target. Use for rotate to Enemy after attack him.*/
	FRotator GetLookAtRotationYaw(FVector Target);

	/*Set new closest in distance CombatTarget. Display Enemy HealthBar.*/
	void UpdateCombatTarget();

	/*Set new reference to Enemy.*/
	FORCEINLINE void SetCombatTarget(AEnemy* NewCombatTarget) { CombatTarget = NewCombatTarget; }
	FORCEINLINE AEnemy* GetCombatTarget() { return CombatTarget; }

	/*Set new status of stamina.*/
	FORCEINLINE void SetStaminaStatus(EStaminaStatus NewStaminaStatus) { StaminaStatus = NewStaminaStatus; }

	/*Return pointer to current weapon.*/
	FORCEINLINE AWeapon* GetEquippedWeapon() { return EquippedWeapon; }

	/*Set pointer to current overlapping item.*/
	FORCEINLINE void SetActiveOverlappingItem(AItem* NewItem) { ActiveOverlappingItem = NewItem; }

	/*Return SpringArm.*/
	FORCEINLINE USpringArmComponent* GetCameraBoom() { return CameraBoom; }

	/*Return Camera.*/
	FORCEINLINE UCameraComponent* GetFollowCamera() { return FollowCamera; }

	/*Set bHasCombatTarget. Use in Enemy.cpp and Enemy Health Bar Widget.*/
	FORCEINLINE void SetHasCombatTarget(bool NewHasCombatTarget) { bHasCombatTarget = NewHasCombatTarget; }

};
