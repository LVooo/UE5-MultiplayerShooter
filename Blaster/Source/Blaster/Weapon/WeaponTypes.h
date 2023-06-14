#pragma once

#define TRACE_LENGTH 80000.f // 射击射线长度

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName="Assault Rifle"),
	EWT_RockerLauncher UMETA(DisplayName="Rocker Launcher"),
	EWT_Pistol UMETA(DisplayName="Pistol"),
	EWT_SubmachineGun UMETA(DisplayName="Submachine Gun"),
	EWT_Shotgun UMETA(DisplayName="Shotgun"),
	EWT_SniperRifle UMETA(DisplayName="Sniper Rifle"),
	EWT_GrenadeLauncher UMETA(DisplayName="Grenade Launcher"),

	EWT_MAX UMETA(DisplayName="DefaultMAX")
};