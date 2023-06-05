// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/HUD/BlasterHUD.h"
#include "Blaster/Weapon/WeaponTypes.h"
#include "Components/ActorComponent.h"
#include "CombatComponent.generated.h"

#define TRACE_LENGTH 80000.f // 射击射线长度

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BLASTER_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCombatComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	// 添加角色友元类，将所有访问权限暴露给该类
	friend class ABlasterCharacter;

	void EquipWeapon(class AWeapon* WeaponToEquip);

	void Reload();

protected:
	virtual void BeginPlay() override;
	void SetAiming(bool bIsAiming);

	UFUNCTION(Server, Reliable)
	void ServerSetAiming(bool bIsAiming); // RPC，作为在服务器上执行SetAiming函数的代理函数

	UFUNCTION()
	void OnRep_EquippedWeapon();

	void FireButtonPressed(bool bPressed);

	void Fire();

	UFUNCTION(Server, Reliable)
	void ServerFire(const FVector_NetQuantize& TraceHitTarget); // 服务器执行

	UFUNCTION(NetMulticast, Reliable)
	void MulticastFire(const FVector_NetQuantize& TraceHitTarget); // 服务器分发到各客户端

	void TraceUnderCrosshairs(FHitResult& TraceHitResult);

	void SetHUDCrosshairs(float DeltaTime);

	UFUNCTION(Server, Reliable)
	void ServerReload();

private:
	UPROPERTY()
	class ABlasterCharacter* Character;
	UPROPERTY()
	class ABlasterPlayerController* Controller;
	UPROPERTY()
	class ABlasterHUD* HUD;

	UPROPERTY(ReplicatedUsing = OnRep_EquippedWeapon)
	AWeapon* EquippedWeapon;

	UPROPERTY(Replicated)
	bool bAiming;

	UPROPERTY(EditAnywhere)
	float BaseWalkSpeed;

	UPROPERTY(EditAnywhere)
	float AimWalkSpeed;

	bool bFireButtonPressed;

	/*
	 * HUD和十字瞄准
	 */

	float CrosshairVelocityFactor;
	float CrosshairInAirFactor;
	float CrosshairAimFactor;
	float CrosshairShootingFactor;
	float CrosshairAimPlayerFactor;

	FVector HitTarget;
	FHitResult HitResult;

	FHUDPackage HUDPackage;

	/*
	 * 瞄准并且放大
	 */

	// 摄像机默认的FOV
	float DefaultFOV;

	UPROPERTY(EditAnywhere, Category=Combat)
	float ZoomedFOV = 30.f;
	
	float CurrentFOV;

	UPROPERTY(EditAnywhere, Category=Combat)
	float ZoomInterpSpeed = 20.f;

	void InterpFOV(float DeltaTime);

	/*
	 * 自动开火
	 */
	FTimerHandle FireTimer;
	bool bCanFire = true;

	void StartFireTimer();
	void FireTimerFinished();

	bool CanFire();

	// 武器总弹夹
	UPROPERTY(ReplicatedUsing=OnRep_CarriedAmmo)
	int32 CarriedAmmo;

	UFUNCTION()
	void OnRep_CarriedAmmo();

	TMap<EWeaponType, int32> CarriedAmmoMap;

	UPROPERTY(EditAnywhere)
	int32 StartingARAmmo = 30;

	void InitializerCarriedAmmo();
	
public:	
		
};
