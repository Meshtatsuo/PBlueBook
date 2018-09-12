// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Components/InputComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Object focusing and interacting
	Reach = 250;
	objectInFocus = "";
	//Default player state
	canJump = true;
	canRun = true;
	flashlightOn = false;
	//movementSpeeds
	defaultMovementSpeed = 200;
	injuredMovementSpeed = 150;
	sprintMovementSpeed = 500;
	//sets movementSpeed to default
	GetCharacterMovement()->MaxWalkSpeed = defaultMovementSpeed;
	//Health and Stamina Check
	playerMaxHealth = 100;
	playerHealth = 75;
	playerMaxStamina = 100;
	playerStamina = 50;
	staminaDrainRate = 2;
	//Initialize Flashlight vars
	maxBatteryLife = 1;
	currentBatteryLife = maxBatteryLife;
	batteryDrainTickRate = 3.5f;
	batteryDrainRate = .05f;
	//Initialize Inventory
	//inventory.Empty;


	//Building character. Creating camera component
	defaultCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("defaultCollisionComp"));
	defaultCollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	defaultPlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("defaultMesh"));
	defaultPlayerMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	playerFlashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	playerFlashlight->SetupAttachment(defaultPlayerMesh);
	playerFlashlight->SetIntensity(0);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = defaultMovementSpeed;
	//Timer every "tickrate" that loops
	GetWorldTimerManager().SetTimer(spawnTimer, this, &APlayerCharacter::UpdateBattery, batteryDrainTickRate, true);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckForInteractables();
	//UpdateStamina();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
	InputComponent->BindAxis("LookPitch", this, &APlayerCharacter::LookPitch);
	InputComponent->BindAxis("LookYaw", this, &APlayerCharacter::LookYaw);
	InputComponent->BindAction("Use", IE_Pressed, this, &APlayerCharacter::Use);
	InputComponent->BindAction("Jump", IE_Pressed, this, &APlayerCharacter::StartJump);
	InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &APlayerCharacter::EndSprint);
	InputComponent->BindAction("StartCrouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
	InputComponent->BindAction("Flashlight", IE_Pressed, this, &APlayerCharacter::ToggleFlashlight);
}

void APlayerCharacter::MoveForward(float amount) {
	//Rotation(roll, yaw, pitch) -- setting roll and pitch to 0 keeps player moving on single plane
	FRotator Rotation(0, GetActorRotation().Yaw, 0);
	FVector Forward = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
	AddMovementInput(Forward, amount);
}

void APlayerCharacter::MoveRight(float amount) {
	//allows player to sidestep left and right
	FRotator Rotation(0, GetActorRotation().Yaw, 0);
	FVector Right = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(Right, amount);
}

void APlayerCharacter::LookPitch(float amount) {
	//adds pitch values based off mouse Y movement
	AddControllerPitchInput(amount);
}

void APlayerCharacter::LookYaw(float amount) {
	//adds pitch values based off mouse X movement
	AddControllerYawInput(amount);
}

void APlayerCharacter::Use() {

}

void APlayerCharacter::StartJump() {
	if (canJump == true) {
		Jump();
		//UEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, "Can Jump");
	}
	else {
		//UEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Can't Jump");
	}
}
void APlayerCharacter::StartSprint() {
	if (canRun) {
		GetCharacterMovement()->MaxWalkSpeed = sprintMovementSpeed;
		isRunning = true;
	}
	else if (!canRun) {
		EndSprint();
	}
}

void APlayerCharacter::EndSprint() {
	GetCharacterMovement()->MaxWalkSpeed = defaultMovementSpeed;
	isRunning = false;
}

void APlayerCharacter::StartCrouch() {
	//Crouch();
}

void APlayerCharacter::ToggleFlashlight() {
	if (flashlightOn) {
		//playerFlashlight->SetIntensity(0);
		flashlightOn = false;
	}
	else if (!flashlightOn && currentBatteryLife > 0) {
		flashlightOn = true;
		//playerFlashlight->SetIntensity(2);
	}
}

void APlayerCharacter::UpdateBattery() {
	if (flashlightOn) {
		if (currentBatteryLife > 0) {
			currentBatteryLife -= batteryDrainRate;
		}
	}

	if (currentBatteryLife <= 0) {
		currentBatteryLife = 0;
		flashlightOn = false;
	}

}

//Checks environement for interactables
void APlayerCharacter::CheckForInteractables() {
	//Creating FHitResult variable and setting up trace vectors.
	FHitResult LinetraceHit;
	StartTrace = FirstPersonCameraComponent->GetComponentLocation();
	EndTrace = (FirstPersonCameraComponent->GetForwardVector() * Reach) + StartTrace;
	//Used to ignore potential collision with playercharacter
	FCollisionQueryParams CQP;
	CQP.AddIgnoredActor(this);
	//begin the trace
	GetWorld()->LineTraceSingleByChannel(LinetraceHit, StartTrace, EndTrace, ECollisionChannel::ECC_WorldDynamic, CQP);
	//store hit result as potential object to be worked with
	//CAST FOR EACH CLASS THAT CAN BE INTERACTED WITH
	AInteractableObject* PotentialObject = Cast<AInteractableObject>(LinetraceHit.GetActor());
	//checks to see if there was a hit. If not, sets currentobject to nullptr.
	if (PotentialObject == NULL) {
		CurrentObject = nullptr;
		objectInFocus = "";
		return;
	}
	//if hit successful, store object as currentobject, get object's name, print name to screen and store name in string objectInFocus.
	else if (PotentialObject != NULL) {
		CurrentObject = PotentialObject;
		CurrentObject->GetName();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, *CurrentObject->GetName());
		objectInFocus = *CurrentObject->GetName();
	}

}


//Updates player health

void APlayerCharacter::UpdateHealth(bool isDamage, int amount) {
	if (isDamage == true) {
		playerHealth -= amount;
		if (playerHealth <= 0) {
			Die();
			return;
		}
		else {
			return;
		}
		return;
	}

	else if (isDamage == false) {
		playerHealth += amount;
		FMath::Clamp(playerHealth, 0.0f, playerMaxHealth);
		return;
	}
}
//Updates player stamina and player ability to run
void APlayerCharacter::UpdateStamina() {

}

void APlayerCharacter::Die() {

}