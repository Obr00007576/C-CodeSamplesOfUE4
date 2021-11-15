// Fill out your copyright notice in the Description page of Project Settings.


#include "MyBoxingSpeedBag.h"

// Sets default values
AMyBoxingSpeedBag::AMyBoxingSpeedBag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpringConstraintComponent = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("SpringConstraint"));
	RootSceneCompoment = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootSceneComponent"));
	PunchBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PunchBox"));
	Pedestal = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pedestal"));
	RootComponent = RootSceneCompoment;
	RemainingTime = 0.f;
}

// Called when the game starts or when spawned
void AMyBoxingSpeedBag::BeginPlay()
{
	Super::BeginPlay();
	PunchBox->OnComponentHit.AddUniqueDynamic(this, &AMyBoxingSpeedBag::OnBoxHit);
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(TakeMassCoolDownTimerHandle, 8.0f, false);
	}
}

// Called every frame
void AMyBoxingSpeedBag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (HasAuthority())
	{
		RemainingTime = GetWorldTimerManager().GetTimerRemaining(TakeMassCoolDownTimerHandle);
	}
}

void AMyBoxingSpeedBag::OnBoxHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (HasAuthority())
	{
		if (OtherActor->IsA(AMyControlledBall::StaticClass()) && !GetWorldTimerManager().IsTimerActive(TakeMassCoolDownTimerHandle))
		{
			UStaticMeshComponent* OtherStaticMeshComponent;
			OtherStaticMeshComponent=((AMyControlledBall*)OtherActor)->GetStaticMeshComponent();
			float SpeedPunchBox = PunchBox->GetPhysicsLinearVelocity().Size();
			if (SpeedPunchBox < 500)
			{
				OtherStaticMeshComponent->SetAllMassScale(OtherStaticMeshComponent->GetMassScale() * 0.75);
			}
			else if(SpeedPunchBox<1000)
			{
				OtherStaticMeshComponent->SetAllMassScale(OtherStaticMeshComponent->GetMassScale() + ((AMyControlledBall*)OtherActor)->OriginalMassScale/2);
			}
			else
			{
				OtherStaticMeshComponent->SetAllMassScale(OtherStaticMeshComponent->GetMassScale() + ((AMyControlledBall*)OtherActor)->OriginalMassScale);
			}
			((AMyControlledBall*)OtherActor)->mass = OtherStaticMeshComponent->GetMass();
			GetWorldTimerManager().SetTimer(TakeMassCoolDownTimerHandle, 8.0f, false);
			GLog->Log(TEXT("========================================================================================Mass================"));
			GLog->Log(FString("SpeedPunchBox: ")+ FString::SanitizeFloat(SpeedPunchBox));
			GLog->Log(FString("SpeedBall: ") + FString::SanitizeFloat(OtherStaticMeshComponent->GetPhysicsLinearVelocity().Size()));
		}
	}
}

void AMyBoxingSpeedBag::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyBoxingSpeedBag, RemainingTime);
}