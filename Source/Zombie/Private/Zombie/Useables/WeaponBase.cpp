// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie/Useables/WeaponBase.h"
#include "Player/ZombiePlayerCharacter.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "Animation/AnimInstance.h"
#include "Net/UnrealNetwork.h"

AWeaponBase::AWeaponBase()
{
 
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = WeaponMesh;
	SetReplicates(true);

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

void AWeaponBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// to prevent cheating so someone can't have unlimited ammo
	// all other players don't need to know about someone elses ammo so don't need to replicate/broadcast to every player
	DOREPLIFETIME_CONDITION(AWeaponBase, CurrentTotalAmmo, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AWeaponBase, CurrentMagazineAmmo, COND_OwnerOnly);
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

TArray<FHitResult> AWeaponBase::PerformLineTrace(FVector MuzzleLocation, FRotator MuzzleRotation)
{
	FVector End = MuzzleLocation + MuzzleRotation.Vector() * 5000.0f;
	TArray<FHitResult> HitResults;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	if (GetOwner())
	{
		CollisionParams.AddIgnoredActor(GetOwner());
	}
	FCollisionResponseParams CollisionResponse;

	GetWorld()->LineTraceMultiByChannel(OUT HitResults, MuzzleLocation, End, ECollisionChannel::ECC_GameTraceChannel2, CollisionParams, CollisionResponse);
	DrawDebugLine(GetWorld(), MuzzleLocation, End, FColor::Red, false, 2.0f, 0, 3.0f);
	return HitResults;
}

bool AWeaponBase::Server_Fire_Validate(const TArray<FHitResult>& HitResults)
{
	return true;
}

void AWeaponBase::Server_Fire_Implementation(const TArray<FHitResult>& HitResults)
{
	if (CurrentMagazineAmmo > 0)
		CurrentMagazineAmmo--;
	UE_LOG(LogTemp, Warning, TEXT("ServerAmmo: %d"), CurrentMagazineAmmo);
}

bool AWeaponBase::Multi_Fire_Validate(const FHitResult& HitResult)
{
	return true;
}

void AWeaponBase::Multi_Fire_Implementation(const FHitResult& HitResult)
{
}


bool AWeaponBase::Fire(AZombiePlayerCharacter* ShootingPlayer)
{
	if (CurrentMagazineAmmo > 0)
		CurrentMagazineAmmo--;
	UE_LOG(LogTemp, Warning, TEXT("ClientAmmo: %d"), CurrentMagazineAmmo);
	return true;
}

bool AWeaponBase::Server_Reload_Validate()
{
	return true;
}

void AWeaponBase::Server_Reload_Implementation()
{
	Reload();  // Reload() now called as the server, which will call multi_reload in that function so no need to call it here
}

bool AWeaponBase::Multi_Reload_Validate()
{
	return true;
}

// runs on all clients including person who called it
void AWeaponBase::Multi_Reload_Implementation()
{
	if (AZombiePlayerCharacter* Pawn = Cast<AZombiePlayerCharacter>(GetOwner()))
	{
		// if you were the client who reloaded don't show animation twice b/c already showed it in Reload()
		if (!Pawn->IsLocallyControlled() && ReloadAnimation)
		{
			// same reload logic in ZombiePlayerCharacter but this is needed so other clients and server can see that player's reload
			if (UAnimInstance* AnimInstance = Pawn->GetMesh1P()->GetAnimInstance())
			{
				if (FPSArmsFireMontage)
				{
					AnimInstance->Montage_Play(FPSArmsFireMontage);
					AnimInstance->Montage_JumpToSection(FName("Reload"), FPSArmsFireMontage);
				}
			}
			WeaponMesh->PlayAnimation(ReloadAnimation, false);
		}
	}
}

// client reloads
bool AWeaponBase::Reload()
{
	if (CurrentTotalAmmo > 0 && CurrentMagazineAmmo != MagazineMaxAmmo)
	{
		if (APawn* Pawn = Cast<APawn>(GetOwner()))
		{
			// if this client was the one who reloaded
			if (Pawn->IsLocallyControlled() && ReloadAnimation)
			{
				WeaponMesh->PlayAnimation(ReloadAnimation, false);
			}
		}
		//UE_LOG(LogTemp, Warning, TEXT("CurrentTotalAmmo Before: %d"), CurrentTotalAmmo);
		int Difference = MagazineMaxAmmo - CurrentMagazineAmmo;
		if (CurrentTotalAmmo - Difference >= 0)
		{
			CurrentTotalAmmo -= Difference;
			CurrentMagazineAmmo = MagazineMaxAmmo;
		}
		else
		{
			CurrentMagazineAmmo += CurrentTotalAmmo;
			CurrentTotalAmmo = 0;
		}
		//UE_LOG(LogTemp, Warning, TEXT("CurrentTotalAmmo After: %d"), CurrentTotalAmmo);

		if (GetWorld()->IsServer())
		{
			Multi_Reload(); // send message to all the clients that someone reloaded
		}
		else
		{
			Server_Reload(); // send message to server that this client reloaded
		}
		return true;
	}
	else
	{
		return false;
	}
}

FWeaponDamage AWeaponBase::GetWeaponDamage()
{
	return WeaponDamage;
}

TArray<int32> AWeaponBase::GetCurrentAmmo()
{
	return { CurrentMagazineAmmo, CurrentTotalAmmo };
}

UAnimMontage* AWeaponBase::GetFireAnimMontage()
{
	return FPSArmsFireMontage;
}

TEnumAsByte<EWeaponID> AWeaponBase::GetWeaponID()
{
	return WeaponID;
}
