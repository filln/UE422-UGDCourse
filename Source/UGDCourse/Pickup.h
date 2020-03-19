// Fill out your copyright notice in the Description page of Project Settings.

/*Class for lifting objects. Specifically - coins.*/

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "Pickup.generated.h"

/**
 *
 */

class AMainCharacter;

UCLASS()
class UGDCOURSE_API APickup : public AItem
{
	GENERATED_BODY()

public:
	APickup();

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Pickup")
		void OnPickUp_BP(AMainCharacter* Target);

public:

	virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

	virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

};
