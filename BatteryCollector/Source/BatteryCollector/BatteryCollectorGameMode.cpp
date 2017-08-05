// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "BatteryCollectorGameMode.h"

#include "BatteryCollector.h"

#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "BatteryCollectorCharacter.h"
#include "SpawnVolume.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
{
    // have to add this otherwise it won't call Tick(deltaTime).
    PrimaryActorTick.bCanEverTick = true;

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

    // base decay rate
    DecayRate = 0.01f;
}

void ABatteryCollectorGameMode::BeginPlay()
{
    Super::BeginPlay();

    // Find all spawn volume actors
    TArray<AActor*> FoundActors;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);
    for (auto Actor : FoundActors) {
        ASpawnVolume* SpawnVolumeActor = Cast<ASpawnVolume>(Actor);
        if (SpawnVolumeActor) {
            SpawnVolumeActors.AddUnique(SpawnVolumeActor);
        }
    }

    SetCurrentState(EBatteryPlayState::EPlaying);

    // set the score to beat
    ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (MyCharacter)
    {
        PowerToWin = MyCharacter->GetInitialPower() * 1.25f;
    }

    if (HUDWidgetClass != nullptr) {
        CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
        if (CurrentWidget != nullptr) {
            CurrentWidget->AddToViewport();
        }
    }


}


void ABatteryCollectorGameMode::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    // get character, make sure it is battery collector.
    ABatteryCollectorCharacter* MyCharacter = Cast<ABatteryCollectorCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
    if (MyCharacter)
    {
        // If characters power is positive, decrease characters power based on decay rate.
        if (MyCharacter->GetCurrentPower() > PowerToWin) {
            SetCurrentState(EBatteryPlayState::EWon);
        }
        else if (MyCharacter->GetCurrentPower() > 0) {
            MyCharacter->UpdatePower((-DecayRate * MyCharacter->GetInitialPower()) * DeltaTime);
        }
        else{
            SetCurrentState(EBatteryPlayState::EGameOver);
        }
    }
}


float ABatteryCollectorGameMode::GetPowerToWin() const {
    return PowerToWin;
}


EBatteryPlayState ABatteryCollectorGameMode::GetCurrentState() const {
    return CurrentState;
}

void ABatteryCollectorGameMode::SetCurrentState(EBatteryPlayState NewState) {
    CurrentState = NewState;
    HandleNewState(NewState);
}

void ABatteryCollectorGameMode::HandleNewState(EBatteryPlayState NewState) {
    APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    ACharacter* MyCharacter = UGameplayStatics::GetPlayerCharacter(this, 0);

    switch (NewState) {
    // if game is playing
    case EBatteryPlayState::EWon:
        // spawn volumes inactive
        for (ASpawnVolume* Volume : SpawnVolumeActors) {
            Volume->SetSpawningActive(false);
        }
        break;
    case EBatteryPlayState::EGameOver:
        // spawn volumes inactive
        for (ASpawnVolume* Volume : SpawnVolumeActors) {
            Volume->SetSpawningActive(false);
        }

        // block character input
        if (PlayerController) {
            PlayerController->SetCinematicMode(true, false, false, true, true);
        }

        // ragdoll character
        if (MyCharacter) {
            MyCharacter->GetMesh()->SetSimulatePhysics(true);
            MyCharacter->GetMovementComponent()->MovementState.bCanJump = false;
        }


        break;
    case EBatteryPlayState::EPlaying:
        // spawn volumes active
        for (ASpawnVolume* Volume : SpawnVolumeActors) {
            Volume->SetSpawningActive(true);
        }

        break;
    case EBatteryPlayState::EUnknown:
    default:
        break;
    }
}