// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameplayTagContainer.h"
#include "LoadScreenSaveGame.generated.h"

class UGameplayAbility;

UENUM(BlueprintType)
enum ESaveSlotStatus
{
	Vacant,
	EnterName,
	Taken
};

USTRUCT(BlueprintType)
struct FSavedAbility
{
	GENERATED_BODY()

		UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = ClassDefaults)
		TSubclassOf<UGameplayAbility> GameplayAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FGameplayTag AbilityTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FGameplayTag AbilityStatus = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FGameplayTag AbilitySlot = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FGameplayTag AbilityType = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 AbilityLevel = 1;
};

inline bool operator==(const FSavedAbility& Left, const FSavedAbility& Right)
{
	return Left.AbilityTag.MatchesTagExact(Right.AbilityTag);
}

USTRUCT()
struct FSavedActor
{
	GENERATED_BODY()

		UPROPERTY()
		FName ActorName = FName();

	UPROPERTY()
		FTransform Transform = FTransform();

	// Serialized variables from the Actor - only those marked with SaveGame specifier
	UPROPERTY()
		TArray<uint8> Bytes;
};

inline bool operator==(const FSavedActor& Left, const FSavedActor& Right)
{
	return Left.ActorName == Right.ActorName;
}

USTRUCT()
struct FSavedMap
{
	GENERATED_BODY()

		UPROPERTY()
		FString MapAssetName = FString();

	UPROPERTY()
		TArray<FSavedActor> SavedActors;
};

/**
 *
 */
UCLASS()
class AURA_API ULoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
		FString SlotName = FString();

	UPROPERTY()
		int32 SlotIndex = 0;

	UPROPERTY()
		FString PlayerName = FString("Default Name");

	/*UPROPERTY()
		FString MapName = FString("Default Map Name");*/

	/*UPROPERTY()
		FString MapAssetName = FString("Default Map Asset Name");*/

	/*UPROPERTY()
		FName PlayerStartTag;*/

	UPROPERTY()
		TEnumAsByte<ESaveSlotStatus> SaveSlotStatus = Vacant;

	UPROPERTY()
		bool bFirstTimeLoadIn = true;

	/* Player */
	UPROPERTY()
		int32 PlayerLevel = 1;

	UPROPERTY()
		int32 XP = 0;

	UPROPERTY()
		bool bNewSpellUnlocked = false;

	/*UPROPERTY()
		int32 SpellPoints = 0;*/

	UPROPERTY(BlueprintReadWrite)
		int32 AttributePoints = 0;

	/* Attributes */
	UPROPERTY(BlueprintReadWrite)
		float Strength = 10;
		float DefaultStrength = 10;

	UPROPERTY(BlueprintReadWrite)
		float Intelligence = 12;
		float DefaultIntelligence = 12;

	UPROPERTY(BlueprintReadWrite)
		float Resilience = 9;
		float DefaultResilience = 9;

	UPROPERTY(BlueprintReadWrite)
		float Vigor = 14;
		float DefaultVigor = 14;

	/* Abilities */
	UPROPERTY(BlueprintReadOnly)
		TArray<FSavedAbility> SavedLoadoutAbilities;

	UPROPERTY(BlueprintReadOnly)
		TArray<FSavedAbility> SecondSavedLoadoutAbilities;

	UPROPERTY(BlueprintReadWrite)
		TArray<FSavedAbility> AllSavedAbilities;

	UPROPERTY()
		TArray<FSavedMap> SavedMaps;

	FSavedMap GetSavedMapWithMapName(const FString& InMapName);
	bool HasMap(const FString& InMapName);
	void ResetToDefault();
};
