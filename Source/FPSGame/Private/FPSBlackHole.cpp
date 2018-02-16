// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSBlackHole.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/Core/Public/Containers/Array.h"
#include "Engine/StaticMeshActor.h"


// Sets default values
AFPSBlackHole::AFPSBlackHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	PhysSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("PhysSphereComp"));
	PhysSphereComp->SetWorldScale3D(FVector(100, 100, 100));
	PhysSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	PhysSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	PhysSphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
	PhysSphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	CenterSphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("CenterSphereComp"));
	CenterSphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CenterSphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	CenterSphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

	CenterSphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSBlackHole::OnBeginOverlap);
	
	CenterSphereComp->SetupAttachment(MeshComp);
}

// Called when the game starts or when spawned
void AFPSBlackHole::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFPSBlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UPrimitiveComponent*> overlappers;
	PhysSphereComp->GetOverlappingComponents(overlappers);

	for (int32 i = 0; i < overlappers.Num(); i++)
	{
		UPrimitiveComponent* primComp = overlappers[i];

		if (primComp && primComp->IsSimulatingPhysics())
		{
			primComp->AddRadialForce(GetActorLocation(), PhysSphereComp->GetScaledSphereRadius(), -2000, ERadialImpulseFalloff::RIF_Constant, true);
		}
	}
}

void AFPSBlackHole::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);
	
	
}

void AFPSBlackHole::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OtherActor->Destroy();
}

