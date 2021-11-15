// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MyStaticMesh.h"
#include "Components/SphereComponent.h"
#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "ParticleSystemActor.h"
#include "Net/UnrealNetwork.h"
#include "MyBoxingSpeedBag.h"
#include "MyMissile.generated.h"

UCLASS()
class BB1_API AMyMissile : public AMyStaticMesh
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
		void OnMyMissileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	AMyMissile();
	void Explode();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UClass* FireBallPartileClass;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Particle Effect")
		UParticleSystem* ExplosionEffect;
	UPROPERTY(EditAnywhere, Replicated, Category = "Particle Effect")
		AParticleSystemActor* FireBall;

};
