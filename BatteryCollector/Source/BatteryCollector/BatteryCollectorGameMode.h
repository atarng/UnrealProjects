// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

//enum to store the current state of gameplay
UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
    EPlaying UMETA(DisplayName = "Playing"),
    EGameOver UMETA(DisplayName = "Game Over"),
    EWon UMETA(DisplayName = "Won"),
    EUnknown  UMETA(DisplayName = "Unknown")
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

    virtual void Tick(float DeltaTime) override;

    // Returns power needed to win.
    UFUNCTION(BlueprintPure, Category = "Power")
    float GetPowerToWin() const;


    virtual void BeginPlay() override;


    UFUNCTION(BlueprintPure, Category = "Power")
    EBatteryPlayState GetCurrentState() const;

    void SetCurrentState(EBatteryPlayState NewState);

protected:

    // The rate at which the character loses power.
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtect = "true"))
    float DecayRate;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtect = "true"))
    float PowerToWin;

    // The widget class to use for our HUD screen
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtect = "true"))
    TSubclassOf<class UUserWidget> HUDWidgetClass;

    // The instance of our HUD
    UPROPERTY()
    class UUserWidget* CurrentWidget;

private:
    // Keeps track of current play state
    EBatteryPlayState CurrentState;

    // array for our spawn volume actors.
    TArray<class ASpawnVolume*> SpawnVolumeActors;

    // Handle any function calls that rely upon 
    void HandleNewState(EBatteryPlayState NewState);
};