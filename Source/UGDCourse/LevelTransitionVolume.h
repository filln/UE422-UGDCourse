// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelTransitionVolume.generated.h"

class UBoxComponent;
class UBillboardComponent;
UCLASS()
class UGDCOURSE_API ALevelTransitionVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelTransitionVolume();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Transition")
		UBoxComponent* TransitionVolume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Transition")
		FName TransitionLevelName;

public:

	UBillboardComponent* Billboard;

public:

	/*Overlapped by MainCharacter.*/
	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
