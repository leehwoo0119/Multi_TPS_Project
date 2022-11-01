// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerBullet.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class AParentsPlayerState;
class AMainPlayerController;

UCLASS()
class TESTPROJECT_API APlayerBullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlayerBullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* projectileMovement;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Bullet", meta = (AllowPrivateAccess = "true"))
	USphereComponent* bulletDamageCollision;

public:
	UPROPERTY(BlueprintReadWrite, Category = "Bullet", meta = (ExposeOnSpawn = "true"))
	AParentsPlayerState* damageCauserState;

	UPROPERTY(BlueprintReadWrite, Category = "Bullet", meta = (ExposeOnSpawn = "true"))
	FVector damageCauserLoc;

public:
	UFUNCTION(BlueprintCallable)
	bool HeadShotCheck(FName _hitBone);
};
