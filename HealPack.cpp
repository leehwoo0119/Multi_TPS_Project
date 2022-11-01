// Fill out your copyright notice in the Description page of Project Settings.


#include "HealPack.h"
#include "Components/SphereComponent.h"
#include "SwatCharacter.h"
#include "ParentsPlayerState.h"

// Sets default values
AHealPack::AHealPack()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	healCollision = CreateDefaultSubobject<USphereComponent>(TEXT("healCollision"));

	healVal = 100;
}

// Called when the game starts or when spawned
void AHealPack::BeginPlay()
{
	Super::BeginPlay();
	
	healCollision->OnComponentBeginOverlap.AddDynamic(this, &AHealPack::HealCollisionOnOverlapBegin);
}

void AHealPack::HealCollisionOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		ASwatCharacter* player = Cast<ASwatCharacter>(OtherActor);

		if (player)
		{
			AParentsPlayerState* playerState = Cast<AParentsPlayerState>(player->GetPlayerState());

			check(playerState != nullptr);

			playerState->SetSwatCurHP(playerState->GetSwatCurHP() + healVal);

			Respawn();
			Destroy();
		}
	}
}


