// Fill out your copyright notice in the Description page of Project Settings.


#include "BB1GameStateBase.h"

ABB1GameStateBase::ABB1GameStateBase():Super()
{
	PrimaryActorTick.bCanEverTick = true;
	SurvivorNumber = 2;
}
void ABB1GameStateBase::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	if (SurvivorNumber == 1)
	{
		if (HasAuthority())
		{
			SurvivorNumber = 2;
			((AGameMode*)GetWorld()->GetAuthGameMode())->RestartGame();
		}
	}
	if (GetWorld()->GetAuthGameMode() != NULL)
	{
		ABB1GameModeBase* CurrentGameMode = (ABB1GameModeBase*)GetWorld()->GetAuthGameMode();
		UMyGameInstance* CurrentGameInstance = (UMyGameInstance*)GetGameInstance();
		if (CurrentGameMode->GetNumPlayers() == 2 && !CurrentGameInstance->bReadyToStart)
		{
			CurrentGameInstance->bReadyToStart = true;
			if (HasAuthority())
			{
				GetWorldTimerManager().SetTimer(ReadyToStartTimerHandle, this, &ABB1GameStateBase::RefreshGame, 5, false);
			}
		}
	}
}

void ABB1GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABB1GameStateBase, SurvivorNumber);
}

void ABB1GameStateBase::RefreshGame()
{
	((AGameMode*)GetWorld()->GetAuthGameMode())->RestartGame();
}