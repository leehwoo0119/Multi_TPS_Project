// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayerController.h"
#include "GameIS.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerParents.h"
#include "TestProjectGameMode.h"
#include "SwatCharacter.h"
#include "PlayerWeapon.h"
#include "ParentsPlayerState.h"

AMainPlayerController::AMainPlayerController()
{

}

void AMainPlayerController::BeginPlay()
{
    Super::BeginPlay();

   PlayerCameraManager->ViewPitchMin = -60.0; // Use whatever values you want
   PlayerCameraManager->ViewPitchMax = 45.0;
}

bool AMainPlayerController::Server_SendMessage_Validate(const FString& _sender, const FString& _message)
{
    return true;
}

void AMainPlayerController::Server_SendMessage_Implementation(const FString& _sender, const FString& _message)
{
    ATestProjectGameMode* testProjectGameMode = Cast<ATestProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    check(testProjectGameMode != nullptr);
    testProjectGameMode->SendMessageToEveryone(_sender, _message);
}

bool AMainPlayerController::Server_PossessPlayer_Validate(APawn* _playerParents)
{
    return true;
}

void AMainPlayerController::Server_PossessPlayer_Implementation(APawn* _playerParents)
{
    PossessPlayer(_playerParents);
}

bool AMainPlayerController::Server_SendPlayerScore_Validate()
{
    return true;
}

void AMainPlayerController::Server_SendPlayerScore_Implementation()
{
    ATestProjectGameMode* testProjectGameMode = Cast<ATestProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    check(testProjectGameMode != nullptr);
    testProjectGameMode->RankUpdate(this);
}

bool AMainPlayerController::Client_SetPlayerName_Validate()
{
    return true;
}

void AMainPlayerController::Client_SetPlayerName_Implementation()
{
    UGameIS* gameIS = Cast<UGameIS>(UGameplayStatics::GetGameInstance(GetWorld()));
    check(gameIS != nullptr);
    Server_SetPlayerName(gameIS->GetPlayerName());
}

bool AMainPlayerController::Server_SetPlayerName_Validate(const FString& _val)
{
    return true;
}

void AMainPlayerController::Server_SetPlayerName_Implementation(const FString& _val)
{
    AParentsPlayerState* parentsPlayerState = Cast<AParentsPlayerState>(PlayerState);
    check(parentsPlayerState != nullptr);
    parentsPlayerState->SetPlayerName(_val);
}

void AMainPlayerController::PossessPlayer(APawn* _playerParents)
{
    UGameIS* gameIS = Cast<UGameIS>(UGameplayStatics::GetGameInstance(GetWorld()));
    check(gameIS != nullptr);

    ATestProjectGameMode* testProjectGameMode = Cast<ATestProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    check(testProjectGameMode != nullptr);

    OnPossess(GetWorld()->SpawnActor<APlayerParents>(
        gameIS->GetPlayerParentsByIndex(gameIS->GetPlayerID()),
        testProjectGameMode->GetRespawnTransform()));

    ASwatCharacter* player = Cast<ASwatCharacter>(_playerParents);
    if (player)
    {
        if (player->GetPlayerWeapon())
        {
            player->GetPlayerWeapon()->Destroy();
        }
        if (player->GetUltimateWeapon())
        {
            player->GetUltimateWeapon()->Destroy();
        }
    }
   
    _playerParents->Destroy();
}
