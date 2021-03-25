// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;


	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraSpring->TargetArmLength = 500.0f;
	CameraSpring->bUsePawnControlRotation = true;
	CameraSpring->bEnableCameraLag = true;
	CameraSpring->CameraLagSpeed = 5.0f;

	

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(CameraSpring, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	CameraSpring->SetupAttachment(RootComponent);

	BoxPerson = CreateDefaultSubobject<UBoxComponent>(TEXT("MelleBox"));
	BoxPerson->SetupAttachment(RootComponent);
	BoxPerson->bHiddenInGame = false;
	
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 100.0f);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	TurnRate = 45.0f;
	LookRate = 45.0f;
	PlayerSprint = 2.0f;
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//Camera RotationPitch
	FRotator NewRotationPitch = CameraSpring->GetComponentRotation();
	NewRotationPitch.Pitch = FMath::Clamp(NewRotationPitch.Pitch + CameraInput.Y, -80.0f, -15.0f);
	CameraSpring->SetWorldRotation(NewRotationPitch);

	//Camera RotationYaw
	FRotator NewYawRotation = GetActorRotation();
	NewYawRotation.Yaw += CameraInput.X;
	SetActorRotation(NewYawRotation);

	if (APlayerCharacter::GetVelocity() != FVector(0.0f, 0.0f, 0.0f))
	{
		GetCharacterMovement()->JumpZVelocity = 400.0f;
	}

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Bind input
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &APlayerCharacter::MoveRight);

	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Pressed, this, &APlayerCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("Jump"), IE_Released, this, &APlayerCharacter::StopJumping);

	PlayerInputComponent->BindAxis("MousePitch", this, &APlayerCharacter::CameraPitch);
	PlayerInputComponent->BindAxis("MouseYaw", this, &APlayerCharacter::CameraYaw);

	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Pressed,this, &APlayerCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), IE_Released,this, &APlayerCharacter::StopSprinting);

	PlayerInputComponent->BindAction(TEXT("LegAttack"), IE_Pressed, this, &APlayerCharacter::LegAttack);
	PlayerInputComponent->BindAction(TEXT("LegAttack"), IE_Released, this, &APlayerCharacter::LegAttack);
}

void APlayerCharacter::MoveForward(float value)
{
	const FRotator rotarorIs = Controller->GetControlRotation();
	const FRotator Yaw(0, rotarorIs.Yaw, 0);
	const FVector direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::X);
	AddMovementInput(direction, value);
}

void APlayerCharacter::MoveRight(float value)
{
	const FRotator rotarorSecond = Controller->GetControlRotation();
	const FRotator Yaw(0, rotarorSecond.Yaw, 0);
	const FVector direction = FRotationMatrix(Yaw).GetUnitAxis(EAxis::Y);
	AddMovementInput(direction, value);
}

void APlayerCharacter::CameraPitch(float AxisValue)
{
	AddControllerPitchInput(AxisValue * GetWorld()->GetDeltaSeconds() * TurnRate);
}

void APlayerCharacter::CameraYaw(float AxisValue)
{
	AddControllerYawInput(AxisValue * GetWorld()->GetDeltaSeconds() * LookRate);
}

void APlayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= PlayerSprint;
}

void APlayerCharacter::StopSprinting()
{
	GetCharacterMovement()->MaxWalkSpeed /= PlayerSprint;
}

void APlayerCharacter::LegAttack()
{

}

