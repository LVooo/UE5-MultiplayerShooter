// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BlasterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	void SetHUDHealth(float Health, float MaxHealth);
	virtual void OnPossess(APawn* InPawn) override;
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDElimatedText(bool bHasElimated);
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;
};
