// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PlayerMove.h"
#include "PlayerSetup.h"
#include "Widgets/SViewport.h"
#include "Widgets/SWidget.h"
#include "BB1GameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "BB1GameModeBase.generated.h"

/**
 * 
 */
struct MyInputMode : public FInputModeDataBase
{
public:
	MyInputMode():isHighPrecisionMouseMovement(false)
	{}
	MyInputMode(bool isHighPrecisionMouseMovement)
	{
		this->isHighPrecisionMouseMovement = isHighPrecisionMouseMovement;
	}
protected:
	bool isHighPrecisionMouseMovement;
	virtual void ApplyInputMode(FReply& SlateOperations, class UGameViewportClient& GameViewportClient) const override;
};

UCLASS()
class BB1_API ABB1GameModeBase : public AGameMode
{
	GENERATED_BODY()
public:
	ABB1GameModeBase();
	void SetPlayerDefaults(APawn* PlayerPawn) override;
};