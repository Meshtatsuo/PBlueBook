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

	//movementSpeeds
	defaultMovementSpeed = 200;
	injuredMovementSpeed = 150;
	sprintMovementSpeed = 700;
	//sets movementSpeed to default
	GetCharacterMovement()->MaxWalkSpeed = defaultMovementSpeed;
	//Health and Stamina Check
	playerMaxHealth = 100;
	playerHealth = 75;
	playerMaxStamina = 100;
	playerStamina = 50;
	//Initialize Inventory
	//inventory.Empty;


	//Building character. Creating camera component
	defaultCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("defaultCollisionComp"));
	defaultCollisionComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	defaultPlayerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("defaultMesh"));
	defaultPlayerMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale, NAME_None);
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->MaxWalkSpeed = defaultMovementSpeed;

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckForInteractables();
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
	InputComponent->BindAction("Sprint", IE_Pressed, this, &APlayerCharacter::Sprint);
	InputComponent->BindAction("StartCrouch", IE_Pressed, this, &APlayerCharacter::StartCrouch);
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
	if (CurrentObject) {

		//switch (CurrentObject->objectType) {


		//}

	}

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
void APlayerCharacter::Sprint() {

}

void APlayerCharacter::StartCrouch() {
	//Crouch();
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