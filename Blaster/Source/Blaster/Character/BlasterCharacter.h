// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/BlasterTypes/CombatState.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "Blaster/Interfaces/InteractWithCrosshairsInterface.h"
#include "Blaster/Weapon/Weapon.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

UCLASS(config=Game)
class ABlasterCharacter : public ACharacter, public IInteractWithCrosshairsInterface
{
	GENERATED_BODY()
public:
	ABlasterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// 注册可复制的变量，网络同步系统自动调用
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	// 构造combat component，将其中的character实例化
	virtual void PostInitializeComponents() override;
	void PlayFireMontage(bool bAiming);
	void PlayReloadMontage();
	void PlayElimMontage();
	void PlayThrowGrenadeMontage();

	// 为了在simulated客户端禁止rootbone
	virtual void OnRep_ReplicatedMovement() override;

	void Elim();
	UFUNCTION(NetMulticast, Reliable)
	void MulticastElim();
	virtual void Destroyed() override;

	UPROPERTY(Replicated)
	bool bDisableGameplay = false;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowSniperScopeWidget(bool bShowScope);

	void UpdateHUDHealth();
	void UpdateHUDShield();
	void UpdateHUDAmmo();

	void SpawnDefaultWeapon();

protected:
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Value);
	void LookUp(float Value);
	
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void ReloadButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AimOffset(float DeltaTime); // 站立状态时的瞄准
	void CalculateAO_Pitch();
	void SimProxiesTurn();
	virtual void Jump() override;
	void FireButtonPressed();
	void FireButtonReleased();
	void PlayHitReactMontage();
	void GrenadeButtonPressed();
	void DropOrDestroyWeapon(AWeapon* Weapon);
	void DropOrDestroyWeapons();

	UFUNCTION()
	void ReceiveDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, class AController* InsigatorController, AActor* DamageCauser);
	// 轮询初始化PlayerState类和得分以及被击杀数的HUD
	void PollInit();
	void RotateInPlace(float DeltaTime);

private:
	UPROPERTY(VisibleAnywhere, Category=Camera) // 需要在蓝图中使用
	class USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, Category=Camera)
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UWidgetComponent* OverheadWidget;

	UPROPERTY(ReplicatedUsing = OnRep_OverlappingWeapon) // 委托函数，设置该属性可在服务器复制到的多个客户端之间同步，但不会在服务端上自己调用
	class AWeapon* OverlappingWeapon;

	UFUNCTION()
	void OnRep_OverlappingWeapon(AWeapon* LastWeapon);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess="true"))
	class UCombatComponent* Combat;

	UPROPERTY(VisibleAnywhere)
	class UBuffComponent* Buff;
	
	UFUNCTION(Server, Reliable)
	void ServerEquipButtonPressed(); // 服务器调用，处理客户端上装备武器的事件

	// 瞄准偏移
	float AO_Yaw;
	float InterpAO_Yaw;
	float AO_Pitch;
	FRotator StartingAimRotation;

	// 旋转偏移枚举
	ETurningInPlace TurningInPlace;
	void TurnInPlace(float DeltaTime);

	/*
	 * 动画蒙太奇
	 */
	
	UPROPERTY(EditAnywhere, Category=Combat)
	class UAnimMontage* FireWeaponMontage;

	UPROPERTY(EditAnywhere, Category=Combat)
	class UAnimMontage* ReloadMontage;

	UPROPERTY(EditAnywhere, Category=Combat)
	class UAnimMontage* HitReactMontage;

	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* ElimMontage;

	UPROPERTY(EditAnywhere, Category=Combat)
	UAnimMontage* ThrowGrenadeMontage;

	void HideCameraIfCharacterClose();

	UPROPERTY(EditAnywhere)
	float CameraThreshold = 200.f;

	bool bRotateRootBone;
	float TurnThreshold = 2.0f;
	FRotator ProxyRotationLastFrame;
	FRotator ProxyRotation;
	float ProxyYaw;
	float TimeSinceLastMovementReplication;
	float CalculateSpeed();

	/*
	 * 玩家生命值
	 */
	UPROPERTY(EditAnywhere, Category="Player Stats")
	float MaxHealth = 100.f;

	UPROPERTY(ReplicatedUsing=OnRep_Health, VisibleAnywhere, Category="Player Stats")
	float Health = 100.f;

	UFUNCTION()
	void OnRep_Health(float LastHealth);

	/*
	 * 玩家护盾值
	 */
	UPROPERTY(EditAnywhere, Category="Player Stats")
	float MaxShield = 100.f;

	UPROPERTY(ReplicatedUsing=OnRep_Shield, EditAnywhere, Category="Player Stats")
	float Shield = 0.f;

	UFUNCTION()
	void OnRep_Shield(float LastShield);

	UPROPERTY()
	class ABlasterPlayerController* BlasterPlayerController;

	bool bElimmed = false;

	FTimerHandle ElimTimer;

	UPROPERTY(EditDefaultsOnly)
	float ElimDelay = 3.f;

	void ElimTimerFinished();

	/*
	 * 溶解效果
	 */
	UPROPERTY(VisibleAnywhere)
	UTimelineComponent* DissolveTimeLine;
	FOnTimelineFloat DissolveTrack;

	UPROPERTY(EditAnywhere)
	UCurveFloat* DissolveCurve;

	UFUNCTION()
	void UpdateDissolveMaterial(float DissolveValue);
	void StartDissolve();

	// 动态修改材质
	UPROPERTY(VisibleAnywhere, Category=Elim)
	UMaterialInstanceDynamic* DynamicDissolveMaterialInstance;

	// 静态材质
	UPROPERTY(EditAnywhere, Category=Elim)
	UMaterialInstance* DissolveMaterialInstance;

	/*
	 * 死亡飞船
	 */
	UPROPERTY(EditAnywhere)
	UParticleSystem* ElimBotEffect;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* ElimBotComponent;

	UPROPERTY(EditAnywhere)
	class USoundCue* ElimBotSound;

	class ABlasterPlayerState* BlasterPlayerState;

	/*
	 * 手榴弹
	 */
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* AttachedGrenade;

	/*
	 * 默认携带武器
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<AWeapon> DefaultWeaponClass;

public:
	// 为OverlappingWeapon赋值
	void SetOverlappingWeapon(AWeapon* Weapon);
	// 判断是否已经装备武器
	bool IsWeaponEquipped();
	// 判断是否瞄准
	bool IsAiming();
	FORCEINLINE float GetAO_Yaw() const { return AO_Yaw; }
	FORCEINLINE float GetAO_Pitch() const { return AO_Pitch; }
	AWeapon* GetEquippedWeapon();
	FORCEINLINE ETurningInPlace GetTurningInPlace() const { return TurningInPlace; }
	FVector GetHitTarget() const;
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool ShouldRotateRootBone() const { return bRotateRootBone; }
	FORCEINLINE bool isElimed() const { return bElimmed; }
	FORCEINLINE float GetHealth() const { return Health; }
	FORCEINLINE void SetHealth(float Amount) { Health = Amount; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetShield() const { return Shield; }
	FORCEINLINE void SetShield(float Amount) { Shield = Amount; }
	FORCEINLINE float GetMaxShield() const { return MaxShield; }
	ECombatState GetCombatState() const;
	FORCEINLINE UCombatComponent* GetCombat() const { return Combat; }
	FORCEINLINE bool GetDisableGameplay() const { return bDisableGameplay; }
	FORCEINLINE UAnimMontage* GetReloadMontage() const { return ReloadMontage; }
	FORCEINLINE UStaticMeshComponent* GetAttachedGrenade() const { return AttachedGrenade; }
	FORCEINLINE UBuffComponent* GetBuff() const { return Buff; }
	bool IsLocallyReloading();
};

