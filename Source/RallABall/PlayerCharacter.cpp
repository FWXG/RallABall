// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	// Make all spring components
	CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	CameraSpring->TargetArmLength = 500.0f;
	CameraSpring->bUsePawnControlRotation = true;
	CameraSpring->bEnableCameraLag = true;
	CameraSpring->CameraLagSpeed = 5.0f;

	// Make all camera components
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(CameraSpring, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;
	CameraSpring->SetupAttachment(RootComponent);

	// Hand box settings
	BoxHand = CreateDefaultSubobject<UBoxComponent>(TEXT("HandBox"));
	BoxHand->SetupAttachment(RootComponent);
	BoxHand->SetNotifyRigidBodyCollision(false);
	BoxHand->SetCollisionProfileName("NoCollision");
	BoxHand->bHiddenInGame = false;

	// leg box settings
	BoxLeg = CreateDefaultSubobject<UBoxComponent>(TEXT("LegBox"));
	BoxLeg->SetupAttachment(RootComponent);
	BoxLeg->SetNotifyRigidBodyCollision(false);
	BoxLeg->SetCollisionProfileName("NoCollision");
	BoxHand->bHiddenInGame = false;
	
	// Capsule settings
	GetCapsuleComponent()->InitCapsuleSize(40.0f, 100.0f);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	TurnRate = 45.0f;
	LookRate = 45.0f;
	PlayerSprint = 2.0f;

	// Make objects of attack anim
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MelleHandAttackObject(TEXT("AnimMontage'/Game/Objects/Person/Animation/Punch.Punch'"));
	static ConstructorHelpers::FObjectFinder<UAnimMontage> MeleeLegAttackObject(TEXT("AnimMontage'/Game/Objects/Person/Animation/LegAttack.LegAttack'"));
	
	// Anim check
	if (MelleHandAttackObject.Succeeded() && MeleeLegAttackObject.Succeeded())
	{
		MeleeHandAttack = MelleHandAttackObject.Object;
		MeleeLegAttack = MeleeLegAttackObject.Object;
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	const FAttachmentTransformRules RulesOfAttach(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget,
		EAttachmentRule::KeepWorld, false);
	// Attach attack box
	BoxHand->AttachToComponent(GetMesh(), RulesOfAttach, "hand_Punch_Socket");
	BoxLeg->AttachToComponent(GetMesh(), RulesOfAttach, "leg_kick_socket");

	BoxHand->OnComponentHit.AddDynamic(this, &APlayerCharacter::AttackHit);
	BoxLeg->OnComponentHit.AddDynamic(this, &APlayerCharacter::AttackHit);

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

	PlayerInputComponent->BindAction(TEXT("HandAttack"), IE_Pressed, this, &APlayerCharacter::HandAttackInput);
	PlayerInputComponent->BindAction(TEXT("HandAttack"), IE_Released, this, &APlayerCharacter::HandAttackEnd);

	PlayerInputComponent->BindAction(TEXT("LegAttack"), IE_Pressed, this, &APlayerCharacter::LegAttackInput);
	PlayerInputComponent->BindAction(TEXT("LegAttack"), IE_Released, this, &APlayerCharacter::LegAttackEnd);
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

void APlayerCharacter::AttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Blue, __FUNCTION__);
}

void APlayerCharacter::HandAttackStart()
{
	BoxHand->SetCollisionProfileName("Melee");
	BoxHand->SetNotifyRigidBodyCollision(true);
}

void APlayerCharacter::HandAttackEnd()
{
	BoxHand->SetNotifyRigidBodyCollision(false);
	BoxHand->SetCollisionProfileName("NoCollision");
}

void APlayerCharacter::HandAttackInput()
{
	PlayAnimMontage(MeleeHandAttack, 1.2f, FName("start_1"));
}

void APlayerCharacter::LegAttackInput()
{
	PlayAnimMontage(MeleeLegAttack, 1.0f, FName("start"));
}

void APlayerCharacter::LegAttackStart()
{
	BoxLeg->SetCollisionProfileName("Melee");
	BoxLeg->SetNotifyRigidBodyCollision(true);
}

void APlayerCharacter::LegAttackEnd()
{
	BoxLeg->SetNotifyRigidBodyCollision(false);
	BoxLeg->SetCollisionProfileName("NoCollision");
}