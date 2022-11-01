// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerParents.h"
#include "SwatCharacter.generated.h"

class USwatCombatComponent;
class USwatAnimInstance;
class AParentsPlayerState;
class APlayerWeapon;
class APostProcessVolume;

UCLASS()
class TESTPROJECT_API ASwatCharacter : public APlayerParents
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASwatCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = "LookUpAxis", Replicated)
	float lookUpAxis;

private:
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	USwatCombatComponent* swatCombatComponent;
	USwatAnimInstance* animInstance;

	UPROPERTY(Replicated)
	APlayerWeapon* swatWeapon;

	UPROPERTY(Replicated)
	APlayerWeapon* ultimateWeapon;

	UPROPERTY(EditAnyWhere, Category = "Weapon")
	TSubclassOf<APlayerWeapon> swatWeaponBP;

	UPROPERTY(EditAnyWhere, Category = "Weapon")
	TSubclassOf<APlayerWeapon> ultimateWeaponBP;

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	APostProcessVolume* damageProcessVolume;

	FTimerHandle fireWaitHandle;
	FTimerHandle isFireHandle;

protected:
	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void OnFire();

	void UltimateFiring();

	void Firing();

	void FireEnd();
	
	void Reload();

	void Chatting();

	void ScoreList();

	void ScoreListEnd();

	void Ultimate();
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LookUpRate(float _rate);
	bool Server_LookUpRate_Validate(float _rate);
	void Server_LookUpRate_Implementation(float _rate);

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE USwatCombatComponent* GetSwatCombatComponent()const { return swatCombatComponent; }

	FORCEINLINE USwatAnimInstance* GetAnimInstance()const { return animInstance; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE APlayerWeapon* GetPlayerWeapon()const { return swatWeapon; }

	UFUNCTION(BlueprintCallable)
	FORCEINLINE APlayerWeapon* GetUltimateWeapon()const { return ultimateWeapon; }

	UFUNCTION(BlueprintCallable)
	void SetSwatCombatComponent(USwatCombatComponent* _comp);

	UFUNCTION(BlueprintCallable)
	void SpawnWeapon();

	UFUNCTION(BlueprintCallable)
	void SpawnUltimateWeapon();
};
