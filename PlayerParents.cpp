// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerParents.h"

// Sets default values
APlayerParents::APlayerParents()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerParents::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerParents::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerParents::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

