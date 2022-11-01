// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBullet.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "SwatCharacter.h"
#include "SwatCombatComponent.h"

// Sets default values
APlayerBullet::APlayerBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	projectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("projectileMovement"));
	projectileMovement->bRotationFollowsVelocity = true;
	projectileMovement->bSimulationEnabled = true;
	projectileMovement->ProjectileGravityScale = 0.0f;
	projectileMovement->InitialSpeed = 10000.0f;
	projectileMovement->MaxSpeed = 10000.0f;

	bulletDamageCollision = CreateDefaultSubobject<USphereComponent>(TEXT("bulletDamageCollision"));
	bulletDamageCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	bulletDamageCollision->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void APlayerBullet::BeginPlay()
{
	Super::BeginPlay();
}

bool APlayerBullet::HeadShotCheck(FName _hitBone)
{
	return _hitBone == FName("Neck")
		|| _hitBone == FName("Neck1")
		|| _hitBone == FName("Head1")
		|| _hitBone == FName("End")
		|| _hitBone == FName("LeftEye")
		|| _hitBone == FName("RightEye")
		|| _hitBone == FName("Jaw");
}




