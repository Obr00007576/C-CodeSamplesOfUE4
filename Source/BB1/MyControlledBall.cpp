// Fill out your copyright notice in the Description page of Project Settings.


#include "MyControlledBall.h"

AMyControlledBall::AMyControlledBall() :Super()
{
	mass = 0.f;
	currentmass = 0.f;
	OriginalMassScale = 0.f;
	WidgetSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("WidgetSpringArm"));
	AboveBallWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("AboveBallWidget"));
	WidgetSpringArm->SetupAttachment(RootComponent);
	AboveBallWidget->SetupAttachment(WidgetSpringArm);
}
void AMyControlledBall::BeginPlay()
{
	Super::BeginPlay();
}
void AMyControlledBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!HasAuthority())
	{
		FOutputDeviceNull ar;
		const FString command = FString::Printf(TEXT("SetMassText %.1fKG"), mass);
		CallFunctionByNameWithArguments(*command, ar, NULL, true);
		if (currentmass != mass)
		{
			int difference = int(mass - currentmass);
			FOutputDeviceNull ar1;
			const FString command1 = FString::Printf(TEXT("PlayBonusEffect %i"), difference);
			CallFunctionByNameWithArguments(*command1, ar1, NULL, true);
			currentmass = mass;
		}
	}
}
void AMyControlledBall::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyControlledBall, mass);
}