#pragma once

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	EWT_AssaultRifle UMETA(DisplayName="Assault Rifle"),
	EWT_RockerLauncher UMETA(DisplayName="Rocker Launcher"),

	EWT_MAX UMETA(DisplayName="DefaultMAX")
};