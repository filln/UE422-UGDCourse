// Fill out your copyright notice in the Description page of Project Settings.


#include "Collider.h"
#include "Components/StaticMeshComponent.h"
#include "Components//SphereComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ColliderMovementComponent.h"

// Sets default values
ACollider::ACollider()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	RootComponent = SphereCollision;
	SphereCollision->SetSphereRadius(40.f);
	SphereCollision->SetCollisionProfileName(TEXT("Pawn"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(GetRootComponent());


	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->RelativeRotation = FRotator(-45.f, 0.f, 0.f);
	SpringArm->TargetArmLength = 400.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	OurMovementComponent = CreateDefaultSubobject<UColliderMovementComponent>(TEXT("ColliderMovementComponent"));
	OurMovementComponent->UpdatedComponent = GetRootComponent();

	CameraInput = FVector2D(0.f, 0.f);

	/*Find the mesh asset.*/
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	/*If found, identify the mesh.*/
	if (MeshAsset.Succeeded())
	{
		Mesh->SetStaticMesh(MeshAsset.Object);
		Mesh->SetRelativeLocation(FVector(0.f, 0.f, -40.f));
		Mesh->SetWorldScale3D(FVector(0.8f));
	}

	AutoPossessPlayer = EAutoReceiveInput::Player0;

}

// Called when the game starts or when spawned
void ACollider::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/*Pawn rotation depending on the mouse moving.*/
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += CameraInput.X;
	SetActorRotation(NewRotation);

	/*Camera and SpringArm rotation depending on the mouse moving.*/
	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();
	NewSpringArmRotation.Pitch = FMath::Clamp(NewSpringArmRotation.Pitch += CameraInput.Y, -80.f, -15.f);
	SpringArm->SetWorldRotation(NewSpringArmRotation);


}

// Called to bind functionality to input
void ACollider::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ACollider::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ACollider::MoveRight);

	PlayerInputComponent->BindAxis(TEXT("CameraPitch"), this, &ACollider::CameraPitch);
	PlayerInputComponent->BindAxis(TEXT("CameraYaw"), this, &ACollider::CameraYaw);

}

UPawnMovementComponent* ACollider::GetMovementComponent() const
{
	return OurMovementComponent;
}

void ACollider::MoveForward(float Value)
{
	FVector ForwardVector = GetActorForwardVector();

	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(Value * ForwardVector, true);
	}


}

void ACollider::MoveRight(float Value)
{
	FVector RightVector = GetActorRightVector();

	if (OurMovementComponent)
	{
		OurMovementComponent->AddInputVector(Value * RightVector, true);
	}


}

void ACollider::CameraYaw(float AxisValue)
{
	CameraInput.X = AxisValue;
}

void ACollider::CameraPitch(float AxisValue)
{
	CameraInput.Y = AxisValue;
}


