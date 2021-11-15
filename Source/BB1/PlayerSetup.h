// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "PlayerMove.h"
#include "EngineUtils.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BB1GameModeBase.h"
#include "Components/LineBatchComponent.h"
#include "Engine/StaticMeshActor.h"
#include "MyMissile.h"
#include "Kismet/KismetMathLibrary.h"
#include "PlayerSetup.generated.h"
class APlayerMove;
UCLASS()
class BB1_API APlayerSetup : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerSetup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UStaticMesh* SphereMesh;
	UMaterialInterface* Material;
	APlayerMove* MoveMode;
	UClass* Missile;
	UFUNCTION(Server, Reliable)
		void RPCSwitchPossession(APlayerController* MyController);
	UFUNCTION(Server, Reliable)
		void RPCCallMissileSupport(const FVector &Location);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:
	UStaticMeshComponent* Mesh;
	void SwitchMode();
	void Click();
	void SetMoveMode(APlayerMove* const p);
};
