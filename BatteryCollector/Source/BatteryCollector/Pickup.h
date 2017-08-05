// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"


UCLASS()
class BATTERYCOLLECTOR_API APickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

    /** Accessor for Pickup **/
    FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }

    /***
     * BlueprintPure: technically can modifiy (but shouldn't?), runs everytime output is used.
     * Fast, for frequently called functions.
     */
    UFUNCTION(BlueprintPure, Category = "Pickup")
    bool IsActive();

    /***
     * BlueprintCallable: Only runs when input execution is hit. Used for functions not called often.
     */
    UFUNCTION(BlueprintCallable, Category = "Pickup")
    void SetActive(bool NewPickupState);

    // Function to call when the pickup is collected
    UFUNCTION(BlueprintNativeEvent)
    void WasCollected();
    virtual void WasCollected_Implementation();


protected:
    /** True when the pickup can be used, false when pickup is inactive **/
    bool bIsActive;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    /***
     * Macros:
     * 
     */
    UPROPERTY(VisibleAnywhere, BlueprintREadOnly, Category = "Pickup", meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent* PickupMesh; // forward decl of static mesh.
	
};
