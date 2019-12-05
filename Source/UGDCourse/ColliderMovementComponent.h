﻿// Fill out your copyright notice in the Description page of Project Settings.
/*
Компонент для движения павна.
*/
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "ColliderMovementComponent.generated.h"

/**
 *
 */
UCLASS()
class UGDCOURSE_API UColliderMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()

public:
	UColliderMovementComponent();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

};