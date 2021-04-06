// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Kismet/GameplayStatics.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Input/Events.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/EngineTypes.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class RALLABALL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	void HandAttackStart();
	void HandAttackEnd();
	void LegAttackStart();
	void LegAttackEnd();
	void LegAttackInput();
	void HandAttackInput();
	class APlayerCharacter* Temp;

	class APlayerController* Input;

	UPROPERTY()
		bool isAttack = false;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		USpringArmComponent* CameraSpring;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		UBoxComponent* BoxHand;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		UBoxComponent* BoxLeg;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Charecter Movement: Walking")
		float PlayerSprint;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		class UAnimMontage* MeleeHandAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
		class UAnimMontage* MeleeLegAttack;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);
	void MoveRight(float value);
	void CameraPitch(float value);
	void CameraYaw(float value);
	void Sprint();
	void StopSprinting();

	UFUNCTION()
		void AttackHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
	/*UFUNCTION()
		void AttackOverlapBegin();*/

	//void DeathRagdoll()

	FVector2D CameraInput;
	FVector2D MoveInput;

	float TurnRate;
	float LookRate;
	

};
