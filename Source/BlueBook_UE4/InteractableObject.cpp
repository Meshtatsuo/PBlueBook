// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractableObject.h"


// Sets default values
AInteractableObject::AInteractableObject()
{
	defaultMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BlockMesh"));
}

// Called when the game starts or when spawned
void AInteractableObject::BeginPlay()
{
	Super::BeginPlay();	
}
