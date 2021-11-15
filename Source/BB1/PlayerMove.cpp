// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerMove.h"
#include "PlayerSetup.h"

// Sets default values
APlayerMove::APlayerMove()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultMesh"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->AttachToComponent(Mesh,FAttachmentTransformRules::KeepRelativeTransform);
	RootComponent = Mesh;
	SphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	Material = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Engine/BasicShapes/BasicShapeMaterial")).Object;
	static ConstructorHelpers::FObjectFinder<UBlueprint> ControlledBallBP(TEXT("Blueprint'/Game/Blueprints/ControlledBall'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> MyWidgetBP(TEXT("Blueprint'/Game/Blueprints/MyWidget'"));
	ControlledBall = (UClass*)ControlledBallBP.Object->GeneratedClass;
	MyWidget = MyWidgetBP.Object->GeneratedClass;
	SpawnLocation = FVector(0, -100, 100);
}

// Called when the game starts or when spawned
void APlayerMove::BeginPlay()
{
	Super::BeginPlay();
	Noob=CreateWidget(GetWorld(), MyWidget);
	if (GetLocalRole() == ROLE_AutonomousProxy)
	{
		PlayerController = GetWorld()->GetFirstPlayerController();
		PlayerController->SetInputMode(MyInputMode(true));
	}
	if (HasAuthority())
	{
		SetupMode = GetWorld()->SpawnActor<APlayerSetup>(FVector::ZeroVector, FRotator::ZeroRotator);
		SetupMode->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		SetupMode->SetMoveMode(this);
		SetupMode->SetOwner(this);
	}
	if (BouncingMesh == nullptr && HasAuthority())
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.Owner = this;
		BouncingMesh = GetWorld()->SpawnActor<AMyStaticMesh>(ControlledBall, SpawnLocation,FRotator::ZeroRotator, SpawnInfo);
	}
}

void APlayerMove::RPCMove_Implementation(const FVector& MoveDirection)
{
	if (BouncingMesh != nullptr)
	{
		BouncingMesh->GetStaticMeshComponent()->AddImpulse(MoveDirection * BouncingMesh->GetStaticMeshComponent()->GetMass()*9);
	}
}

// Called every frame
void APlayerMove::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(!Move.IsZero())
	{
		FVector NewMove = FVector::ZeroVector;
		NewMove.X = Move.X;
		NewMove.Y = Move.Y;
		RPCMove(NewMove);
	}
	if (BouncingMesh != NULL)
	{
		if (BouncingMesh->GetActorLocation().Z <= -500)
		{
			if (HasAuthority())
			{
				if (BouncingMesh->GetActorLocation().Z <= -500)
				{
					((ABB1GameStateBase*)GetWorld()->GetGameState())->SurvivorNumber -= 1;
					GLog->Log(FString::FromInt(((ABB1GameStateBase*)GetWorld()->GetGameState())->SurvivorNumber));
					BouncingMesh->Destroy();
					BouncingMesh = nullptr;
				}
			}
			if (!HasAuthority())
			{
				//Noob->AddToViewport();
			}
		}
	}
}

// Called to bind functionality to input
void APlayerMove::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAxis("MoveForward", this, &APlayerMove::MoveForward);
	InputComponent->BindAxis("MoveLeft", this, &APlayerMove::MoveLeft);
	InputComponent->BindAction("SwitchMode",EInputEvent::IE_Pressed, this, &APlayerMove::SwitchMode);
}

void APlayerMove::MoveForward(float AxisValue)
{
	Move.X = AxisValue;
}

void APlayerMove::MoveLeft(float AxisValue)
{
	Move.Y = AxisValue;
}

void APlayerMove::SwitchMode()
{
	RPCSwitchPossession(PlayerController);
	PlayerController->bShowMouseCursor = true;
	PlayerController->SetInputMode(MyInputMode(false));
	PlayerController->SetMouseLocation(GSystemResolution.ResX/2, GSystemResolution.ResY/2);
}

void APlayerMove::RPCSwitchPossession_Implementation(APlayerController* MyController)
{
	MyController->UnPossess();
	MyController->Possess(SetupMode);
	MyController->SetViewTarget(this);
}

void APlayerMove::ShowYou()
{
	FOutputDeviceNull ar;
	const FString command = FString::Printf(TEXT("SetMeText You"));
	if (BouncingMesh != nullptr)
	{
		BouncingMesh->CallFunctionByNameWithArguments(*command, ar, NULL, true);
	}
}

void APlayerMove::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerMove, BouncingMesh);
}

void APlayerMove::OnRep_BouncingMesh()
{
	ShowYou();
}
