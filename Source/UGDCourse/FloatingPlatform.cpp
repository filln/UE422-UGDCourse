// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	StartPoint = FVector(0.f);
	EndPoint = FVector(0.f);
	InterpSpeed = 4.f;
	bInterping = false;
	InterpTime = 1.f;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	StartPoint = GetActorLocation();

	/*Get EndPoint in World Space.*/
	EndPoint += StartPoint;

	/*Delay in launching the platform.*/
	GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);
	Distance = (EndPoint - StartPoint).Size();

}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bInterping)
	{
		/*Move platform every Tick to point Interp.*/
		FVector CurrentLocation = GetActorLocation();
		FVector Interp = FMath::VInterpTo(CurrentLocation, EndPoint, DeltaTime, InterpSpeed);
		SetActorLocation(Interp);
	}

	/*Get distance between current location and StartPoint.*/
	float DistanceTraveled = (GetActorLocation() - StartPoint).Size();

	/*If platform come to EndPoint.*/
	if (Distance - DistanceTraveled <= 0.1f)
	{
		/*Stop moving.*/
		ToggleInterping();

		/*Delay in launching the platform.*/
		GetWorldTimerManager().SetTimer(InterpTimer, this, &AFloatingPlatform::ToggleInterping, InterpTime);

		/*Swap start and end points.*/
		SwapVectors(StartPoint, EndPoint);
	}

}

void AFloatingPlatform::ToggleInterping()
{
	bInterping = !bInterping;
}

void AFloatingPlatform::SwapVectors(FVector& VecOne, FVector& VecTwo)
{
	FVector VecTmp = VecOne;
	VecOne = VecTwo;
	VecTwo = VecTmp;
}

