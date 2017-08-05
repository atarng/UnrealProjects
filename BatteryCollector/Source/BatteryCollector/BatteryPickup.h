// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup.h"
#include "BatteryPickup.generated.h"

/**
 * 
 */
UCLASS()
class BATTERYCOLLECTOR_API ABatteryPickup : public APickup
{
	GENERATED_BODY()
	
public:
    // Sets default values for this actor's properties
    ABatteryPickup();

    // overriding WasCollected function - use implementation b/c blueprint native
    void WasCollected_Implementation() override;

    // Accessor for our batter power
    float GetPower();

protected:
    // Set the amount of power the battery gives to the character
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
    float BatteryPower;
   
private:
	
	
};
