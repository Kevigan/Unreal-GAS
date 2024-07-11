// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "LoadScreenSaveGame.h"
#include "AuraGameInstance.generated.h"


/**
 *
 */
UCLASS()
class AURA_API UAuraGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		bool bIsFirstLoad = false;

	UPROPERTY()
		FName PlayerStartTag = FName();

	UPROPERTY()
		FString LoadSlotName = FString();

	UPROPERTY()
		int32 LoadSlotIndex = 0;

	UPROPERTY(BlueprintReadWrite)
		ULoadScreenSaveGame* SaveData = NewObject<ULoadScreenSaveGame>();

	UPROPERTY()
		float TempStrength = 0;

	UPROPERTY()
		float TempIntelligence = 0;

	UPROPERTY()
		float TempResilience = 0;

	UPROPERTY()
		float TempVigor = 0;

	void ResetTempValues();

	UPROPERTY(BlueprintReadOnly)
		int32 PlayerXP = 0;

	UPROPERTY(BlueprintReadOnly)
		int32 PlayerLevel = 1;

	UPROPERTY(BlueprintReadWrite)
		bool bNewSpellUnlocked = false;

	/* Spell Level Getters */
	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetFireBlast_Level() const { return FireBlast_Level; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetFireBolt_Level() const { return FireBolt_Level; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetArcaneShards_Level() const { return ArcaneShards_Level; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetLifeSiphon_Level() const { return LifeSiphon_Level; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetManaSiphon_Level() const { return ManaSiphon_Level; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetHaloOfProtection_Level() const { return HaloOfProtection_Level; }

	UFUNCTION(BlueprintCallable)
		FORCEINLINE float GetDash_Level() const { return Dash_level; }
	/* Spell Level Getters End */

	/* Spell Level Setters */
	UFUNCTION(BlueprintCallable)
		void SetFireBlastLevel(int32 InLevel) { FireBlast_Level = InLevel; }
	UFUNCTION(BlueprintCallable)
		void SetFireBoltLevel(int32 InLevel) { FireBolt_Level = InLevel; }
	UFUNCTION(BlueprintCallable)
		void SetArcaneShardsLevel(int32 InLevel) { ArcaneShards_Level = InLevel; }

	UFUNCTION(BlueprintCallable)
		void SetLifeSiphonLevel(int32 InLevel) { LifeSiphon_Level = InLevel; }
	UFUNCTION(BlueprintCallable)
		void SetManaSiphonLevel(int32 InLevel) { ManaSiphon_Level = InLevel; }
	UFUNCTION(BlueprintCallable)
		void SetHaloOfProtectionLevel(int32 InLevel) { HaloOfProtection_Level = InLevel; }

	UFUNCTION(BlueprintCallable)
		void SetDashLevel(int32 InLevel) { Dash_level = InLevel; }
	/* Spell Level Setters End */

	UPROPERTY(BlueprintReadOnly)
		TArray<FGameplayTag> UnlockedSkills;


private:
	/* Offensive Spell Levels */
	int32 FireBlast_Level = 1;
	int32 FireBolt_Level = 1;
	int32 ArcaneShards_Level = 1;
	/* Offensive Spell Levels End */

	/* Passive Spell Levels */
	int32 LifeSiphon_Level = 1;
	int32 ManaSiphon_Level = 1;
	int32 HaloOfProtection_Level = 1;
	/* Passive Spell Levels End */

	/* Dash Spell Levels */
	int32 Dash_level = 1;
	/* Dash Spell Levels End */

};
