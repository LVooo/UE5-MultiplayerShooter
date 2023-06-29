// Fill out your copyright notice in the Description page of Project Settings.


#include "BlasterGameState.h"

#include "Net/UnrealNetwork.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"

void ABlasterGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABlasterGameState, TopScoringPlayers);
	DOREPLIFETIME(ABlasterGameState, RedTeamScore);
	DOREPLIFETIME(ABlasterGameState, BlueTeamScore);
}

void ABlasterGameState::UpdateTopScore(ABlasterPlayerState* ScoringPlayer)
{
	if (TopScoringPlayers.Num() == 0)
	{
		TopScoringPlayers.Add(ScoringPlayer);
		TopScores = ScoringPlayer->GetScore();
	}
	else if (TopScores == ScoringPlayer->GetScore())
	{
		TopScoringPlayers.AddUnique(ScoringPlayer);
	}
	else if (ScoringPlayer->GetScore() > TopScores)
	{
		TopScoringPlayers.Empty();
		TopScoringPlayers.AddUnique(ScoringPlayer);
		TopScores = ScoringPlayer->GetScore();
	}
}

void ABlasterGameState::OnRep_RedTeamScore()
{
}

void ABlasterGameState::OnRep_BlueTeamScore()
{
}
