// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameFramework/PlayerState.h"
#include "PlayerMove.h"
#include "BB1GameModeBase.h"
#include "MyGameInstance.h"
#include "BB1GameStateBase.generated.h"

/**
 * 
 */
UCLASS()
class BB1_API ABB1GameStateBase : public AGameState
{
	GENERATED_BODY()
public:
	ABB1GameStateBase();
	virtual void Tick(float DeltaTime) override;
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION()
		void RefreshGame();
public:
	FTimerHandle ReadyToStartTimerHandle;
	UPROPERTY(Replicated)
		int SurvivorNumber;
};
