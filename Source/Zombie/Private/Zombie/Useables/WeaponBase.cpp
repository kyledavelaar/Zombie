// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/WeaponBase.h"
#include "Player/ZombiePlayerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Animation/AnimInstance.h"

AWeaponBase::AWeaponBase()
{
 
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = WeaponMesh;
	SetReplicates(true);

	WeaponName = "Default";
	BaseDamage = 100;
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

TArray<FHitResult> AWeaponBase::PerformLineTrace(AZombiePlayerCharacter* ShootingPlayer)
{
	// // used when this logic was in character component
	//FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	//FVector Rot = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
	//FVector Start = WeaponMesh->GetBoneLocation(FName("Grip_Bone"));
	//FVector Rot = WeaponMesh->GetBoneQuaternion(FName("Grip_Bone")).Vector();
	FVector Start = WeaponMesh->GetSocketLocation(FName("MuzzleFlash"));
	FVector Rot = WeaponMesh->GetSocketRotation(FName("MuzzleFlash")).Vector();
	FVector End = Start + Rot * 5000.0f;
	TArray<FHitResult> HitResults;
	//FCollisionObjectQueryParams CollisionQuery;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(ShootingPlayer);
	FCollisionResponseParams CollisionResponse;

	// can only hit one thing, instead we'll use LineTraceMultiByChannel so able to hit multiple things
	// GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams)

	// set visibility to overlap on mesh collision so can shoot through objects
	// add channel in project settings collision section, then go to DefaultEngine.ini config to find what name is: ECC_GameTraceChannel2
	GetWorld()->LineTraceMultiByChannel(OUT HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse);
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);
	return HitResults;
}

TArray<FHitResult> AWeaponBase::Fire(AZombiePlayerCharacter* ShootingPlayer)
{
	return TArray<FHitResult>();
}

void AWeaponBase::Reload()
{
}

TArray<int32> AWeaponBase::GetCurrentAmmo()
{
	return { CurrentMagazineAmmo, CurrentTotalAmmo };
}

UAnimMontage* AWeaponBase::GetFireAnimMontage()
{
	return FPSArmsFireMontage;
}

