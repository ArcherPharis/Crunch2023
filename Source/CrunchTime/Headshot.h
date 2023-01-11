// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Headshot.generated.h"

/**
 * 
 */
UCLASS()
class CRUNCHTIME_API UHeadshot : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	virtual void BeginDestroy() override;

public:
	void DesaturateImage();
	void ReactToTakingDamage();

private:
	class FPreviewScene* renderScene;

	UPROPERTY(EditDefaultsOnly, Category = "Headshot")
	TSubclassOf<class ARenderActor> renderActorClass;
	ARenderActor* renderActor;

	UPROPERTY()
	class UTextureRenderTarget2D* renderTarget;

	UPROPERTY(meta = (BindWidget))
	class UImage* HeadShotImage;

	UPROPERTY(EditDefaultsOnly, Category = "Headshot")
	FName RenderTargetPropertyName;
	UPROPERTY(EditDefaultsOnly, Category = "Headshot")
	FName DesatValueName;

	UPROPERTY(EditDefaultsOnly, Category = "Headshot")
	int frameRate = 24;

	FTimerHandle RefreshTimer;
	FTimerHandle ResetImageTimer;


	void RefreshRender();
	void ResetImageColor();
	
};
