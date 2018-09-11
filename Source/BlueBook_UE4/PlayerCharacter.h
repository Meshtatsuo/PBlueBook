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
	int32 playerMaxHealth;
	int32 playerHealth;
	int32 playerMaxStamina;
	int32 playerStamina;
	int32 staminaDrainRate;
	//booleans used to enable and disable abilities
	bool canJump = false;
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Any)
		TArray<TSubclassOf<AInteractableObject>> inventory;
		*/
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
	void Sprint();
	void StartCrouch();
	// Checking for interactable objects
	void CheckForInteractables();

private:

	UPROPERTY(VisibleAnywhere)
		UCameraComponent* FirstPersonCameraComponent;
	UPROPERTY(VisibleAnywhere)
		UBoxComponent* defaultCollisionComp;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* defaultPlayerMesh;

	//Stores item being looked at by the player//
	AInteractableObject* CurrentObject;

	//Distance for line trace + trace vectors
	FVector StartTrace;
	FVector EndTrace;
	float Reach;
	//String to store name of object that is in focus
	FString objectInFocus = "";
};
