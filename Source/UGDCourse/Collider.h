﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Collider.generated.h"

class UStaticMeshComponent;
class USphereComponent;
class UCameraComponent;
class USpringArmComponent;
class UColliderMovementComponent;

UCLASS()
class UGDCOURSE_API ACollider : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACollider();

public:

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* Mesh;
	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		USphereComponent* SphereCollision;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		UCameraComponent* Camera;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		UColliderMovementComponent* OurMovementComponent;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE UStaticMeshComponent* GetMesh() { return Mesh; };
	FORCEINLINE void SetMesh(UStaticMeshComponent* InMesh) { Mesh = InMesh; };

	FORCEINLINE USphereComponent* GetSphere() { return SphereCollision; };
	FORCEINLINE void SetSphere(USphereComponent* InSphere) { SphereCollision = InSphere; };

	FORCEINLINE UCameraComponent* GetCamera() { return Camera; };
	FORCEINLINE void SetCamera(UCameraComponent* InCamera) { Camera = InCamera; };

	FORCEINLINE USpringArmComponent* GetSpringArm() { return SpringArm; };
	FORCEINLINE void SetSpringArm(USpringArmComponent* InSpringArm) { SpringArm = InSpringArm; };

	virtual UPawnMovementComponent* GetMovementComponent() const override;

private:

	void MoveForward(float Value);
	void MoveRight(float Value);
};