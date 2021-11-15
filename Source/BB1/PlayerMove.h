// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BB1GameModeBase.h"
#include "Net/UnrealNetwork.h"
#include "Misc/OutputDeviceNull.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/Engine.h"
#include "Blueprint/UserWidget.h"
#include "MyStaticMesh.h"
#include "PlayerMove.generated.h"

class APlayerSetup;
UCLASS()
class BB1_API APlayerMove : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerMove();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void ShowYou();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	void MouseYaw(float AxisVlue);
	void MousePitch(float AxisValue);
	void MoveForward(float AxisValue);
	void MoveLeft(float AxisValue);
	void SwitchMode();
	UFUNCTION()
		void OnRep_BouncingMesh();
	UFUNCTION(Server, Reliable)
		void RPCMove(const FVector& MoveDirection);
	UFUNCTION(Server, Reliable)
		void RPCSwitchPossession(APlayerController* MyController);
protected:
	APlayerController* PlayerController;
	UStaticMeshComponent* Mesh;
	FVector2D Move;
	UStaticMesh* SphereMesh;
	UMaterialInterface* Material;
	UClass* ControlledBall;
public:
	APlayerSetup* SetupMode;
	FVector SpawnLocation;
	UUserWidget* Noob;
	UPROPERTY(EditAnywhere)
		TSubclassOf<UUserWidget> MyWidget;
	UPROPERTY(BlueprintReadWrite, Category = Camera)
		UCameraComponent* Camera;
	UPROPERTY(EditAnywhere, ReplicatedUsing = OnRep_BouncingMesh, Category = StaticActor)
		AMyStaticMesh* BouncingMesh;
};
