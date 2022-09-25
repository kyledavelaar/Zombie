// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/Weapon1911.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

AWeapon1911::AWeapon1911()
{
	WeaponName = "1911";
	BaseDamage = 100;
	WeaponMaxAmmo = 98;
	MagazineMaxAmmo = 7;
	CurrentTotalAmmo = WeaponMaxAmmo;
	CurrentMagazineAmmo = MagazineMaxAmmo;
}

void AWeapon1911::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("SHOOTING 1911"));
	// line trace setup to hit zombie
	// // used when this logic was in character component
	//FVector Start = GetFirstPersonCameraComponent()->GetComponentLocation();
	//FVector Rot = GetFirstPersonCameraComponent()->GetComponentRotation().Vector();
	//FVector Start = WeaponMesh->GetBoneLocation(FName("Grip_Bone"));
	//FVector Rot = WeaponMesh->GetBoneQuaternion(FName("Grip_Bone")).Vector();
	FVector Start = WeaponMesh->GetSocketLocation(FName("MuzzleFlash"));
	FVector Rot = WeaponMesh->GetSocketRotation(FName("MuzzleFlash")).Vector();
	FVector End = Start + Rot * 2000.0f;
	TArray<FHitResult> HitResults;
	//FCollisionObjectQueryParams CollisionQuery;
	FCollisionQueryParams CollisionParams;
	FCollisionResponseParams CollisionResponse;
	CollisionParams.AddIgnoredActor(this);

	// can only hit one thing, instead we'll use LineTraceMultiByChannel so able to hit multiple things
	// GetWorld()->LineTraceSingleByObjectType(OUT HitResult, Start, End, CollisionQuery, CollisionParams)

	// set visibility to overlap on mesh collision so can shoot through objects
	// add channel in project settings collision section, then go to DefaultEngine.ini config to find what name is: ECC_GameTraceChannel2
	if (GetWorld()->LineTraceMultiByChannel(OUT HitResults, Start, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse))
	{
		for (FHitResult& HitResult : HitResults)
		{
			if (AActor* HitActor = HitResult.GetActor())
			{
				UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetName());
			}
		}


	/*	if (AZombieBase* Zombie = Cast<AZombieBase>(HitResult.GetActor()))
		{
			UE_LOG(LogTemp, Warning, TEXT("ZOMBIE HIT, %s"), *Zombie->GetName());
			Zombie->Hit(this);
		}*/
	}
	DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 2.0f, 0, 3.0f);

}

void AWeapon1911::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("RELOADING 1911"));
}
