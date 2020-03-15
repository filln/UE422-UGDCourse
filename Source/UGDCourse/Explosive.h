// Fill out your copyright notice in the Description page of Project Settings.

/*Explosive item, that works after overlap MainCharacter.*/

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Explosive.generated.h"

/**
 *
 */

class UDamageType;

UCLASS()
class UGDCOURSE_API AExplosive : public AItem
{
	GENERATED_BODY()

public:

	AExplosive();

public:

	/*Damage to Actor.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
		float Damage;

	/*Use for method UGameplayStatics::ApplyDamage().*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
		TSubclassOf<UDamageType> DamageTypeClass;

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
