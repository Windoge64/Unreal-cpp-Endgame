// Fill out your copyright notice in the Description page of Project Settings.

#include "BlackHole.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include <Components/PrimitiveComponent.h>
//#include <Engine/EngineTypes.h>                                  //Defines SweepResult
//#include <Steamworks/Steamv139/sdk/public/steam/steamtypes.h>	//Defines bFromSweep
//#include <GameFramework/Actor.h>

// Sets default values
ABlackHole::ABlackHole()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SuckInside = CreateDefaultSubobject<USphereComponent>(TEXT("SuckInside"));
	SuckInside->SetSphereRadius(100);
	SuckInside->SetupAttachment(MeshComp);

	//Bind to event
	SuckInside->OnComponentBeginOverlap.AddDynamic(this, &ABlackHole::OverlapInnerSphere);

	Attract = CreateDefaultSubobject<USphereComponent>(TEXT("Attract"));
	Attract->SetSphereRadius(3000);
	Attract->SetupAttachment(MeshComp);



}

void ABlackHole::OverlapInnerSphere(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	if (OtherActor)
	{
		OtherActor->Destroy();
	}
}


// Called when the game starts or when spawned


// Called every frame
void ABlackHole::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Find all overlapping components that can collide and may be physically simulating.
	TArray<UPrimitiveComponent*> OverlappingComponents;
	Attract->GetOverlappingComponents(OverlappingComponents);

	for (int32 i = 0; i < OverlappingComponents.Num(); i++)
	{
		UPrimitiveComponent *PrimeComponent = OverlappingComponents[i];
		if (PrimeComponent && PrimeComponent->IsSimulatingPhysics())
		{
			//Suck inside all the prime components if they exist and obey physics.
			const float SphereRadius = Attract->GetScaledSphereRadius();
			const float ForceStrength = -2500; // +ve to push everything away, -ve to suck inside.

			PrimeComponent->AddRadialForce(GetActorLocation(), SphereRadius, ForceStrength, ERadialImpulseFalloff::RIF_Constant, true);

		}
	}


}