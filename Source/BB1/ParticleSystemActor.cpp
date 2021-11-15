// Fill out your copyright notice in the Description page of Project Settings.
#include "ParticleSystemActor.h"

// Sets default values
AParticleSystemActor::AParticleSystemActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	RootComponent = ParticleSystemComponent;
	bActivated = true;
}

// Called when the game starts or when spawned
void AParticleSystemActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AParticleSystemActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bActivated && ParticleSystemComponent->IsActive())
	{
		ParticleSystemComponent->Deactivate();
	}
}

UParticleSystemComponent* AParticleSystemActor::GetParticleSystem()
{
	return ParticleSystemComponent;
}

void AParticleSystemActor::OnParticleSystemFinished(UParticleSystemComponent* PSystem)
{
	static int i = 1;
	GEngine->AddOnScreenDebugMessage(i++, 2, FColor::Red, TEXT("FireBall Finished"));
	RPCDestroy();
}
void AParticleSystemActor::RPCDestroy_Implementation()
{
	Destroy();
}

void AParticleSystemActor::DestroyByTimer()
{
	GetWorldTimerManager().ClearTimer(DestroyTimerHandle);
	Destroy();
}

void AParticleSystemActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AParticleSystemActor, bActivated);
}