// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimAttackNotifyState.h"

void UAnimAttackNotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration) 
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Emerald, __FUNCTION__);

	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (Player != nullptr)
		{
			Player->LegAttackStart();
			Player->HandAttackStart();
		}
	}
}

void UAnimAttackNotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Emerald, __FUNCTION__);
	if (MeshComp != nullptr && MeshComp->GetOwner() != nullptr)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(MeshComp->GetOwner());
		if (Player != nullptr)
		{
			Player->LegAttackEnd();
			Player->HandAttackEnd();
		}
	}
}
