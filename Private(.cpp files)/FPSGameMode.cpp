// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::MissionComplete(APawn *InstigatorPawn)
{
	if (InstigatorPawn)
	{
		InstigatorPawn->DisableInput(NULL);
		
		

		TArray<AActor*> ReturnedActors;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatingViewpointClass, ReturnedActors);


		//Change ViewTarget if any new actor found
		if (ReturnedActors.Num() > 0)
		{
			AActor *NextViewTarget = ReturnedActors[0];

			APlayerController *PC = Cast<APlayerController>(InstigatorPawn->GetController());

			if (PC)
			{
				PC->SetViewTargetWithBlend(NextViewTarget, 0.625, EViewTargetBlendFunction::VTBlend_Cubic);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("SpectatingVeiwpointClass is NULL, please update Gamemode classwith valid subclass. Cannot change spectating ViewTarget"));
		}
		
	}

	OnMissionCompleted(InstigatorPawn);



}
