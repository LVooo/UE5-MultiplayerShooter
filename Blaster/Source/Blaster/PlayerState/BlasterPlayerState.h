// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Blaster/BlasterTypes/Team.h"
#include "BlasterPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API ABlasterPlayerState : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/*
	 * 得分的notify
	 */
	virtual void OnRep_Score() override;

	UFUNCTION()
	virtual void OnRep_Defeats();

	void AddToScore(float ScoreAmount);
	void AddToDefeats(int32 DefeatsAmount);
	void SetElimatedText(bool bHasElimated);

private:
	UPROPERTY()
	class ABlasterCharacter* Character;
	UPROPERTY()
	class ABlasterPlayerController* Controller;

	UPROPERTY(ReplicatedUsing=OnRep_Defeats)
	int32 Defeats;

	UPROPERTY(Replicated)
	ETeam Team = ETeam::ET_NoTeam;

public:
	FORCEINLINE ETeam GetTeam() const { return Team; }
	FORCEINLINE void SetTeam(ETeam TeamToSet) { Team = TeamToSet; }
};
