// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/World.h"
#include "Enemy.h"
#include "AIController.h"


// Sets default values
ASpawnVolume::ASpawnVolume()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawningBox = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawningBox"));


}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();

	if (Actor_1 && Actor_2 && Actor_3 && Actor_4)
	{
		SpawnArray.Add(Actor_1);
		SpawnArray.Add(Actor_2);
		SpawnArray.Add(Actor_3);
		SpawnArray.Add(Actor_4);
	}
	
}

FVector ASpawnVolume::GetSpawnPoint()
{
	/*Get size of spawn volume.*/
	FVector Extent = SpawningBox->GetScaledBoxExtent();
	/*Get location of spawn folume.*/
	FVector Origin = SpawningBox->GetComponentLocation();

	/*Generate the random point.*/
	FVector Point = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);

	return Point;
}

TSubclassOf<AActor> ASpawnVolume::GetSpawnActor()
{
	if (SpawnArray.Num() > 0)
	{
		int32 Selection = FMath::RandRange(0, SpawnArray.Num() - 1);
		return SpawnArray[Selection];
	}
	else
	{
		return nullptr;
	}
}

void ASpawnVolume::SpawnOurActor_Implementation(UClass* ToSpawn, const FTransform& TransformOfSpawn)
{
	if (ToSpawn == nullptr)
	{
		return;
	}

	UWorld* World = GetWorld();
	if (World == nullptr)
	{
		return;
	}

	/*Spawn Critter.*/
	FActorSpawnParameters SpawnParams;
	AActor* Actor = World->SpawnActor<AActor>(ToSpawn, TransformOfSpawn, SpawnParams);

	/*Spawn the AIController and define the variable in Enemy for the AIController.*/
	AEnemy* Enemy = Cast<AEnemy>(Actor);
	if (Enemy)
	{
		Enemy->SpawnDefaultController();
		AAIController* AIContr = Cast<AAIController>(Enemy->GetController());
		if (AIContr)
		{
			Enemy->AIController = AIContr;
		}

	}

}


// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

