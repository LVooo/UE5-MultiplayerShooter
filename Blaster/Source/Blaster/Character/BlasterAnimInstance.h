// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
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
};
