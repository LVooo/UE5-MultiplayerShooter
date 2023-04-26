// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "OverHeadWidget.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UOverHeadWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(meta=(BindWidget)) // 加上这个之后可以关联蓝图中与C++类中的同名变量
	class UTextBlock* DisplayText;
	UPROPERTY(meta=(BindWidget))
	class UTextBlock* DisplayUser;

	void SetDisplayText(FString TextToDisplay, FString ClientName); // 传入在头顶显示的网络角色以及机器名

	UFUNCTION(BlueprintCallable)
	void ShowPlayerNetRole(APawn* InPawn);

protected:
	virtual void OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld) override;
};
