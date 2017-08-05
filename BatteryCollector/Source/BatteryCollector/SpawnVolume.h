// Written by Alfred Tarng of AtRNGames. 2017

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class BATTERYCOLLECTOR_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // FORCEINLINE and UFUNCTION not compatible
    // Returns the where to spawn subobject.
    FORCEINLINE class UBoxComponent* GetWhereToSpawn() const { return WhereToSpawn; }

    UFUNCTION(BlueprintPure, Category = "Spawning")
    FVector GetRandomPointInVolume();

    // Toggles Spawning Behavior.
    UFUNCTION(BlueprintCallable, Category = "Spawning")
    void SetSpawningActive(bool bShouldSpawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    // The pickup to spawn
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<class APickup> WhatToSpawn;

    FTimerHandle SpawnTimer;

    /** Minimum spawn delay **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnDelayRangeLow;

    /** Maximum spawn delay **/
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawning")
    float SpawnDelayRangeHigh;

private:	

    // Box Component to specify where pickups should be spawned.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Spawning", meta = (AllowPrivateAccess = "true"))
    class UBoxComponent* WhereToSpawn;
	
    // Handle Spawning a new Pickup
    void SpawnPickup();

    // The Current Spawn Delay
    float SpawnDelay;

};
