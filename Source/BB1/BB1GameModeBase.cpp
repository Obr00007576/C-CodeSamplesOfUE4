// Copyright Epic Games, Inc. All Rights Reserved.


#include "BB1GameModeBase.h"
void MyInputMode::ApplyInputMode(FReply& SlateOperations, class UGameViewportClient& GameViewportClient) const
{
	TSharedPtr<SViewport> ViewportWidget = GameViewportClient.GetGameViewportWidget();
	if (ViewportWidget.IsValid())
	{
		if (isHighPrecisionMouseMovement)
		{
			GameViewportClient.SetMouseLockMode(EMouseLockMode::LockAlways);
			SlateOperations.LockMouseToWidget(ViewportWidget.ToSharedRef());
			SlateOperations.UseHighPrecisionMouseMovement(ViewportWidget.ToSharedRef());
			GameViewportClient.SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
		}
		else
		{
			GameViewportClient.SetMouseLockMode(EMouseLockMode::DoNotLock);
			SlateOperations.ReleaseMouseLock();
			GameViewportClient.SetMouseCaptureMode(EMouseCaptureMode::CaptureDuringMouseDown);
			if (SlateOperations.ShouldUseHighPrecisionMouse())
			{
				SlateOperations.ReleaseMouseCapture();
			}
		}
		GameViewportClient.SetIgnoreInput(false);
		GameViewportClient.SetHideCursorDuringCapture(false);
	}
}
ABB1GameModeBase::ABB1GameModeBase():Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("Blueprint'/Game/Blueprints/MyPawn'"));
	DefaultPawnClass = PlayerPawnBPClass.Class;
	GameStateClass = ABB1GameStateBase::StaticClass();
}
void ABB1GameModeBase::SetPlayerDefaults(APawn* PlayerPawn)
{
	Super::SetPlayerDefaults(PlayerPawn);
	if (((APlayerMove*)PlayerPawn)->BouncingMesh != NULL)
	{
		((APlayerMove*)PlayerPawn)->BouncingMesh->SetActorLocation(FVector(0,(GetNumPlayers()-1)*700-350,100));
	}
}
