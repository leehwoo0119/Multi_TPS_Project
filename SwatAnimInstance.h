// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SwatAnimInstance.generated.h"

class UAnimInstance;
class APawn;
class UAnimMontage;
class ASwatCharacter;

UCLASS()
class TESTPROJECT_API USwatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
protected:
	void NativeInitializeAnimation()override;

	void NativeBeginPlay()override;

private:
	UAnimInstance* aniInstance;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	APawn* pawn;

	ASwatCharacter* mainChar;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* combatMontage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Anims")
	UAnimMontage* reloadMontage;

public:
	UFUNCTION()
	void AnimMontagePlay(UAnimMontage* _animMontage, float _animSpeed, FName montageSectionName);
};
