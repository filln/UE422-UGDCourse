// Fill out your copyright notice in the Description page of Project Settings.

/*Player Controller for MainCharacter. Use for display HUD.*/

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

class UUserWidget;

/*Structure for configuration of Enemy Health Bar.*/
USTRUCT(BlueprintType)
struct FEnemyHealthBarParameters
{
	GENERATED_USTRUCT_BODY()

		/*Offset relative to position in viewport.*/
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		float DeltaXEnemyHealthBar = 0.f;

	/*Offset relative to position in viewport.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		float DeltaYEnemyHealthBar = 0.f;

	/*Size of Health bar.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		FVector2D SizeInViewport = FVector2D(200.f, 25.f);
};

UCLASS()
class UGDCOURSE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	/*Reference to the UMG asset in the editor.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> HUDOverlayAsset;

	/*Variable to hold the widget after creating it.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* HUDOverlay;

	/*Structure for configuration of Enemy Health Bar.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		FEnemyHealthBarParameters EnemyHealthBarParameters;

	/*Class of Enemy Health bar.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<UUserWidget> WEnemyHealthBar;

	/*Object of Enemy Health bar.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		UUserWidget* EnemyHealthBar;


public:
	/*Do not specify the position of the health widget in Tick().*/
	bool bEnemyHealthBarVisible;

	/*For position of the health widget.*/
	FVector EnemyLocation;


public:

	/*Show or hide widget.*/
	void DisplayEnemyHealthBar();
	void RemoveEnemyHealthBar();

protected:

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
};
