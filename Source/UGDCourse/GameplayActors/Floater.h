// Fill out your copyright notice in the Description page of Project Settings.

/*It's a three-point system floating in its amplitude. Or rather, its visual representation.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Floater.generated.h"

class UStaticMeshComponent;

UCLASS()
class UGDCOURSE_API AFloater : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloater();

public:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ActorMeshComponents")
		UStaticMeshComponent* StaticMesh;

	/*The initial random position of the Floater is set in BeginPlay().*/
	UPROPERTY(EditInstanceOnly, Category = "FloaterVectors")
		FVector InitialLocation;

	/*Floater position at the beginning of the game. Defines the installation of the Floater in the editor.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "FloaterVectors")
		FVector PlacedLocation;

	/*Not use.*/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "FloaterVectors")
		FVector WorldOrigin;

	/*Not use.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVectors")
		FVector InitialDirection;

	/*Not use.*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FloaterVectors")
		FVector InitialForce;

	/*Not use.*/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FloaterVectors")
		FVector InitialTorque;

	/*Moving on Tick()*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
		bool bShouldFloat;

	/*Move to InitialLocation at the beginning of the game.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FloaterVariables")
		bool bInitializeFloaterLocations;

	/*Oscillation amplitude.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloaterVariables | Wave parameters")
	float Amplitude;

	/*Multiplicator for oscillation parameters.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloaterVariables | Wave parameters")
	float TimeStretch;


private:

	/*Oscillation time for sin() and cos(). Depends on DeltaTime in Tick().*/
	float RunningTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
