// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComp");
	SpawnArea = CreateDefaultSubobject<UBoxComponent>("SpawnArea");
	SpawnArea->SetupAttachment(RootComponent);
	SpawnArea->SetCollisionResponseToAllChannels(ECR_Ignore);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(SpawnTimmer, this, &AEnemySpawner::SpawnEnemy, SpawnInterval, true);
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemySpawner::SpawnEnemy()
{
	FVector extend = SpawnArea->GetUnscaledBoxExtent();
	FVector center = SpawnArea->GetComponentLocation();
	
	float spawnDistanceFromCenter = FMath::RandRange(-1, 1);
	FVector SpawnLoc = center + extend * spawnDistanceFromCenter;

	FActorSpawnParameters spawnParams;
	spawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	GetWorld()->SpawnActor<AActor>(ActorToSpawn, SpawnLoc,FRotator::ZeroRotator,spawnParams);
}
