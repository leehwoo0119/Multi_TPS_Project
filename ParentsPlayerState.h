// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ParentsPlayerState.generated.h"

UCLASS()
class TESTPROJECT_API AParentsPlayerState : public APlayerState
{
	GENERATED_BODY()
		
public:
	AParentsPlayerState();
	
protected:
	// Called when the game starts or when spawned
	void BeginPlay() override;

private:
	int swatMaxHP;
	UPROPERTY(Replicated)
	int swatCurHP;
	
	int maxAmmoCnt;
	UPROPERTY(Replicated)
	int curAmmoCnt;

	float weaponMaxDis;
	float respawnDelay;

	UPROPERTY(Replicated)
	bool bReload;

	UPROPERTY(Replicated)
	bool bDeath;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "weapon", meta = (AllowPrivateAccess = "true"))
	int weaponDamage;

	UPROPERTY(Replicated)
	int killCnt;

	UPROPERTY(Replicated)
	int deathCnt;

	float fireDelay;

	float maxUltimateGauge;

	UPROPERTY(Replicated)
	float curUltimateGauge;

	bool bUltimate;

	int maxUltimateCnt;
	
	int curUltimateCnt;

public:
	FORCEINLINE int GetSwatMaxHP()const { return swatMaxHP; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetSwatCurHP()const { return swatCurHP; }

	FORCEINLINE float GetWeaponMaxDis()const { return weaponMaxDis; }
	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetRespawnDelay()const { return respawnDelay; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetMaxAmmoCnt()const { return maxAmmoCnt; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetCurAmmoCnt()const { return curAmmoCnt; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetMaxUltimateCnt()const { return maxUltimateCnt; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetCurUltimateCnt()const { return curUltimateCnt; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetReload()const { return bReload; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetDeath()const { return bDeath; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetUltimate()const { return bUltimate; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetKillCnt()const { return killCnt; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE int GetDeathCnt()const { return deathCnt; }

	FORCEINLINE float GetFireDelay()const { return fireDelay; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE float GetCurUltimateGauge()const { return curUltimateGauge; }

	UFUNCTION(BlueprintCallable)
	void SetSwatCurHP(float _val);

	UFUNCTION(BlueprintCallable)
	void SetReload(bool _val);

	UFUNCTION(BlueprintCallable)
	void SetDeath(bool _val);

	void SetUltimate(bool _val);

	void SetCurUltimateGauge(float _val, bool _reset= false);

	void PlusKillCnt(bool _reset = false);

	void PlusDeathCnt(bool _reset = false);

	UFUNCTION(BlueprintCallable)
	void ReloadCurAmmoCnt();
	void FireAmmo(bool _bReload = false);
	
	bool CheckAmmoZero();

	bool FireUltimate();
	bool CheckUltimate();

	UFUNCTION(BlueprintCallable)
	void RespawnStateReset();

	UFUNCTION(BlueprintCallable)
	void SetPlayerInGameName(FString _val);

	UFUNCTION(BlueprintImplementableEvent)
	void DamagePostProcess(FVector _damageLoc);
};
