// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtractionZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
AExtractionZone::AExtractionZone()
{

	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComponent"));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapComponent->SetCollisionResponseToChannel(ECC_Pawn,ECR_Overlap);
	OverlapComponent->SetBoxExtent(FVector(200.0f));
	OverlapComponent->SetHiddenInGame(false);
	RootComponent = OverlapComponent;

	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &AExtractionZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(200.0f, 200.0f, 200.0f);

	DecalComp->SetupAttachment(RootComponent);

}



void AExtractionZone::HandleOverlap(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	
	AFPSCharacter *MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn == NULL) //&& 
		return;
	if((MyPawn->bIsCarryingObjective))
	{
		UE_LOG(LogTemp, Log, TEXT("Overlapped with extraction zone !"));
		AFPSGameMode *GM = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
			GM->MissionComplete(MyPawn);
	}
	else
	{
		UGameplayStatics::PlaySound2D(this, ObjectiveMissingSound);
	}
}



