// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "AuraGameModeBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAttributePointsEmpty, bool, EnableButton);

class UCharacterClassInfo;
class UAuraGameInstance;
class UGameplayEffect;
class UAbilityInfo;
class UMVVM_LoadSlot;
class USaveGame;
class ULoadScreenSaveGame;
class ULootTiers;

UENUM(BlueprintType)
enum class EAttribute : uint8
{
	Strength,
	Resillience,
	Intelligence,
	Vigor
};

/**
 *
 */
UCLASS()
class AURA_API AAuraGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Character Class Defaults")
		TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Character Class Defaults")
		TArray<FSavedAbility> StartupAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Ability Info")
		TObjectPtr<UAbilityInfo> AbilityInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Loot Tiers")
		TObjectPtr<ULootTiers> LootTiers;

	/* New Saving Game*/
	UPROPERTY()
		FString NewLoadSlotName = "AuraSaveGame";

	UPROPERTY()
		int32 NewSlotIndex = 0;

	USaveGame* NewSaveGameObject = nullptr;

	UFUNCTION(BlueprintCallable)
		void LoadSlotData();
	UFUNCTION(BlueprintCallable)
		void SaveSlotData();
	void LoadSlotCallBack(const FString& Name, const int32 Index, USaveGame* InSaveGame);
	void SaveSlotCallBack(const FString& Name, const int32 Index, bool InBool);
	FAsyncSaveGameToSlotDelegate SavedDelegate;
	FAsyncLoadGameFromSlotDelegate LoadedDelegate;
	void CreateSaveGameCallBack();

	UPROPERTY(BlueprintReadWrite);
	APlayerState* PlayerState;

	UPROPERTY(BlueprintReadOnly)
		ULoadScreenSaveGame* NewLoadScreenSaveGame;
	/* New Saving Game End*/

	/* Attributes */
	UFUNCTION(BlueprintCallable)
		void UpgradeAttribute(EAttribute Attribute);

	UFUNCTION(BlueprintCallable)
		void DowngradeAttribute(EAttribute Attribute);

	UFUNCTION(BlueprintCallable)
		void ResetAllAttributesToDefault();

	UFUNCTION(BlueprintImplementableEvent)
		void EnableButton(EAttribute Attribute, bool Enable);

	UFUNCTION(BlueprintImplementableEvent)
		void UpdateValues();

	UPROPERTY(BlueprintAssignable)
		FOnAttributePointsEmpty OnAttributePointsEmpty;
	/* Attributes End */

	/* Spells */
	UFUNCTION(BlueprintCallable)
		void SaveSpells(TArray<FSavedAbility> InSavedAbilities, TArray<FSavedAbility> InSecondSavedAbilities);

	UFUNCTION(BlueprintCallable)
		TArray<FSavedAbility> GetSavedAbilities();

	UFUNCTION(BlueprintCallable)
		TArray<FSavedAbility> GetSecondSavedAbilities();
	/* Spells End */

	/* Upgrade Spell Temporary */
	UFUNCTION(BlueprintCallable)
		void UpgradeSpell(const FGameplayTag& AbilityTag, UAbilitySystemComponent* ASC);
	/* End Upgrade Spell Temporary */

	/* Upgrade Attribute Temporary */
	UFUNCTION(BlueprintCallable)
		void ApplyEffectToActor(TSubclassOf<UGameplayEffect> InstantGameplayEffectClass, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);
	/* End Upgrade Attribute Temporary */

	//void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SLotIndex)const;
	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);
	ULoadScreenSaveGame* RetrieveInGameSaveData();
	void SaveInGameProgressData(ULoadScreenSaveGame* SaveObject);

	//void SaveWorldState(UWorld* World, const FString& DestinationMapAssetName = FString("")) const;
	void LoadWorldState(UWorld* World) const;

	void TravelToMap(UMVVM_LoadSlot* Slot);

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
		FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
		TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
		FName DefaultPlayerStartTag;

	UPROPERTY(EditDefaultsOnly)
		TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	FString GetMapNameFromMapAssetName(const FString& MapAssetName) const;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PlayerDied(ACharacter* DeadCharacter);

	void SetUnlockedSpell();
protected:
	virtual void BeginPlay() override;
};
