// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionBox.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AExplosionBox::AExplosionBox():boxHP(100.0f)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void AExplosionBox::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AExplosionBox, boxHP);
}

// Called when the game starts or when spawned
void AExplosionBox::BeginPlay()
{
	Super::BeginPlay();


}

bool AExplosionBox::Server_TakeDamage_Validate()
{
	return true;
}

void AExplosionBox::Server_TakeDamage_Implementation()
{
	boxHP -= FMath::RandRange(10, 20);

	bool bDeath = false;
	if (boxHP <= 0)bDeath = true;
	Multi_TakeDamage(boxHP, bDeath);
}

bool AExplosionBox::Multi_TakeDamage_Validate(float _hp, bool _bDeath)
{
	return true;
}

void AExplosionBox::Multi_TakeDamage_Implementation(float _hp, bool _bDeath)
{
	if (_bDeath)
	{
		Explosion();
	}
	else
	{
		DrawHp(_hp);
	}	
}

void AExplosionBox::BoxDamage()
{
	FlushNetDormancy();
	Server_TakeDamage();
}




