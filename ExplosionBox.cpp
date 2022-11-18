// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionBox.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AExplosionBox::AExplosionBox() :boxHealth(100.0f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AExplosionBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExplosionBox, boxHealth);
}

// Called when the game starts or when spawned
void AExplosionBox::BeginPlay()
{
	Super::BeginPlay();


}

void AExplosionBox::OnRep_HealthUpdate()
{
	DrawHp();
}

void AExplosionBox::BoxDamage()
{
	FlushNetDormancy();
	boxHealth -= FMath::RandRange(10, 20);
}

bool AExplosionBox::Server_Destroy_Validate()
{
	return boxHealth <= 0;
}

void AExplosionBox::Server_Destroy_Implementation()
{
	Multi_Destroy();
	Destroy();
}

bool AExplosionBox::Multi_Destroy_Validate()
{
	return true;
}

void AExplosionBox::Multi_Destroy_Implementation()
{
	Explosion();
}
