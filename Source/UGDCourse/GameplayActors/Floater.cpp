﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "Floater.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFloater::AFloater()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CustomStaticMesh"));

	bInitializeFloaterLocations = false;
	bShouldFloat = false;

	InitialLocation = FVector(0.0f);
	PlacedLocation = FVector(0.0f);
	WorldOrigin = FVector(0.0f);
	InitialDirection = FVector(0.0f);
	InitialForce = FVector(2000000.0f, 0.0f, 0.0f);
	InitialTorque = FVector(2000000.0f, 0.0f, 0.0f);

	RunningTime = 0.f;

	Amplitude = 1.f;
	TimeStretch = 1.f;

}

// Called when the game starts or when spawned
void AFloater::BeginPlay()
{
	Super::BeginPlay();

	/*Not use.*/
	PlacedLocation = GetActorLocation();

	if (bInitializeFloaterLocations)
	{
		/*Set a random position.*/
		float InitialX = FMath::FRandRange(-500.f, 500.f);
		float InitialY = FMath::FRandRange(-500.f, 500.f);
		float InitialZ = FMath::FRandRange(0.f, 500.f);

		InitialLocation.X = InitialX;
		InitialLocation.Y = InitialY;
		InitialLocation.Z = InitialZ;

		SetActorLocation(InitialLocation);
	}

	//StaticMesh->AddForce(InitialForce);
	//StaticMesh->AddTorque(InitialTorque);




}

// Called every frame
void AFloater::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bShouldFloat)
	{
		/*Set the rotation for Floater.*/

		FVector NewLocation = GetActorLocation();

		NewLocation.X = NewLocation.X + Amplitude * FMath::Sin(TimeStretch * RunningTime);
		NewLocation.Z = NewLocation.Z + Amplitude * FMath::Cos(TimeStretch * RunningTime);

		SetActorLocation(NewLocation);

		RunningTime += DeltaTime;
	}

}

