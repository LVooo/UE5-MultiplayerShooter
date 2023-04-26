// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"

#include "GameFramework/GameStateBase.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	int32 NumberOfPlayers = GameState.Get()->PlayerArray.Num();
	if (NumberOfPlayers == 1)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			// 使用无缝传送
			bUseSeamlessTravel = true;
			// 传入地图链接并设置为监听服务器
			World->ServerTravel(FString("/Game/Maps/BlasterMap?listen"));
		}
	}
}
