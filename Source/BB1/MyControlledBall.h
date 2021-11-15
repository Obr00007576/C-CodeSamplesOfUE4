// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyStaticMesh.h"
#include "GameFramework/SpringArmComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "Net/UnrealNetwork.h"
#include "Components/WidgetComponent.h"
#include "MyControlledBall.generated.h"

/**
 * 
 */
UCLASS()
class BB1_API AMyControlledBall : public AMyStaticMesh
{
	GENERATED_BODY()
public:
	AMyControlledBall();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float OriginalMassScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		USpringArmComponent* WidgetSpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UWidgetComponent* AboveBallWidget;
	UPROPERTY(EditAnywhere, Replicated, BlueprintReadWrite)
		float mass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float currentmass;
};
