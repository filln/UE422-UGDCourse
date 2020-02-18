// Fill out your copyright notice in the Description page of Project Settings.

/*Class for Enemy. Can move to MainCharacter and attack him.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class USphereComponent;
class AAIController;
class AMainCharacter;
class UParticleSystem;
class USoundCue;
class UBoxComponent;
class UAnimMontage;
class UDamageType;

/*Different states. Used to control animation, to switch between different actions: movement, attack, idle.*/
UENUM(BlueprintType)
enum class EEnemyMovementStatus : uint8
{
	EMS_Idle			 UMETA(DisplayName = "Idle"),
	EMS_MoveToTarget	 UMETA(DisplayName = "MoveToTarget"),
	EMS_Attacking		 UMETA(DisplayName = "Attacking"),
	EMS_Dead			 UMETA(DisplayName = "Dead"),
	 
	EMS_MAX				 UMETA(DisplayName = "DefaultMax")
};


UCLASS()
class UGDCOURSE_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

public:

	/*Current status for Enemy.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		EEnemyMovementStatus EnemyMovementStatus;

	/*If MainCharacter pass through sphere, than Enemy move to him.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* AgroSphere;

	/*If MainCharacter pass through sphere, than Enemy attack him.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* CombatSphereTakeDamage;

	/*To react to weapons MainCharacter through custom collision channel (Damage). */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		USphereComponent* DamageSphere;

	/*Is installed at weapons Enemy. For attack to MainCharacter.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		UBoxComponent* CombatCollisionDoDamage;

	/*Use for moving.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		AAIController* AIController;

	/*Whether MainCharacter crosses the CombatSphereTakeDamage collision. Used for attack. */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
		bool bOverlappingCombatSphere;

	/*Remember reference to MainCharacter if he crosses the CombatSphereTakeDamage. Use in MoveToTarget(CombatTarget). Nullptr if AgroSphereOnOverlapEnd().*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
		AMainCharacter* CombatTarget;

	/*Maximum Health. Use in HUD.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AI")
		float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Health;

	/*Damage. Use in HUD.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		UParticleSystem* HitParticles;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		USoundCue* HitSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		USoundCue* SwingSound;

	/*Animations of attack and death.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		UAnimMontage* CombatMontage;

	/*Cannot attack if the attack animation is already playing.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Combat")
		bool bAttacking;

	/*To calculate a random attack frequency in the range of AttackMinTime, AttackMaxTime. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackMinTime;
	/*To calculate a random attack frequency in the range of AttackMinTime, AttackMaxTime. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float AttackMaxTime;

	/*Use for method UGameplayStatics::ApplyDamage().*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;

	/*Delay before Destroy().*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		float DeathDelay;

public:

	/*For frequency of attack.*/
	FTimerHandle AttackTimer;

	/*For delay before Destroy().*/
	FTimerHandle DeathTimer;


public:

	/*Overlap methods for AgroSphere.*/
	UFUNCTION()
		virtual void AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*Overlap methods for CombatSphereTakeDamage.*/
	UFUNCTION()
		virtual void CombatSphereTakeDamageOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void CombatSphereTakeDamageOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*Overlap methods for CombatCollisionDoDamage.*/
	UFUNCTION()
		void CombatCollisionDoDamageOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void CombatCollisionDoDamageOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*Move to MainCharacter if it locate in some radius.*/
	UFUNCTION(BlueprintCallable)
		void MoveToTarget(AMainCharacter* Target);

	/*Activates the collision CombatCollisionDoDamage in the right animation trajectory so as not to attack randomly. */
	UFUNCTION(BlueprintCallable)
		void ActivateCollision();
	/*Deactivates the collision CombatCollisionDoDamage in the right animation trajectory so as not to attack randomly. */
	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();

	/*Called at the end of the attack animation when Enemy need to launch the next attack or move to MainCharacter.*/
	UFUNCTION(BlueprintCallable)
		void AttackEnd();

	/*Called at the end of the attack Death when Enemy need prepare to Destroy().*/
	UFUNCTION(BlueprintCallable)
		void DeathEnd();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE void SetMovementStatus(EEnemyMovementStatus NewMovementStatus) { EnemyMovementStatus = NewMovementStatus; }
	FORCEINLINE EEnemyMovementStatus GetMovementStatus() { return EnemyMovementStatus; }

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*Stop movement and play attack animation.*/
	void Attack();

	/*Decrease in health after an attack.*/
	virtual float TakeDamage
	(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
	) override;

	/*Decrease Health when take damage.*/
	void DecrementHealth(float Amount);

	/*Die when Health == 0.*/
	void Die();

	/*Checking the enemy is not dead.*/
	bool Alive();

	/*Destroy().*/
	void Disappear();
};
