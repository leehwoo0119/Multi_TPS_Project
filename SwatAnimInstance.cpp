// Fill out your copyright notice in the Description page of Project Settings.


#include "SwatAnimInstance.h"
#include "SwatCharacter.h"

void USwatAnimInstance::NativeInitializeAnimation()
{
	pawn = TryGetPawnOwner();

	if (pawn)
		mainChar = Cast<ASwatCharacter>(pawn);

	if (mainChar)
		aniInstance = Cast<UAnimInstance>(mainChar->GetMesh()->GetAnimInstance());

}

void USwatAnimInstance::NativeBeginPlay()
{
	check(pawn != nullptr);
	check(mainChar != nullptr);
	check(aniInstance != nullptr);
	check(combatMontage != nullptr);
	check(reloadMontage != nullptr);	
}

void USwatAnimInstance::AnimMontagePlay(UAnimMontage* _animMontage, float _animSpeed, FName montageSectionName)
{
	mainChar->GetMesh()->GetAnimInstance()->Montage_Play(
		_animMontage,
		_animSpeed
	);
	mainChar->GetMesh()->GetAnimInstance()->Montage_JumpToSection(
		montageSectionName,
		_animMontage
	);
}