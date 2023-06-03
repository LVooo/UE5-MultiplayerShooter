// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Blaster/BlasterTypes/TurningInPlace.h"
#include "BlasterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API UBlasterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override; // 类似EventBegin
	virtual void NativeUpdateAnimation(float DeltaTime) override; // 类似Tick
private:
	UPROPERTY(BlueprintReadOnly, Category=Character, meta=(AllowPrivateAccess="true")) // 允许私有成员在蓝图中访问修改
	class ABlasterCharacter* BlasterCharacter;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	bool bIsAccelerating;
	
	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	bool bWeaponEquipped;

	class AWeapon* EquippedWeapon;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	bool bIsCrouched;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	float YawOffset;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	float Lean;

	FRotator CharacterRotationLastFrame;
	FRotator CharacterRotation;
	FRotator DeltaRotation;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	float AO_Yaw;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	float Ao_Pitch;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	FTransform LeftHandTransform; // 固定左手连接武器的位置

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	ETurningInPlace TurningInPlace;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	FRotator RightHandRotation;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	bool bLocallyControlled;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	bool bRotateRootBone;

	UPROPERTY(BlueprintReadOnly, Category=Movement, meta=(AllowPrivateAccess="true"))
	bool bElimmed;
};
