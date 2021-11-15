// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerSetup.h"
#include "PlayerMove.h"

// Sets default values

APlayerSetup::APlayerSetup()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DefaultMesh"));
	RootComponent = Mesh;
	SphereMesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'")).Object;
	Material = ConstructorHelpers::FObjectFinder<UMaterialInterface>(TEXT("/Engine/BasicShapes/BasicShapeMaterial")).Object;
	static ConstructorHelpers::FObjectFinder<UBlueprint> MissileBP(TEXT("Blueprint'/Game/Blueprints/Missile'"));
	Missile = (UClass*)MissileBP.Object->GeneratedClass;
	bOnlyRelevantToOwner=true;
}

// Called when the game starts or when spawned
void APlayerSetup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerSetup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerSetup::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("SwitchMode", EInputEvent::IE_Pressed, this, &APlayerSetup::SwitchMode);
	InputComponent->BindAction("Click", EInputEvent::IE_Pressed, this, &APlayerSetup::Click);
}

void APlayerSetup::SwitchMode()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	RPCSwitchPossession(PlayerController);
	PlayerController->bShowMouseCursor = false;
	PlayerController->SetInputMode(MyInputMode(true));
}

void APlayerSetup::RPCSwitchPossession_Implementation(APlayerController* MyController)
{
	MyController->UnPossess();
	MyController->Possess(MoveMode);
}

void APlayerSetup::Click()
{
	FVector worldLocation;
	FVector worldDirection;
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	PlayerController->DeprojectMousePositionToWorld(worldLocation, worldDirection);
	FHitResult hitRes;
	FCollisionQueryParams collisionQueryParams;
	collisionQueryParams.bTraceComplex = true;
	collisionQueryParams.bReturnPhysicalMaterial = true;
	bool DidTrace = GetWorld()->LineTraceSingleByChannel(hitRes, worldLocation, worldLocation+worldDirection*10000, ECC_Pawn, collisionQueryParams);
	if (DidTrace)
	{
		if (hitRes.GetActor()->GetFolderPath().ToString().Equals(FString("Ground")))
		{
			RPCCallMissileSupport(hitRes.Location + FVector(0, 0, 1000));
		}
		else
		{
			if (hitRes.GetActor()->GetClass() == AStaticMeshActor::StaticClass())
			{
				AStaticMeshActor* ball = dynamic_cast<AStaticMeshActor*>(hitRes.GetActor());
				ball->GetStaticMeshComponent()->AddImpulse(GetActorRotation().Vector()*100000);
			}
		}
	}
}

void APlayerSetup::SetMoveMode(APlayerMove* const p)
{
	MoveMode = p;
}

void APlayerSetup::RPCCallMissileSupport_Implementation(const FVector& Location)
{
	FActorSpawnParameters p;
	AMyMissile* MissileObject = GetWorld()->SpawnActor<AMyMissile>(Missile, Location, UKismetMathLibrary::FindLookAtRotation(FVector(0, 0, 0), FVector(0, 0, -1)), p);
}