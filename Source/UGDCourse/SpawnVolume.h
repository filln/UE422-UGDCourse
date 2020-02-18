// Fill out your copyright notice in the Description page of Project Settings.

/*Place for spawn Critter in random point of this place.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

class UBoxComponent;
class ACritter;

UCLASS()
class UGDCOURSE_API ASpawnVolume : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASpawnVolume();

public:

	/*The area in which the object will spawn.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
		UBoxComponent* SpawningBox;

	/*Class for spawn.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "SpawnVolume")
		TSubclassOf<ACritter> PawnToSpawn;

public:

	/*Return random vector of location for spawn object.*/
	UFUNCTION(BlueprintPure, Category = "SpawnVolume")
		FVector GetSpawnPoint();

	/*Blueprint event for spawn our ToSpawn class.*/
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "SpawnVolume")
		void SpawnOurPawn(UClass* ToSpawn, const FTransform& TransformOfSpawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
