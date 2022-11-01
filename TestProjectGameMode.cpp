// Copyright Epic Games, Inc. All Rights Reserved.

#include "TestProjectGameMode.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "MainPlayerController.h"
#include "ParentsPlayerState.h"
#include "ParentsPlayerState.h"
#include "GameIS.h"

ATestProjectGameMode::ATestProjectGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/SwatCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

FTransform ATestProjectGameMode::GetRespawnTransform()
{
	int size = respawnTransformBP.Num();

	AActor* respawnTransformActor = UGameplayStatics::GetActorOfClass(GetWorld(), respawnTransformBP[FMath::RandRange(0, size - 1)]);
	check(respawnTransformActor != nullptr);

	return respawnTransformActor->GetTransform();
}

void ATestProjectGameMode::SendMessageToEveryone(const FString& _sender, const FString& _message)
{
	for (auto ctrl : playerCtrlArr)
	{
		ctrl->AddMessage(_sender, _message);
	}
}

void ATestProjectGameMode::SendKillDeathToEveryone(AParentsPlayerState* _damagedPlayer, AParentsPlayerState* _damageCauserPlayer, bool _bHead)
{
	for (auto ctrl : playerCtrlArr)
	{
		ctrl->DrawKillDeathPlayer(_damagedPlayer, _damageCauserPlayer, _bHead);
	}
}

void ATestProjectGameMode::RankUpdate(AMainPlayerController* _ctrl)
{
	TArray<AParentsPlayerState*> playerRankArr;

	for (auto ctrl : playerCtrlArr)
	{
		AParentsPlayerState* playerState = Cast<AParentsPlayerState>(ctrl->PlayerState);
		check(playerState != nullptr);
		playerRankArr.Add(playerState);
	}	
	playerRankArr.Sort([](const AParentsPlayerState& _killCnt1, const AParentsPlayerState& _killCnt2) {
		return  _killCnt1.GetKillCnt() > _killCnt2.GetKillCnt();
		});
	_ctrl->AddPlayerScore(playerRankArr);
}

//void ATestProjectGameMode::GetDeathScore()
//{
//	for (auto ctrl : playerCtrlArr)
//	{
//		AParentsPlayerState* state = Cast<AParentsPlayerState>(ctrl->PlayerState);
//		check(state != nullptr);
//		UE_LOG(LogTemp, Log, TEXT("%d "), state->GetDeathCnt());	
//	}
//}
