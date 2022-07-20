// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "BatteryCollectorGameMode.generated.h"

// Enum to store current state of gameplay
UENUM(BlueprintType)
enum class EBatteryPlayState : uint8
{
	EPlaying UMETA(DisplayName = "Playing"),
	EGameOver UMETA(Displayname = "Game Over"),
	EWon UMETA(DisplayName = "Won"),
	EUnknown UMETA(DisplayName = "Unknown")
};

UCLASS(minimalapi)
class ABatteryCollectorGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	virtual void Tick(float DeltaTime) override;

	/** Returns power to win - needed for HUD */
	UFUNCTION(BlueprintPure, Category = "Power")
	float GetPowerToWin() const;

	virtual void BeginPlay() override;

	/** Returns the current playing state */
	UFUNCTION(BlueprintPure, Category = "Power")
	EBatteryPlayState GetCurrentState() const;

	/** Sets a new play state */
	void SetCurrentState(EBatteryPlayState NewState);

protected:
	/** The rate at which the character loses power */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float DecayRate;

	/** Power needed to win the game */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	float PowerToWin;

	/** The widget class to use for our HUD screen */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Power", Meta = (BlueprintProtected = "true"))
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	/** The instance of the HUD */
	UPROPERTY()
	class UUserWidget* CurrentWidget;

private:
	/** Keep track of the current playing state */
	EBatteryPlayState CurrentState;

	TArray<class ASpawnVolume*> SpawnVolumeActors;

	/** Handle any function calls that rely upon changing the playing of our game */
	void HandleNewState(EBatteryPlayState NewState);
};



