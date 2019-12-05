// Fill out your copyright notice in the Description page of Project Settings.

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

	/*Начальная локация при вызове BeginPlay().*/
	UPROPERTY(EditInstanceOnly, Category = "FloaterVectors")
		FVector InitialLocation;

	/*Сохраненная локация при установке из редактора.*/
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "FloaterVectors")
		FVector PlacedLocation;

	/**/
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "FloaterVectors")
		FVector WorldOrigin;

	/*Вектор движения для AddActorLocalOffset().*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVectors")
		FVector InitialDirection;

	/**/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FloaterVectors")
		FVector InitialForce;

	/**/
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "FloaterVectors")
		FVector InitialTorque;

	/*Двигаться ли?*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloaterVariables")
		bool bShouldFloat;

	/*Перемещать ли на локацию InitialLocation в начале игры?*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "FloaterVariables")
		bool bInitializeFloaterLocations;

	/*Параметры колебаний*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloaterVariables | Wave parameters")
	float Amplitude;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloaterVariables | Wave parameters")
	float TimeStretch;


private:

	float RunningTime;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
