// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "ProjectileWeapon.generated.h"

/**
 * 
 */
UCLASS()
class BLASTER_API AProjectileWeapon : public AWeapon
{
	GENERATED_BODY()
public:
	virtual void Fire(const FVector& HitTarget) override;
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ProjectileClass; //  用于在运行时指定生成的子弹类型，这样可以方便地切换不同的子弹类型而不需要修改代码

	UPROPERTY(EditAnywhere)
	TSubclassOf<class AProjectile> ServerSideRewindProjectileClass; // 用于本地生成的子弹，non-replicated

};
