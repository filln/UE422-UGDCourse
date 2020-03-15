// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "MainCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "MainPlayerController.h"
// Sets default values
AEnemy::AEnemy()
{
    // Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    AgroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AgroSphere"));
    AgroSphere->SetupAttachment(GetRootComponent());
    AgroSphere->InitSphereRadius(600.f);

    CombatSphereTakeDamage = CreateDefaultSubobject<USphereComponent>(TEXT("CombatSphere"));
    CombatSphereTakeDamage->SetupAttachment(GetRootComponent());
    CombatSphereTakeDamage->InitSphereRadius(75.f);

    DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
    DamageSphere->SetupAttachment(GetRootComponent());
    DamageSphere->InitSphereRadius(55.f);

    CombatCollisionDoDamage = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageCollision"));
    CombatCollisionDoDamage->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));

    bOverlappingCombatSphere = false;

    Health = 75.f;
    MaxHealth = 100.f;
    Damage = 10.f;

    bAttacking = false;

    AttackMinTime = 0.5f;
    AttackMaxTime = 3.5f;

    SetMovementStatus(EEnemyMovementStatus::EMS_Idle);

    DeathDelay = 3.0f;

    bHasValidTarget = false;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
    Super::BeginPlay();
    
    AIController = Cast<AAIController>(GetController());

    AgroSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapBegin);
    AgroSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::AgroSphereOnOverlapEnd);

    CombatSphereTakeDamage->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatSphereTakeDamageOnOverlapBegin);
    CombatSphereTakeDamage->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatSphereTakeDamageOnOverlapEnd);

    CombatCollisionDoDamage->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::CombatCollisionDoDamageOnOverlapBegin);
    CombatCollisionDoDamage->OnComponentEndOverlap.AddDynamic(this, &AEnemy::CombatCollisionDoDamageOnOverlapEnd);

    GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
    GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

}

void AEnemy::AgroSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    /*if overlapped Actor is valid and Enemy is not dead than move to overlapped Actor.*/
    if (OtherActor && Alive())
    {
        AMainCharacter* Target = Cast<AMainCharacter>(OtherActor);
        if (Target)
        {
            MoveToTarget(Target);
        }
    }
}

void AEnemy::AgroSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    /*if overlapped Actor is valid and Enemy is not dead.*/
    if (OtherActor && Alive())
    {
        AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
        if (MainCharacter)
        {
            /*Stop movement.*/
            SetMovementStatus(EEnemyMovementStatus::EMS_Idle);
            if (AIController)
            {
                AIController->StopMovement();
            }

            /*Unset CombatTarget at MainCharacter.*/
            if (MainCharacter->CombatTarget == this)
            {
                MainCharacter->SetCombatTarget(nullptr);
            }

            /*Hide Enemy Healthbar.*/
            MainCharacter->SetHasCombatTarget(false);
            MainCharacter->UpdateCombatTarget();

            /*Unset CombatTarget at Enemy.*/
            CombatTarget = nullptr;
            bHasValidTarget = false;
        }
    }
}

void AEnemy::CombatSphereTakeDamageOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    /*if overlapped Actor is valid and Enemy is not dead.*/
    if (OtherActor && Alive())
    {
        AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
        if (MainCharacter)
        {
            bHasValidTarget = true;
            /*Set CombatTarget at MainCharacter*/
            MainCharacter->SetCombatTarget(this);
            MainCharacter->SetHasCombatTarget(true);

            /*Set new closest Enemy, display health bar.*/
            MainCharacter->UpdateCombatTarget();

            /*Set CombatTarget at Enemy.*/
            CombatTarget = MainCharacter;
            bOverlappingCombatSphere = true;

            /*Start attack.*/
            Attack();
        }
    }
}

void AEnemy::CombatSphereTakeDamageOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    /*if overlapped Actor is valid and Enemy is not dead.*/
    if (OtherActor && Alive() && OtherComp)
    {
        AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
        if (MainCharacter)
        {
            bOverlappingCombatSphere = false;
            MoveToTarget(MainCharacter);
            CombatTarget = nullptr;
            if (MainCharacter->CombatTarget == this)
            {
                MainCharacter->SetCombatTarget(nullptr);
                MainCharacter->bHasCombatTarget = false;
                MainCharacter->UpdateCombatTarget();
            }

            if (MainCharacter->MainPlayerController)
            {
                USkeletalMeshComponent* MainCharacterMesh = Cast<USkeletalMeshComponent>(OtherComp);
                if (MainCharacterMesh)
                {
                    MainCharacter->MainPlayerController->RemoveEnemyHealthBar();
                }
            }

            GetWorldTimerManager().ClearTimer(AttackTimer);
        }
    }
}

void AEnemy::CombatCollisionDoDamageOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    /*If combat collision overlap MainCharacter.*/
    if (OtherActor)
    {
        AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
        if (MainCharacter)
        {
            /*Spawn emitters in damage location.*/
            if (MainCharacter->HitParticles)
            {
                const USkeletalMeshSocket* TipSocket = GetMesh()->GetSocketByName("TipSocket");
                if (TipSocket)
                {
                    FVector TipSocketLocation = TipSocket->GetSocketLocation(GetMesh());
                    UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MainCharacter->HitParticles, TipSocketLocation, FRotator(0.f), false);
                }
            }

            /*Play sound.*/
            if (MainCharacter->HitSound)
            {
                UGameplayStatics::PlaySound2D(this, MainCharacter->HitSound);
            }

            /*Do damage.*/
            if (DamageTypeClass)
            {
                UGameplayStatics::ApplyDamage(MainCharacter, Damage, AIController, this, DamageTypeClass);
            }
        }
    }
}

void AEnemy::CombatCollisionDoDamageOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}


// Called every frame
void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::MoveToTarget(AMainCharacter* Target)
{
    SetMovementStatus(EEnemyMovementStatus::EMS_MoveToTarget);

    if (AIController)
    {
        /*Set variables for movement.*/
        FAIMoveRequest MoveRequest;
        MoveRequest.SetGoalActor(Target);
        MoveRequest.SetAcceptanceRadius(5.f);
        FNavPathSharedPtr NavPath;

        /*Move to MainCharacter.*/
        AIController->MoveTo(MoveRequest, &NavPath);

        /*
        auto PathPoints = NavPath->GetPathPoints();
        for (auto PathPoint : PathPoints)
        {
            FVector Location = PathPoint.Location;
            UKismetSystemLibrary::DrawDebugSphere(this, Location, 25.f, 8, FLinearColor::Red, 10.f, 1.5f);
        }
        */
    }
}

void AEnemy::ActivateCollision()
{
    CombatCollisionDoDamage->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

    /*Play sound for weapon attack movement.*/
    if (SwingSound)
    {
        UGameplayStatics::PlaySound2D(this, SwingSound);
    }
}

void AEnemy::DeactivateCollision()
{
    CombatCollisionDoDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemy::Attack()
{
    /*If attacking now than do nothing.*/
    if (bAttacking)
    {
        return;
    }
    
    if (AIController == nullptr || !bHasValidTarget)
    {
        return;
    }

    /*If Enemy die than do nothing.*/
    if (!Alive())
    {
        return;
    }

    /*Stop movement before attack.*/
    AIController->StopMovement();

    SetMovementStatus(EEnemyMovementStatus::EMS_Attacking);
    bAttacking = true;

    /*Play animation of attack.*/
    UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
    if (AnimInst && CombatMontage)
    {
        AnimInst->Montage_Play(CombatMontage, 1.5f);
        AnimInst->Montage_JumpToSection(FName("Attack"), CombatMontage);
    }
}

float AEnemy::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
    /*Reduce Enemy Health.*/
    DecrementHealth(DamageAmount, DamageCauser);
    return DamageAmount;
}

void AEnemy::DecrementHealth(float Amount, AActor* DamageCauser)
{
    /*Limit Health to a value equal or above zero.*/
    Health = FMath::Clamp(Health - Amount, (float)0, MaxHealth);

    /*If Health equal zero than die.*/
    if (Health == 0)
    {
        Die(DamageCauser);
    }
}

void AEnemy::Die(AActor* Causer)
{
    SetMovementStatus(EEnemyMovementStatus::EMS_Dead);

    /*Play death animation.*/
    UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
    if (AnimInst && CombatMontage)
    {
        AnimInst->Montage_Play(CombatMontage, 1.0f);
        AnimInst->Montage_JumpToSection(FName("Death"), CombatMontage);
    }

    /*Switch off the collision.*/
    GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    AgroSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CombatSphereTakeDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    DamageSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    CombatCollisionDoDamage->SetCollisionEnabled(ECollisionEnabled::NoCollision);

    AMainCharacter* MainCharacter = Cast<AMainCharacter>(Causer);
    if (MainCharacter)
    {
        MainCharacter->UpdateCombatTarget(); 
    }
}

bool AEnemy::Alive()
{
    /* Return true if Enemy alive.*/
    return GetMovementStatus() != EEnemyMovementStatus::EMS_Dead;
}

void AEnemy::Disappear()
{
    /*Destroy Enemy as Actor.*/
    Destroy();
}

void AEnemy::AttackEnd()
{
    bAttacking = false;
    SetMovementStatus(EEnemyMovementStatus::EMS_Idle);

    /*If MainCharacter in CombatSphere.*/
    if (bOverlappingCombatSphere)
    {
        /*Prepare Enemy for next attack.*/
        float AttackTime = FMath::FRandRange(AttackMinTime, AttackMaxTime);
        GetWorldTimerManager().SetTimer(AttackTimer, this, &AEnemy::Attack, AttackTime);
        
    }
    /*If MainCharacter not in CombatSphere.*/
    else
    {
        /*If CombatTarget is valid and MainCharacter in AgroSphere.*/
        if (CombatTarget)
        {
            /*Stop prepare for attack.*/
            GetWorldTimerManager().ClearTimer(AttackTimer);

            /*move to MainCharacter.*/
            MoveToTarget(CombatTarget);
        }
    }
}

void AEnemy::DeathEnd()
{
    /*After death animation pause all animations and movement and prepare for Destroy.*/
    GetMesh()->bPauseAnims = true;
    GetMesh()->bNoSkeletonUpdate = true;

    GetWorldTimerManager().SetTimer(DeathTimer, this, &AEnemy::Disappear, DeathDelay);
}
