// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "InteractableObject.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class BLUEBOOK_UE4_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	APlayerCharacter();


	//Movement variables
	float defaultMovementSpeed;
	float injuredMovementSpeed;
	float sprintMovementSpeed;
	//Player health and stamina check
	float playerMaxHealth;
	float playerHealth;
	float playerMaxStamina;
	float playerStamina;
	float staminaDrainRate;
	bool isRunning;
	bool canRun;
	//booleans used to enable and disable abilities
	bool canJump = false;
	UPROPERTY(BlueprintReadOnly)
	bool flashlightOn = false;
	//Flashlight Vars
	float maxBatteryLife;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currentBatteryLife;
	float batteryDrainRate;
	float batteryDrainTickRate;
	float newBatteryCharge;
	FTimerHandle spawnTimer;

	//RADIO VARS
	float scrubRate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float currentFrequency;
	UPROPERTY(VisibleAnywhere)
	bool radioIsOn;
	
	//Inventory Tracking
	int batteryCount;
	int clearanceLevel;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Axis Mapping Events
	void MoveForward(float amount);
	void MoveRight(float amount);
	void LookPitch(float amount);
	void LookYaw(float amount);
	// Action Mapping Events
	void Use();
	void StartJump();
	void StartSprint();
	void EndSprint();
	void StartCrouch();
	void EndCrouch();
	void ToggleFlashlight();
	void LoadBatteries();
	//Radio Control

	void ScrubRadioFreqUp();
	void ScrubRadioFreqDown();
	void UseRadio();
	void StopRadio();
	// Checking for interactable objects
	void CheckForInteractables();
	// Manage Player Stamina/Health/Battery
	void UpdateBattery();
	void UpdateStamina();
	void UpdateHealth(bool isDamage, int dmgAmount);
	void Die();


	//Stores item being looked at by the player//
	AInteractableObject* currentObject;
	AInteractableObject* lastCurrentObject;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	USpringArmComponent* springArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USpotLightComponent* playerFlashlight;
private:

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* defaultCollisionComp;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* defaultPlayerMesh;



	//Distance for line trace + trace vectors
	FVector StartTrace;
	FVector EndTrace;
	float Reach;
	//String to store name of object that is in focus
	FString objectInFocus = "";
};
