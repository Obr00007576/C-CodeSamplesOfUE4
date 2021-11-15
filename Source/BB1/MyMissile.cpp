// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMissile.h"

void AMyMissile::BeginPlay()
{
	Super::BeginPlay();
	SuperMesh->SetNotifyRigidBodyCollision(true);
	SuperMesh->OnComponentHit.AddUniqueDynamic(this, &AMyMissile::OnMyMissileHit);
	if (HasAuthority())
	{
		FireBall = GetWorld()->SpawnActor<AParticleSystemActor>(FireBallPartileClass, SuperMesh->GetSocketLocation("FireBallSocket"), GetActorRotation());
		FireBall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}
void AMyMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
AMyMissile::AMyMissile() :Super()
{
	static ConstructorHelpers::FObjectFinder<UBlueprint> ParticleActorObject(TEXT("Blueprint'/Game/Blueprints/FireBallParticle'"));
	FireBallPartileClass = ParticleActorObject.Object->GeneratedClass;
}
void AMyMissile::Explode()
{
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), AllActors);
	for (AActor* MyActor : AllActors)
	{
		if (MyActor->IsA(AMyBoxingSpeedBag::StaticClass()))
		{
			FVector ExplosionCenter = SuperMesh->GetComponentLocation() - FVector(0, 0, 133.5f);
			float dist = FVector::Dist(ExplosionCenter, ((AMyBoxingSpeedBag*)MyActor)->PunchBox->GetComponentLocation());
			if (dist <= 400)
			{
				FVector ImpulseDirection = (((AMyBoxingSpeedBag*)MyActor)->PunchBox->GetComponentLocation() - ExplosionCenter);
				ImpulseDirection.Normalize();
				((AMyBoxingSpeedBag*)MyActor)->PunchBox->AddImpulse(ImpulseDirection * 40000000 / dist );
			}
		}
		if (HasAuthority())
		{
			if (MyActor->IsA(AMyStaticMesh::StaticClass()))
			{
				if (((AMyStaticMesh*)MyActor)->GetStaticMeshComponent()->Mobility == EComponentMobility::Movable && ((AMyStaticMesh*)MyActor) != this)
				{
					FVector ExplosionCenter = SuperMesh->GetComponentLocation() - FVector(0, 0, 133.5f);
					float dist = FVector::Dist(ExplosionCenter, MyActor->GetActorLocation());
					if (dist <= 400)
					{
						FVector ImpulseDirection = (MyActor->GetActorLocation() - ExplosionCenter);
						ImpulseDirection.Normalize();
						((AMyStaticMesh*)MyActor)->GetStaticMeshComponent()->AddImpulse(ImpulseDirection * 40000000/dist);
					}
				}
			}
		}
	}
	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(FireBall->DestroyTimerHandle, FireBall, &AParticleSystemActor::DestroyByTimer, 2.0f);
		if (FireBall != NULL)
		{
			FireBall->bActivated = false;
		}
		Destroy();
	}
}
void AMyMissile::OnMyMissileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Explode();
}
void AMyMissile::Destroyed()
{
	if (!HasAuthority())
	{
		UParticleSystemComponent* ExplosionParticleSystem = UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionEffect, GetActorLocation(), FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
		ExplosionParticleSystem->SetCastShadow(true);
	}
}
void AMyMissile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps)const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyMissile, FireBall);
}