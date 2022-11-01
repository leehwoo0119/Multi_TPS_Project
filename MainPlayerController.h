// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

UCLASS()
class TESTPROJECT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AMainPlayerController();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void ShowChat(bool _bFofused);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddMessage(const FString& _sender, const FString& _message);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void AddPlayerScore(const TArray<AParentsPlayerState*>& playerRankArr);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DrawEndPlayerScore();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DrawKillDeathPlayer(AParentsPlayerState* _damagedPlayer, AParentsPlayerState* _damageCauserPlayer, bool _bHead);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_SendMessage(const FString& _sender, const FString& _message);
	bool Server_SendMessage_Validate(const FString& _sender, const FString& _message);
	void Server_SendMessage_Implementation(const FString& _sender, const FString& _message);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_PossessPlayer(APawn* _playerParents);
	bool Server_PossessPlayer_Validate(APawn* _playerParents);
	void Server_PossessPlayer_Implementation(APawn* _playerParents);

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_SendPlayerScore();
	bool Server_SendPlayerScore_Validate();
	void Server_SendPlayerScore_Implementation();

	UFUNCTION(Client, Reliable, WithValidation, BlueprintCallable)
	void Client_SetPlayerName();
	bool Client_SetPlayerName_Validate();
	void Client_SetPlayerName_Implementation();

	UFUNCTION(Server, Reliable, WithValidation, BlueprintCallable)
	void Server_SetPlayerName(const FString& _val);
	bool Server_SetPlayerName_Validate(const FString& _val);
	void Server_SetPlayerName_Implementation(const FString& _val);

	UFUNCTION(BlueprintCallable)
	void PossessPlayer(APawn* _playerParents);

};
