// Fill out your copyright notice in the Description page of Project Settings.


#include "ParentsPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "GameIS.h"

AParentsPlayerState::AParentsPlayerState()
{
	weaponMaxDis = 10000.0f;
	weaponDamage = 20.0f;
	swatMaxHP = 200.0f;
	swatCurHP = swatMaxHP;
	maxAmmoCnt = 25;
	curAmmoCnt = maxAmmoCnt;
	respawnDelay = 6.0f;
	killCnt = 0;
	deathCnt = 0;
	fireDelay = 0.15f;
	maxUltimateGauge = 100.0f;
	curUltimateGauge = 0.0f;

	maxUltimateCnt = 2;
	curUltimateCnt = maxUltimateCnt;
}

void AParentsPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AParentsPlayerState, swatCurHP);
	DOREPLIFETIME(AParentsPlayerState, bDeath);
	DOREPLIFETIME(AParentsPlayerState, curAmmoCnt);
	DOREPLIFETIME(AParentsPlayerState, bReload);
	DOREPLIFETIME(AParentsPlayerState, killCnt);
	DOREPLIFETIME(AParentsPlayerState, deathCnt);
	DOREPLIFETIME(AParentsPlayerState, curUltimateGauge);
}


void AParentsPlayerState::BeginPlay()
{
}

void AParentsPlayerState::SetSwatCurHP(float _val)
{
	swatCurHP = _val;
	if (swatCurHP < 0)
	{
		swatCurHP = 0;
	}
	if (swatCurHP >= swatMaxHP)
	{
		swatCurHP = swatMaxHP;
	}
}

void AParentsPlayerState::SetReload(bool _val)
{
	bReload = _val;
}

void AParentsPlayerState::SetDeath(bool _val)
{
	bDeath = _val;
}

void AParentsPlayerState::SetUltimate(bool _val)
{
	if (_val == true)curUltimateCnt = maxUltimateCnt;
	bUltimate = _val;
}

void AParentsPlayerState::SetCurUltimateGauge(float _val, bool _reset)
{
	if (!_reset && curUltimateGauge >= maxUltimateGauge)
	{
		curUltimateGauge = maxUltimateGauge;
		return;
	}
	curUltimateGauge = _val;
}

void AParentsPlayerState::PlusKillCnt(bool _reset)
{
	if (_reset)
	{
		killCnt = 0;
	}
	else
	{
		killCnt++;
	}
}

void AParentsPlayerState::PlusDeathCnt(bool _reset)
{
	if (_reset)
	{
		deathCnt = 0;
	}
	else
	{
		deathCnt++;
	}
}

void AParentsPlayerState::ReloadCurAmmoCnt()
{
	curAmmoCnt = maxAmmoCnt;
}

void AParentsPlayerState::FireAmmo(bool _bReload)
{
	if (_bReload)
	{
		curAmmoCnt = maxAmmoCnt;
	}
	else
	{
		curAmmoCnt -= 1;
	}
}

bool AParentsPlayerState::FireUltimate()
{
	curUltimateCnt -= 1;
	if (curUltimateCnt == 0)return true;
	else return false;
}

bool AParentsPlayerState::CheckUltimate()
{
	return curUltimateGauge == maxUltimateGauge;
}

bool AParentsPlayerState::CheckAmmoZero()
{
	return curAmmoCnt == 0;
}

void AParentsPlayerState::RespawnStateReset()
{
	curAmmoCnt = maxAmmoCnt;
	swatCurHP = swatMaxHP;
	bDeath = false;
	bReload = false;
	bUltimate = false;
	curUltimateGauge = 0;
}

void AParentsPlayerState::SetPlayerInGameName(FString _val)
{
	SetPlayerName(_val);
}

