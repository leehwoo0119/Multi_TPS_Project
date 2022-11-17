// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionBox.generated.h"

UCLASS()
class TESTPROJECT_API AExplosionBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExplosionBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(BlueprintReadWrite, Category = "status", meta = (AllowPrivateAccess = "true"), Replicated)
	float boxHP;

public:
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_TakeDamage();
	bool Server_TakeDamage_Validate();
	void Server_TakeDamage_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_TakeDamage(float _hp, bool _bDeath);
	bool Multi_TakeDamage_Validate(float _hp, bool _bDeath);
	void Multi_TakeDamage_Implementation(float _hp, bool _bDeath);

	UFUNCTION(BlueprintCallable)
	void BoxDamage();

	UFUNCTION(BlueprintImplementableEvent)
	void DrawHp(float _hp);

	UFUNCTION(BlueprintImplementableEvent)
	void Explosion();
};
