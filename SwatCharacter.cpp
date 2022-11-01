// Fill out your copyright notice in the Description page of Project Settings.


#include "SwatCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/KismetMathLibrary.h"
#include "SwatCombatComponent.h"
#include "SwatAnimInstance.h"
#include "PlayerWeapon.h"
#include "MainPlayerController.h"
#include "ParentsPlayerState.h"

// Sets default values
ASwatCharacter::ASwatCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 60.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 150.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

}

// Called when the game starts or when spawned
void ASwatCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	animInstance = Cast<USwatAnimInstance>(GetMesh()->GetAnimInstance());
	check(animInstance != nullptr);
}

void ASwatCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(ASwatCharacter, lookUpAxis);
	DOREPLIFETIME(ASwatCharacter, swatWeapon);
	DOREPLIFETIME(ASwatCharacter, ultimateWeapon);
}

// Called to bind functionality to input
void ASwatCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ASwatCharacter::OnFire);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ASwatCharacter::FireEnd);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ASwatCharacter::Reload);
	PlayerInputComponent->BindAction("Chatting", IE_Pressed, this, &ASwatCharacter::Chatting);
	PlayerInputComponent->BindAction("ScoreList", IE_Pressed, this, &ASwatCharacter::ScoreList);
	PlayerInputComponent->BindAction("ScoreList", IE_Released, this, &ASwatCharacter::ScoreListEnd);
	PlayerInputComponent->BindAction("Ultimate", IE_Pressed, this, &ASwatCharacter::Ultimate);
	
	PlayerInputComponent->BindAxis("MoveForward", this, &ASwatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASwatCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ASwatCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ASwatCharacter::LookUpAtRate);
}

void ASwatCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ASwatCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void ASwatCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ASwatCharacter::LookUpAtRate(float Rate)
{
	Server_LookUpRate(UKismetMathLibrary::NormalizedDeltaRotator(GetControlRotation(), GetActorRotation()).Pitch);
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ASwatCharacter::OnFire()
{
	AParentsPlayerState* swatPlayerState = Cast<AParentsPlayerState>(GetPlayerState());

	if (swatPlayerState)
	{
		swatCombatComponent->SwatRecoil(true);

		if (!swatPlayerState->GetUltimate())
		{
			GetWorld()->GetTimerManager().SetTimer(
				fireWaitHandle,
				this,
				&ASwatCharacter::Firing,
				swatPlayerState->GetFireDelay(),
				true,
				0.0f);
		}
		else
		{
			UltimateFiring();
		}
	}
	
}

void ASwatCharacter::Firing()
{
	AParentsPlayerState* swatPlayerState = Cast<AParentsPlayerState>(GetPlayerState());

	if (swatPlayerState)
	{
		if (!swatPlayerState->CheckAmmoZero() &&
			!swatPlayerState->GetReload() &&
			!swatPlayerState->GetDeath())
		{
			FVector start = GetFollowCamera()->GetComponentLocation() + (GetFollowCamera()->GetForwardVector() * 180.0f) + (GetFollowCamera()->GetUpVector() * -10.0f);
			FVector end = GetFollowCamera()->GetComponentLocation() + (GetFollowCamera()->GetForwardVector() * swatPlayerState->GetWeaponMaxDis());
			swatCombatComponent->Server_OnFire(start, end, swatPlayerState, false);
			swatCombatComponent->SwatRecoil();
			swatPlayerState->FireAmmo();
		}
	}
	
}

void ASwatCharacter::UltimateFiring()
{
	AParentsPlayerState* swatPlayerState = Cast<AParentsPlayerState>(GetPlayerState());

	if (swatPlayerState)
	{
		if (!swatPlayerState->GetReload() &&
			!swatPlayerState->GetDeath())
		{
			FVector start = GetFollowCamera()->GetComponentLocation() + (GetFollowCamera()->GetForwardVector() * 180.0f);
			FVector end = GetFollowCamera()->GetComponentLocation() + (GetFollowCamera()->GetForwardVector() * swatPlayerState->GetWeaponMaxDis());
			swatCombatComponent->Server_OnFire(start, end, swatPlayerState, true);
			if (swatPlayerState->FireUltimate())
			{
				swatPlayerState->SetCurUltimateGauge(0, true);
				swatPlayerState->SetUltimate(false);
				swatCombatComponent->Server_SpawnWeapon(false);
			}
		}
	}
	
}

void ASwatCharacter::FireEnd()
{
	GetWorldTimerManager().ClearTimer(fireWaitHandle);
}

void ASwatCharacter::Reload()
{	
	AParentsPlayerState* swatPlayerState = Cast<AParentsPlayerState>(GetPlayerState());

	if (swatPlayerState)
	{
		if (swatPlayerState->GetReload() || swatPlayerState->GetDeath())
		{
			return;
		}

		swatPlayerState->SetReload(true);
		swatCombatComponent->Server_Reload();
	}
	
}

void ASwatCharacter::Chatting()
{
	AMainPlayerController* mainPlayerController = Cast<AMainPlayerController>(GetController());
	if (mainPlayerController)
	{
		mainPlayerController->ShowChat(true);
	}
	
}

void ASwatCharacter::ScoreList()
{
	AMainPlayerController* mainPlayerController = Cast<AMainPlayerController>(GetController());
	if (mainPlayerController)
	{
		mainPlayerController->Server_SendPlayerScore();
	}	
}

void ASwatCharacter::ScoreListEnd()
{
	AMainPlayerController* mainPlayerController = Cast<AMainPlayerController>(GetController());
	if (mainPlayerController)
	{
		mainPlayerController->DrawEndPlayerScore();
	}	
}

void ASwatCharacter::Ultimate()
{
	AParentsPlayerState* swatPlayerState = Cast<AParentsPlayerState>(GetPlayerState());

	if (swatPlayerState)
	{
		if (!swatPlayerState->CheckUltimate())return;

		swatPlayerState->SetUltimate(true);
		swatCombatComponent->Server_SpawnWeapon(true);
	}	
}

bool ASwatCharacter::Server_LookUpRate_Validate(float _rate)
{
	return true;
}

void ASwatCharacter::Server_LookUpRate_Implementation(float _rate)
{
	lookUpAxis = _rate;
}

void ASwatCharacter::SetSwatCombatComponent(USwatCombatComponent* _comp)
{
	swatCombatComponent = _comp;
	check(swatCombatComponent != nullptr);
}

void ASwatCharacter::SpawnWeapon()
{
	swatWeapon = GetWorld()->SpawnActor<APlayerWeapon>(swatWeaponBP);
	check(swatWeapon != nullptr);

	swatWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("WeaponSocket2"));
}

void ASwatCharacter::SpawnUltimateWeapon()
{
	ultimateWeapon = GetWorld()->SpawnActor<APlayerWeapon>(ultimateWeaponBP);
	check(ultimateWeapon != nullptr);

	ultimateWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("WeaponSocket2"));
}
