// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Enemy.generated.h"

UCLASS()
class RALLABALL_API AEnemy : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Enemy)
		USkeletalMeshComponent* EnemyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Collision)
		UBoxComponent* EnemyBodyCollision;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
