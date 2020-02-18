// Fill out your copyright notice in the Description page of Project Settings.

/*Switch to open the door. When the character becomes on the switch, the switch moves down under the load of the character.
Then the switch signal opens the door.
*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorSwitch.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class UGDCOURSE_API AFloorSwitch : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloorSwitch();

public:

	/*Overlap volume for functionality to be triggered.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FloorSwitch")
		UBoxComponent* TriggerBox;

	/*Switch for the character to step on.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FloorSwitch")
		UStaticMeshComponent* FloorSwitchMesh;

	/*Door to move when the floor switch is stepped on.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "FloorSwitch")
		UStaticMeshComponent* DoorMesh;

	/*Initial position of the door.*/
	UPROPERTY(BlueprintReadWrite, Category = "FloorSwitch")
		FVector InitialDoorLocation;

	/*Initial position of the floor.*/
	UPROPERTY(BlueprintReadWrite, Category = "FloorSwitch")
		FVector InitialFloorLocation;

	/*The time after which the door closes.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloorSwitch")
		float SwitchTime;

private:

	/*Timer for when the door closes.*/
	FTimerHandle SwitchHandle;

public:

	/*Overlap with MainCharacter so that open the door.*/
	UFUNCTION()
		void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/*Overlap with MainCharacter so that close the door.*/
	UFUNCTION()
		void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/*Open the door.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
		void RaiseDoor();

	/*Close the door.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
		void LowerDoor();

	/*Raise the platform from which the character left.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
		void RiseFloor();

	/*Lower the platform the character walked into.*/
	UFUNCTION(BlueprintImplementableEvent, Category = "FloorSwitch")
		void LowerFloor();

	/*Update location in timeline. Turn the door up or down.*/
	UFUNCTION(BlueprintCallable, Category = "FloorSwitch")
		void UpdateDoorLocation(float Z);

	/*Update location in timeline. Turn the floor up or down.*/
	UFUNCTION(BlueprintCallable, Category = "FloorSwitch")
		void UpdateFloorLocation(float Z);

private:

	/*Some of the events associated with closing the door.*/
	void CloseDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
