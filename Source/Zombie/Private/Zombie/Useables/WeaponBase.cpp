// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/WeaponBase.h"
#include "Components/SkeletalMeshComponent.h"

AWeaponBase::AWeaponBase()
{
 
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");

	WeaponName = "Default";
	WeaponMaxAmmo = 255;
	MagazineMaxAmmo = 30;
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;

}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBase::Fire()
{
}

void AWeaponBase::Reload()
{
}

TArray<int32> AWeaponBase::GetCurrentAmmo()
{
	return { CurrentMagazineAmmo, CurrentTotalAmmo };
}

