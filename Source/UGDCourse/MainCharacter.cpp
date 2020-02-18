// Fill out your copyright notice in the Description page of Project Settings.


#include "MainCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/UnrealMathUtility.h"
#include "Weapon.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemy.h"
#include "MainPlayerController.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Create springArm.
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->TargetArmLength = 600.f;

	//Rotate arm based on controller.
	CameraBoom->bUsePawnControlRotation = true;

	GetCapsuleComponent()->SetCapsuleSize(49, 110);

	//Create camera.
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	BaseTurnRate = 65.f;
	BaseLookUpRate = 65.f;

	/*Dont rotate when the controller rotates.*/
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	/*Rotate to direction of movement.*/
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 650.f;
	GetCharacterMovement()->AirControl = 0.2f;

	Health = 65.f;
	MaxHealth = 100.f;
	Stamina = 120.f;
	MaxStamina = 150.f;
	Coins = 0;

	RunningSpeed = 650.f;
	SprintingSpeed = 950.f;

	bShiftKeyDown = false;
	bLMBDown = false;

	MovementStatus = EMovementStatus::EMS_Normal;
	StaminaStatus = EStaminaStatus::ESS_Normal;

	StaminaDrainRate = 25.f;
	MinSprintStamina = 50.f;

	InterpSpeed = 15.f;
	bInterpToEnemy = false;

	bHasCombatTarget = false;
}


// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	MainPlayerController = Cast<AMainPlayerController>(GetController());

}

void AMainCharacter::SetMovementStatus(EMovementStatus NewStatus)
{
	/*Set new status*/
	MovementStatus = NewStatus;

	/*Set new speed depending on MovementStatus.*/
	if (MovementStatus == EMovementStatus::EMS_Sprinting)
	{
		GetCharacterMovement()->MaxWalkSpeed = SprintingSpeed;
	}
	else
	{
		GetCharacterMovement()->MaxWalkSpeed = RunningSpeed;
	}
}


// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*Use stamina and sprint.*/
	StaminaManagement(DeltaTime);

	/*Rotate to Enemy.*/
	if (bInterpToEnemy && CombatTarget)
	{
		FRotator LookAtYaw = GetLookAtRotationYaw(CombatTarget->GetActorLocation());
		FRotator InterpRotation = FMath::RInterpTo(GetActorRotation(), LookAtYaw, DeltaTime, InterpSpeed);
		SetActorRotation(InterpRotation);
	}

	/*Set Enemy location.*/
	if (CombatTarget)
	{
		CombatTargetLocation = CombatTarget->GetActorLocation();
		MainPlayerController->EnemyLocation = CombatTargetLocation;
	}

}

void AMainCharacter::StaminaManagement(float DeltaTime)
{
	/*Increase stamina per tick.*/
	float DeltaStamina = StaminaDrainRate * DeltaTime;

	switch (StaminaStatus)
	{
	case EStaminaStatus::ESS_Normal:
		/*If Shift key down.*/
		if (bShiftKeyDown)
		{
			/*Decrease stamina*/
			Stamina -= DeltaStamina;
			if (Stamina <= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_BelowMinimum);
			}
			/*Set status to sprinting.*/
			SetMovementStatus(EMovementStatus::EMS_Sprinting);
		}
		/*If shift key up.*/
		else
		{
			/*Increase stamina.*/
			Stamina += DeltaStamina;

			/*Limit the value.*/
			if (Stamina >= MaxStamina)
			{
				Stamina = MaxStamina;
			}

			/*Set status to Normal.*/
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_BelowMinimum:
		if (bShiftKeyDown)
		{
			Stamina -= DeltaStamina;
			if (Stamina <= 0.f)
			{
				Stamina = 0.f;
				SetMovementStatus(EMovementStatus::EMS_Normal);
				SetStaminaStatus(EStaminaStatus::ESS_Exhausted);
			}
			else
			{
				SetMovementStatus(EMovementStatus::EMS_Sprinting);
			}
		}
		else
		{
			Stamina += DeltaStamina;
			if (Stamina >= MinSprintStamina)
			{
				SetStaminaStatus(EStaminaStatus::ESS_Normal);
			}
			SetMovementStatus(EMovementStatus::EMS_Normal);
		}
		break;

	case EStaminaStatus::ESS_Exhausted:
		SetMovementStatus(EMovementStatus::EMS_Normal);
		if (bShiftKeyDown)
		{
			Stamina = 0.f;
		}
		else
		{
			SetStaminaStatus(EStaminaStatus::ESS_ExhaustedRecovering);
		}
		break;

	case EStaminaStatus::ESS_ExhaustedRecovering:
		SetMovementStatus(EMovementStatus::EMS_Normal);
		Stamina += DeltaStamina;
		if (Stamina >= MinSprintStamina)
		{
			SetStaminaStatus(EStaminaStatus::ESS_Normal);
		}
		break;

	default:
		;
	}
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::ShiftKeyDown);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::ShiftKeyUp);

	PlayerInputComponent->BindAction("LMB", IE_Pressed, this, &AMainCharacter::LMBDown);
	PlayerInputComponent->BindAction("LMB", IE_Released, this, &AMainCharacter::LMBUp);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);

	PlayerInputComponent->BindAxis("CameraYaw", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("CameraPitch", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("CameraYawRate", this, &AMainCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("CameraPitchRate", this, &AMainCharacter::LookUpAtRate);
}

void AMainCharacter::MoveForward(float Value)
{
	/*Not move.*/
	if (Controller == nullptr || Value == 0 || bAttacking)
	{
		return;
	}

	/*Get Direction of movement from current rotation.*/
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	/*Move to Direction in X axe.*/
	AddMovementInput(Direction, Value);
}

void AMainCharacter::MoveRight(float Value)
{
	/*Not move.*/
	if (Controller == nullptr || Value == 0 || bAttacking)
	{
		return;
	}

	/*Get Direction of movement from current rotation.*/
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	/*Move to Direction in Y axe.*/
	AddMovementInput(Direction, Value);
}

void AMainCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::DecrementHealth(float Amount)
{
	/*Limit Health from zero to maximum.*/
	Health = FMath::Clamp(Health - Amount, (float)0, MaxHealth);

	if (Health == 0)
	{
		Die();
	}
}

float AMainCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	DecrementHealth(DamageAmount);
	return DamageAmount;
}

void AMainCharacter::Die()
{
	/*Play death animation.*/
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst && CombatMontage)
	{
		AnimInst->Montage_Play(CombatMontage, 1.0f);
		AnimInst->Montage_JumpToSection(FName("Death"), CombatMontage);
	}
}

void AMainCharacter::IncrementCoins(int32 Amount)
{
	Coins += Amount;
}

void AMainCharacter::ShiftKeyDown()
{
	bShiftKeyDown = true;
}

void AMainCharacter::ShiftKeyUp()
{
	bShiftKeyDown = false;
}

void AMainCharacter::LMBDown()
{
	bLMBDown = true;

	/*If character ovelappings some weapon than equips it. Else it attacks.*/

	if (ActiveOverlappingItem)
	{
		AWeapon* Weapon = Cast<AWeapon>(ActiveOverlappingItem);
		if (Weapon)
		{
			Weapon->Equip(this);
			SetActiveOverlappingItem(nullptr);
		}
	}
	else
	{
		if (EquippedWeapon)
		{
			Attack();
		}
	}

}

void AMainCharacter::LMBUp()
{
	bLMBDown = false;
}

void AMainCharacter::SetEquippedWeapon(AWeapon* NewWeapon)
{
	/*If character has weapon than destroy it and equipps new weapon.*/
	if (EquippedWeapon)
	{
		EquippedWeapon->Destroy();
	}

	EquippedWeapon = NewWeapon;
}

void AMainCharacter::Attack()
{
	/*Do nothing if character attack now.*/
	if (bAttacking)
	{
		return;
	}

	bAttacking = true;

	/*Rotate to Enemy.*/
	SetInterpToEnemy(true);

	/*Play random attack animation in CombatMontage.*/
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst && CombatMontage)
	{
		AnimInst->Montage_Play(CombatMontage, 1.35f);

		int32 Section = FMath::RandRange(0, 1);
		switch (Section)
		{
		case 0:
			AnimInst->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
			break;
		case 1:
			AnimInst->Montage_JumpToSection(FName("Attack_2"), CombatMontage);
			break;

		default:
			AnimInst->Montage_JumpToSection(FName("Attack_1"), CombatMontage);
			break;
		}
	}
}

FRotator AMainCharacter::GetLookAtRotationYaw(FVector Target)
{
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), Target);
	FRotator LookAtRotationYaw(0.f, LookAtRotation.Yaw, 0.f);
	return LookAtRotationYaw;
}

void AMainCharacter::DeathEnd()
{
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	SetLifeSpan(3.0f);
}

void AMainCharacter::SetInterpToEnemy(bool NewbInterpToEnemy)
{
	bInterpToEnemy = NewbInterpToEnemy;
}

void AMainCharacter::AttackEnd()
{
	bAttacking = false;

	/*Not rotate to enemy.*/
	SetInterpToEnemy(false);

	/*Repeat attack if pressing Left Mouse Button.*/
	if (bLMBDown)
	{
		Attack();
	}
}

void AMainCharacter::PlaySwingSound()
{
	if (EquippedWeapon)
	{
		if (EquippedWeapon->SwingSound)
		{
			UGameplayStatics::PlaySound2D(this, EquippedWeapon->SwingSound);
		}
	}
}
