// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Actor.h"
#include "MyStaticMesh.generated.h"


UCLASS()
class BB1_API AMyStaticMesh : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyStaticMesh();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UStaticMeshComponent* GetStaticMeshComponent();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UStaticMeshComponent* SuperMesh;
};
