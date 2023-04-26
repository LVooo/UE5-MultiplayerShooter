// Fill out your copyright notice in the Description page of Project Settings.


#include "OverHeadWidget.h"

#include "Components/TextBlock.h"
#include "GameFramework/PlayerState.h"

void UOverHeadWidget::SetDisplayText(FString TextToDisplay, FString ClientName)
{
	if (DisplayText)
	{
		DisplayText->SetText(FText::FromString(TextToDisplay));
	}

	if (DisplayUser)
	{
		DisplayUser->SetText(FText::FromString(ClientName));
	}
}

void UOverHeadWidget::ShowPlayerNetRole(APawn* InPawn)
{
	// 获取网络角色
	const ENetRole RemoteRole = InPawn->GetRemoteRole();
	FString Role;
	switch (RemoteRole)
	{
	case ENetRole::ROLE_Authority:
		Role = FString("Authority");
		break;
	case ENetRole::ROLE_AutonomousProxy:
		Role = FString("Autonomous Proxy");
		break;
	case ENetRole::ROLE_SimulatedProxy:
		Role = FString("Simulated Proxy");
		break;
	case ENetRole::ROLE_None:
		Role = FString("None");
		break;
	default:
		break;
	}
	// 转换为字符串形式以便传参
	const FString RemoteRoleString = FString::Printf(TEXT("Remote Role: %s"), *Role); // 加上*为了获取C风格的字符串
	// 获取玩家状态需要一定的延迟，因为需等待玩家建立连接之后才能获取
	const APlayerState* PlayerState = InPawn->GetPlayerState();
	FString ClientName = "";
	if (PlayerState)
	{
		ClientName = PlayerState->GetPlayerName();
	}
	
	SetDisplayText(RemoteRoleString, ClientName);
}

void UOverHeadWidget::OnLevelRemovedFromWorld(ULevel* InLevel, UWorld* InWorld)
{
	RemoveFromParent();
	Super::OnLevelRemovedFromWorld(InLevel, InWorld);
}
