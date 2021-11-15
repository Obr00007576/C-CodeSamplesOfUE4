// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"
#include "ParticleSystemActor.generated.h"

UCLASS()
class BB1_API AParticleSystemActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticleSystemActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UParticleSystemComponent* GetParticleSystem();
	UFUNCTION(Server, Reliable)
		void RPCDestroy();
	UFUNCTION()
		void OnParticleSystemFinished(UParticleSystemComponent* PSystem);
	void DestroyByTimer();
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UParticleSystemComponent* ParticleSystemComponent;
	UPROPERTY(Replicated)
		bool bActivated;
	FTimerHandle DestroyTimerHandle;
};
