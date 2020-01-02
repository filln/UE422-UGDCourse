// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;
class UStaticMeshComponent;
class UParticleSystemComponent;
class UParticleSystem;
class USoundCue;

UCLASS()
class UGDCOURSE_API AItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AItem();

public:

	/*Base collision.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Collision")
		USphereComponent* CollisionVolume;
	/*Base static mesh.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Mesh")
		UStaticMeshComponent* Mesh;
	/*Base particle.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		UParticleSystemComponent* IdleParticlesComponent;
	/*Base particle for overlap CollisionVolume.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		UParticleSystem* OverlapParticles;
	/*Transform for OverlapParticles. Loaction = GetActorLocation()*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Particles")
		FTransform OverlapParticlesTransform;
	/*Base sound for overlap.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Sound")
		USoundCue* OverlapSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Properties")
		bool bRotate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item | Properties")
		float RotationRate;

public:

	UFUNCTION()
		virtual void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
