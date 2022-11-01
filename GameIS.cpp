// Fill out your copyright notice in the Description page of Project Settings.


#include "GameIS.h"

UGameIS::UGameIS() :playerID(0), playerName("None")
{
}

TSubclassOf<APlayerParents> UGameIS::GetPlayerParentsByIndex(int _index)
{
	return playerParentsBP[_index];
}

void UGameIS::SetPlayerID(int _val)
{
	playerID = _val;
}

void UGameIS::SetPlayerName(FString _val)
{
	playerName = _val;
}
