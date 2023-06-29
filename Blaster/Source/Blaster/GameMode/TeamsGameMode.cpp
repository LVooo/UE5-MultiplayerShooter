// Fill out your copyright notice in the Description page of Project Settings.


#include "TeamsGameMode.h"

#include "Blaster/GameState/BlasterGameState.h"
#include "Blaster/PlayerState/BlasterPlayerState.h"
#include "Kismet/GameplayStatics.h"

ATeamsGameMode::ATeamsGameMode()
{
	bTeamsMatch = true;
}

void ATeamsGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	// 针对中间加入的游戏玩家
	ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BlasterGameState)
	{
		ABlasterPlayerState* BlasterPlayerState = NewPlayer->GetPlayerState<ABlasterPlayerState>();
		if (BlasterPlayerState && BlasterPlayerState->GetTeam() == ETeam::ET_NoTeam)
		{
			if (BlasterGameState->BlueTeam.Num() >= BlasterGameState->RedTeam.Num())
			{
				BlasterGameState->RedTeam.AddUnique(BlasterPlayerState);
				BlasterPlayerState->SetTeam(ETeam::ET_RedTeam);
			}
			else
			{
				BlasterGameState->BlueTeam.AddUnique(BlasterPlayerState);
				BlasterPlayerState->SetTeam(ETeam::ET_BlueTeam);
			}
		}
	}
}

void ATeamsGameMode::Logout(AController* Exiting)
{
	ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* BlasterPlayerState = Exiting->GetPlayerState<ABlasterPlayerState>();
	if (BlasterGameState && BlasterPlayerState)
	{
		if (BlasterGameState->RedTeam.Contains(BlasterPlayerState))
		{
			BlasterGameState->RedTeam.Remove(BlasterPlayerState);
		}
		if (BlasterGameState->BlueTeam.Contains(BlasterPlayerState))
		{
			BlasterGameState->BlueTeam.Remove(BlasterPlayerState);
		}
	}
}

float ATeamsGameMode::CalculateDamage(AController* Attacker, AController* Victim, float BaseDamage)
{
	ABlasterPlayerState* AttackerPState = Attacker->GetPlayerState<ABlasterPlayerState>();
    ABlasterPlayerState* VictimPState = Victim->GetPlayerState<ABlasterPlayerState>();
    if (AttackerPState == nullptr || VictimPState == nullptr) return BaseDamage;
    if (AttackerPState == VictimPState)
    {
    	return BaseDamage;
    }
    if (AttackerPState->GetTeam() == VictimPState->GetTeam())
    {
    	return 0.f;
    }
    return BaseDamage;
}

void ATeamsGameMode::PlayerEliminated(ABlasterCharacter* ElimmedCharacter, ABlasterPlayerController* VictimController,
	ABlasterPlayerController* AttackController)
{
	Super::PlayerEliminated(ElimmedCharacter, VictimController, AttackController);

	ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	ABlasterPlayerState* AttackerPlayerState = AttackController ? Cast<ABlasterPlayerState>(AttackController->PlayerState) : nullptr;
	if (BlasterGameState && AttackerPlayerState)
	{
		if (AttackerPlayerState->GetTeam() == ETeam::ET_BlueTeam)
		{
			BlasterGameState->BlueTeamScores();
		}
		if (AttackerPlayerState->GetTeam() == ETeam::ET_RedTeam)
		{
			BlasterGameState->RedTeamScores();
		}
	}
}

void ATeamsGameMode::HandleMatchHasStarted()
{
	Super::HandleMatchHasStarted();

	ABlasterGameState* BlasterGameState = Cast<ABlasterGameState>(UGameplayStatics::GetGameState(this));
	if (BlasterGameState)
	{
		// 可以直接从Gamestate中获取玩家状态集合
		for (auto PState : BlasterGameState->PlayerArray)
		{
			ABlasterPlayerState* BlasterPlayerState = Cast<ABlasterPlayerState>(PState.Get());
			if (BlasterPlayerState && BlasterPlayerState->GetTeam() == ETeam::ET_NoTeam)
			{
				if (BlasterGameState->BlueTeam.Num() >= BlasterGameState->RedTeam.Num())
				{
					BlasterGameState->RedTeam.AddUnique(BlasterPlayerState);
					BlasterPlayerState->SetTeam(ETeam::ET_RedTeam);
				}
				else
				{
					BlasterGameState->BlueTeam.AddUnique(BlasterPlayerState);
					BlasterPlayerState->SetTeam(ETeam::ET_BlueTeam);
				}
			}
		}

	}
}
