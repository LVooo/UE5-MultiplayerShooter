// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/Weapon/WeaponTypes.h"
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
	virtual void OnPossess(APawn* InPawn) override;
	void SetHUDHealth(float Health, float MaxHealth);
	void SetHUDScore(float Score);
	void SetHUDDefeats(int32 Defeats);
	void SetHUDElimatedText(bool bHasElimated);
	void SetHUDWeaponAmmo(int32 Ammo);
	void SetHUDCarriedAmmo(int32 Ammo);
	void SetHUDWeaponType(EWeaponType WeaponType);
	void SetHUDMatchCountdown(float CountdownTime);
	virtual void Tick(float DeltaTime) override;

	virtual float GetServerTime(); // 获得客户端与服务器同步的时间
	virtual void ReceivedPlayer() override;
	
protected:
	virtual void BeginPlay() override;

	void SetHUDTime();

	/*
	 * 同步客户端与服务器上的时间
	 */

	// RPC 获取当前服务端上的时间，参数传递为发送请求时客户端上的时间
	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	// RPC 回传当前服务端的时间到客户端上
	UFUNCTION(Client, Reliable)
	void ClientReportServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);

	float ClientServerDelta = 0.f; // 客户端和服务端时间上的差异

	UPROPERTY(EditAnywhere, Category=Time)
	float TimeSyncFrequency = 5.f;

	float TimeSyncRunningTime = 0.f;
	void CheckTimeSync(float DeltaTime);

private:
	UPROPERTY()
	class ABlasterHUD* BlasterHUD;

	float MatchTime = 120.f;
	uint32 CountdownInt = 0;
};
