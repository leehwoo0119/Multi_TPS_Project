// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TestProjectGameMode.generated.h"

class AActor;
class AMainPlayerController;
class AParentsPlayerState;

UCLASS(minimalapi)
class ATestProjectGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATestProjectGameMode();

	//virtual void PostLogin(APlayerController* NewPlayer)override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<AActor>> respawnTransformBP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<AMainPlayerController*> playerCtrlArr;

public:
	FTransform GetRespawnTransform();

	void SendMessageToEveryone(const FString& _sender, const FString& _message);

	void SendKillDeathToEveryone(AParentsPlayerState* _damagedPlayer, AParentsPlayerState* _damageCauserPlayer, bool _bHead);

	void RankUpdate(AMainPlayerController* _ctrl);
	
};



