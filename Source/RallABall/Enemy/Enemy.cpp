// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"

AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EnemyMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EnemyMesh"));
	EnemyMesh->SetSimulatePhysics(true);
	EnemyMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	RootComponent = EnemyMesh;

	EnemyBodyCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BodyCollision"));
	EnemyBodyCollision->SetNotifyRigidBodyCollision(true);
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

