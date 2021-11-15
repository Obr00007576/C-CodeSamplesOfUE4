// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PhysicsEngine/PhysicsConstraintComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyControlledBall.h"
#include "MyBoxingSpeedBag.generated.h"

UCLASS()
class BB1_API AMyBoxingSpeedBag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBoxingSpeedBag();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
		void OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UPhysicsConstraintComponent* SpringConstraintComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* PunchBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* Pedestal;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USceneComponent* RootSceneCompoment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		FTimerHandle TakeMassCoolDownTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
		float RemainingTime;
};
