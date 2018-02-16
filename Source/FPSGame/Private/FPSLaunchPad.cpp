// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);

	SphereComp->SetupAttachment(MeshComp);


	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleOverlap);
}

void AFPSLaunchPad::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) 
{
	ACharacter* character = Cast<ACharacter>(OtherActor);
	if (character)
	{
		FVector launchVector = GetActorForwardVector();
		launchVector.Z = 1;
		launchVector.Normalize();
		launchVector *= LaunchVelocity;
		character->LaunchPawn(launchVector, true, true);

		UGameplayStatics::SpawnEmitterAtLocation(this, LaunchFX, GetActorLocation());

		UE_LOG(LogTemp, Log, TEXT("A Character should be launching!."));
	}
	else
	{
		UStaticMeshComponent* box = OtherActor->FindComponentByClass<UStaticMeshComponent>();
		
		if (box)
		{
			//box->AddImpulse(LaunchVelocity);
			UE_LOG(LogTemp, Log, TEXT("A Box should be launching!."));
		}
	}
}

