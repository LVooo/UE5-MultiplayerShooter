// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "GameFramework/Character.h"
#include "BlasterCharacter.generated.h"

UCLASS(config=Game)
class ABlasterCharacter : public ACharacter
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

protected:
	virtual void BeginPlay() override;
	
	void MoveForward(float Value);
	void MoveRight(float Value);
	void Turn(float Rate);
	void LookUp(float Rate);
	void EquipButtonPressed();
	void CrouchButtonPressed();
	void AimButtonPressed();
	void AimButtonReleased();
	void AimOffset(float DeltaTime); // 站立状态时的瞄准
	virtual void Jump() override;
	void FireButtonPressed();
	void FireButtonReleased();

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

	UPROPERTY(VisibleAnywhere)
	class UCombatComponent* Combat;
	
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

	UPROPERTY(EditAnywhere, Category=Combat)
	class UAnimMontage* FireWeaponMontage;
	
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
};

