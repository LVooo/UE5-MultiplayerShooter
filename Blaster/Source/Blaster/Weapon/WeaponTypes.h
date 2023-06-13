#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName="Assault Rifle"),
	EWT_RockerLauncher UMETA(DisplayName="Rocker Launcher"),
	EWT_Pistol UMETA(DisplayName="Pistol"),
	EWT_SubmachineGun UMETA(DisplayName="Submachine Gun"),

	EWT_MAX UMETA(DisplayName="DefaultMAX")
};