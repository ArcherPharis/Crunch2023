// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class CRUNCHTIME_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "GameModeBase")
	float SpawnInterval = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "GameModeBase")
	TSubclassOf<AActor> ActorToSpawn;

	UPROPERTY(EditDefaultsOnly, Category = "GameModeBase")
	class UBoxComponent* SpawnArea;
		
	FTimerHandle SpawnTimmer;

	void SpawnEnemy();
};
