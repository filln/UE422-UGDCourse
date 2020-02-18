// Fill out your copyright notice in the Description page of Project Settings.

/*Simple Pawn. Knows how to move at the touch of a key. Movement is implemented without Movement Component. Also can be controlled by AI.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Critter.generated.h"

class USkeletalMeshComponent;
class UCameraComponent;

UCLASS()
class UGDCOURSE_API ACritter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACritter();

public:

	UPROPERTY(EditAnywhere, Category = "Mesh")
		USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "Camera")
		UCameraComponent* Camera;

	/*Maximal movement speed.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Movement")
		float MaxSpeed;

private:

	/*To save the movement speed calculated in MoveForward, MoveRight.*/
	FVector CurrentVelocity;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	void MoveForward(float Value);
	void MoveRight(float Value);

};
