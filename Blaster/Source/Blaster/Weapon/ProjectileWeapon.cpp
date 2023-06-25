
// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileWeapon.h"

#include "Projectile.h"
#include "Engine/SkeletalMeshSocket.h"

void AProjectileWeapon::Fire(const FVector& HitTarget)
{
	Super::Fire(HitTarget);
	
	APawn* InstigatorPawn = Cast<APawn>(GetOwner());
	const USkeletalMeshSocket* MuzzleFlashSocket = GetWeaponMesh()->GetSocketByName(FName("MuzzleFlash"));
	UWorld* World = GetWorld();
	if (MuzzleFlashSocket && World)
	{
		FTransform SocketTransform = MuzzleFlashSocket->GetSocketTransform(GetWeaponMesh());
		FVector ToTarget = HitTarget - SocketTransform.GetLocation();
		FRotator TargetRotation = ToTarget.Rotation();
		FActorSpawnParameters SpawnParameters;
		// 武器
		SpawnParameters.Owner = GetOwner();
		// 射击者
		SpawnParameters.Instigator = InstigatorPawn;

		AProjectile* SpawnedProjectile = nullptr;
		if (bUseServerSideRewind)
		{
			if (InstigatorPawn->HasAuthority()) // 服务端
			{
				// 服务器本地控制，使用replicated projectile, 不使用 ssr (服务器倒带算法)
				if (InstigatorPawn->IsLocallyControlled())
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(),TargetRotation,SpawnParameters);
					SpawnedProjectile->bUseServerSideRewind = false;
					SpawnedProjectile->Damage = Damage;
				}
				else // 服务器，不是本地控制，发射non-replicated projectile, 使用 ssr
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(),TargetRotation,SpawnParameters);
					SpawnedProjectile->bUseServerSideRewind = false;
				}
			}
			else // 客户端，使用ssr
			{
				if (InstigatorPawn->IsLocallyControlled()) // 本地控制的客户端，产生non-replicated的子弹，使用ssr
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(),TargetRotation,SpawnParameters);
					SpawnedProjectile->bUseServerSideRewind = false;
					SpawnedProjectile->TraceStart = SocketTransform.GetLocation();
					SpawnedProjectile->InitialVelocity =  SpawnedProjectile->GetActorForwardVector() * SpawnedProjectile->InitialSpeed;
					SpawnedProjectile->Damage = Damage;
				}
				else // 其他客户端，产生non-replicated的子弹，不使用ssr
				{
					SpawnedProjectile = World->SpawnActor<AProjectile>(ServerSideRewindProjectileClass, SocketTransform.GetLocation(),TargetRotation,SpawnParameters);
					SpawnedProjectile->bUseServerSideRewind = false;
				}
			}
		}
		else // 不使用ssr的武器
		{
			if (InstigatorPawn->HasAuthority())
			{
				SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileClass, SocketTransform.GetLocation(),TargetRotation,SpawnParameters);
				SpawnedProjectile->bUseServerSideRewind = false;
				SpawnedProjectile->Damage = Damage;
			}
		}
	}
}
