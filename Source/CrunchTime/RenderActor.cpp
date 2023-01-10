// Fill out your copyright notice in the Description page of Project Settings.


#include "RenderActor.h"
#include "Components/SceneCaptureComponent2D.h"

// Sets default values
ARenderActor::ARenderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	Capturer = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("Capturer"));
	Capturer->SetupAttachment(RootComponent);


}

void ARenderActor::AssignRenderTarget(UTextureRenderTarget2D* renderTarget)
{
	Capturer->TextureTarget = renderTarget;
}

void ARenderActor::UpdateCapture(FSceneInterface* scene)
{
	Capturer->UpdateDeferredCaptures(scene);
}

// Called when the game starts or when spawned
void ARenderActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Begin play"));
	
}

// Called every frame
void ARenderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("tick"));


}

