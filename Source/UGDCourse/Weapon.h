// Fill out your copyright notice in the Description page of Project Settings.

/*Class for weapon of MainCharacter.*/

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Weapon.generated.h"

class USkeletalMeshComponent;
class AMainCharacter;
class USoundCue;
class UBoxComponent;
class UDamageType;
class AController;

/*State of weapon.*/
UENUM(BlueprintType)
enum class EWeaponState : uint8
{
	EWS_Pickup		UMETA(DisplayName = "Pickup"),		//Default state: ready for pickup.
	EWS_Equipped	UMETA(DisplayName = "Equipped"),	//Equipped by MainCharacter.

	EWS_DefaultMax	UMETA(DisplayName = "DefaultMax")	//Unused.
};

/**
 *
 */
UCLASS()
class UGDCOURSE_API AWeapon : public AItem
{
	GENERATED_BODY()

public:
	AWeapon();

public:

	/*For animations.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
		USkeletalMeshComponent* SkeletalMesh;

	/*Collision for overlap enemy.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Combat")
		UBoxComponent* CombatCollision;

	/*Sound for equipping.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		USoundCue* onEquipSound;

	/*Sound for attack.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		USoundCue* SwingSound;

	/*Save particles after equipping.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item | Particles")
		bool bWeaponParticles;

	/*State of weapon.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item")
		EWeaponState WeaponState;

	/*Damage of the weapon.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Combat")
		float Damage;

	/*For settings Damage in Blueprints.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;

	/*Variable for ApplyDamage. It is the MainPlayerController.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		AController* WeaponInstigator;



public:

	/*Overlap methods for CombatCollision.*/
	UFUNCTION()
		void CombatOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void CombatOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*Activate or deactivate collision in some point of animation.*/
	UFUNCTION(BlueprintCallable)
		void ActivateCollision();
	UFUNCTION(BlueprintCallable)
		void DeactivateCollision();

public:

	/*Overlap methods for CollisionVolume inherited by Item.*/
	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	/*Equip MainCharacter with the weapon.*/
	void Equip(AMainCharacter* Char);

	FORCEINLINE void SetWeaponState(EWeaponState NewWeaponState) { WeaponState = NewWeaponState; }
	FORCEINLINE EWeaponState GetWeaponState() { return WeaponState; }

	FORCEINLINE void SetInstigator(AController* NewInstigator) { WeaponInstigator = NewInstigator; }

protected:

	virtual void BeginPlay() override;
};
