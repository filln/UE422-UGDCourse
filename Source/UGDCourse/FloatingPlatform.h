// Fill out your copyright notice in the Description page of Project Settings.

/*A platform that moves independently from one point to another. Designed to move the character.*/

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

	/*First point of motion.*/
	UPROPERTY(EditAnywhere, Category = "FloatingPlatform")
		FVector StartPoint;

	/*Second point of motion. In Local space.*/
	UPROPERTY(EditAnywhere, meta = (MakeEditWidget = "true"), Category = "FloatingPlatform")
		FVector EndPoint;

	/*Speed of platform.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloatingPlatform")
		float InterpSpeed;

	/*Pause between movings.*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "FloatingPlatform")
		float InterpTime;

private:

	/*Timer for pause between movings.*/
	FTimerHandle InterpTimer;

	/*If true than move. If false than stay.*/
	bool bInterping;

	/*Distance between StartPoint and EndPoint.*/
	float Distance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*Switch bInterping.*/
	void ToggleInterping();

	/*Swap StartPoint and EndPoint for change of direction.*/
	void SwapVectors(FVector& VecOne, FVector& VecTwo);

};
