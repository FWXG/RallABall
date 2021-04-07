// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "PlayerCharacter.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerInput.h"
#include "AnimAttackNotifyState.generated.h"

/**
 * 
 */
UCLASS()
class RALLABALL_API UAnimAttackNotifyState : public UAnimNotifyState
{
	GENERATED_BODY()

public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	class APlayerCharacter* MyPlayer;
	class APlayerController* Input;
};
