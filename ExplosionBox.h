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
	UPROPERTY(ReplicatedUsing = OnRep_HealthUpdate, BlueprintReadWrite, Category = "status", meta = (AllowPrivateAccess = "true"))
	float boxHealth;

public:
	UFUNCTION()
	void OnRep_HealthUpdate();

	UFUNCTION(BlueprintCallable)
	void BoxDamage();

	UFUNCTION(BlueprintImplementableEvent)
	void DrawHp();

	UFUNCTION(BlueprintImplementableEvent)
	void Explosion();

	UFUNCTION(Server, Unreliable, WithValidation, BlueprintCallable)
	void Server_Destroy();
	bool Server_Destroy_Validate();
	void Server_Destroy_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	void Multi_Destroy();
	bool Multi_Destroy_Validate();
	void Multi_Destroy_Implementation();
};
