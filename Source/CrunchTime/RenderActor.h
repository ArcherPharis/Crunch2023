// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RenderActor.generated.h"

UCLASS()
class CRUNCHTIME_API ARenderActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARenderActor();

	void AssignRenderTarget(class UTextureRenderTarget2D* renderTarget);

	void UpdateCapture(FSceneInterface* scene);

	void PlayReactMontage();

	USkeletalMeshComponent* GetTAMesh() const { return mesh; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(VisibleDefaultsOnly, Category = "RenderActor")
	class USceneCaptureComponent2D* Capturer;

	UPROPERTY(EditDefaultsOnly, Category = "RenderActor")
	USkeletalMeshComponent* mesh;

	UPROPERTY(EditDefaultsOnly, Category = "RenderActor")
	UAnimMontage* reactMontage;

};
