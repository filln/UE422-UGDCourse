// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

class UStaticMeshComponent;

UCLASS()
class UGDCOURSE_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFloatingPlatform();

public:

	/*Mesh for the platform.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FloatingPlatform")
		UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, Category = "FloatingPlatform")
		FVector StartPoint;

	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"), Category = "FloatingPlatform")
		FVector EndPoint;

	/*Speed of platform.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloatingPlatform")
		float InterpSpeed;
	/*Pause between movings.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloatingPlatform")
		float InterpTime;

private:

	FTimerHandle InterpTimer;
	bool bInterping;
	float Distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ToggleInterping();
	void SwapVectors(FVector& VecOne, FVector& VecTwo);

};
