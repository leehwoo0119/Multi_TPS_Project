// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameIS.generated.h"

class APlayerParents;
class AParentsPlayerState;

UCLASS()
class TESTPROJECT_API UGameIS : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UGameIS();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APlayerParents>> playerParentsBP; 

	int playerID;

	FString playerName;

public:
	UFUNCTION(BlueprintCallable)
	TSubclassOf<APlayerParents> GetPlayerParentsByIndex(int _index);

	UFUNCTION(BlueprintCallable)
	void SetPlayerID(int _val);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetPlayerID()const { return playerID; }

	UFUNCTION(BlueprintCallable)
	void SetPlayerName(FString _val);

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FString GetPlayerName()const { return playerName; }

};
