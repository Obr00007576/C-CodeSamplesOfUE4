// Fill out your copyright notice in the Description page of Project Settings.


#include "MyStaticMesh.h"

// Sets default values
AMyStaticMesh::AMyStaticMesh()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SuperMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	RootComponent = SuperMesh;
	bReplicates = true;
	bAlwaysRelevant = true;
	//Mesh->AttachToComponent(SuperMesh,FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld,false));
}

// Called when the game starts or when spawned
void AMyStaticMesh::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);
}

// Called every frame
void AMyStaticMesh::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UStaticMeshComponent* AMyStaticMesh::GetStaticMeshComponent()
{
	return SuperMesh;
}
