// Fill out your copyright notice in the Description page of Project Settings.


#include "Headshot.h"
#include "PreviewScene.h"
#include "RenderActor.h"
#include "EngineUtils.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/Image.h"

void UHeadshot::NativeConstruct()
{
	Super::NativeConstruct();
	renderScene = new FPreviewScene(FPreviewScene::ConstructionValues().SetEditor(false));
	renderActor = renderScene->GetWorld()->SpawnActor<ARenderActor>(renderActorClass);
	renderTarget = NewObject<UTextureRenderTarget2D>(GetTransientPackage());
	renderTarget->InitAutoFormat(256, 256);
	renderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8_SRGB;
	renderTarget->Filter = TextureFilter::TF_MAX;
	renderActor->AssignRenderTarget(renderTarget);

	HeadShotImage->GetDynamicMaterial()->SetTextureParameterValue(RenderTargetPropertyName, renderTarget);
}

void UHeadshot::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	if (renderScene && renderScene->GetWorld())
	{
		UWorld* renderWorld = renderScene->GetWorld();
		if (!renderWorld->bBegunPlay)
		{
			//begin play
			//loop through all actors with iterators and dispatch their begin play
			for (FActorIterator actorIter{ renderWorld }; actorIter; ++actorIter)
			{
				actorIter->DispatchBeginPlay();
			}
			renderWorld->bBegunPlay = true;
			float resfreshRate = 1.f / (float)frameRate;
			GetWorld()->GetTimerManager().SetTimer(RefreshTimer, this, &UHeadshot::RefreshRender, resfreshRate, true);
			return;
		}
		renderWorld->Tick(ELevelTick::LEVELTICK_All, InDeltaTime);
		//renderActor->UpdateCapture(renderWorld->Scene);

	}

	//tick
	
}

void UHeadshot::BeginDestroy()
{
	delete renderScene;
	renderScene = nullptr;
	Super::BeginDestroy();
}

void UHeadshot::DesaturateImage()
{
	HeadShotImage->GetDynamicMaterial()->SetScalarParameterValue(DesatValueName, 1.0f);
	HeadShotImage->SetOpacity(0.8f);
}

void UHeadshot::RefreshRender()
{

	if(renderScene)
	renderActor->UpdateCapture(renderScene->GetWorld()->Scene);
}
