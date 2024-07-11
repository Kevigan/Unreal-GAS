// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h"
#include "AuraPlayerState.generated.h"

class UAbilitySystemComponent;
class UAttributeSet;
class ULevelUpInfo;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnPlayerStatChanged, int32 /*StatValue*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelChanged, int32 /*StatValue*/, bool /*bLevelUp*/)

/**
 *
 */
	UCLASS()
	class AURA_API AAuraPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AAuraPlayerState();
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<ULevelUpInfo> LevelUpInfo;

	FOnPlayerStatChanged OnXPChangedDelegate;
	FOnPlayerStatChanged OnUpgradePointsXPChangedDelegate;
	FOnPlayerStatChanged OnUpgradePointsAddedDelegate;
	FOnPlayerStatChanged OnUpgradePointsRemovedDelegate;
	FOnLevelChanged OnLevelChangedDelegate;
	FOnPlayerStatChanged OnAttributePointsChangedDelegate;
	FOnPlayerStatChanged OnSpellPointsChangedDelegate;

	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetPlayerLevel() const { return Level; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetXP() const { return XP; }
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }
	FORCEINLINE int32 GetSpellPoints() const { return SpellPoints; }
	UFUNCTION(BlueprintCallable)
		FORCEINLINE int32 GetUpgradePoints() const { return UpgradePoints; }

	FORCEINLINE float GetMaxManaAddition() const { return MaxManaAddition; }
	FORCEINLINE float GetMaxLifeAddition() const { return MaxLifeAddition; }


	void AddToLevel(int32 InLevel);
	UFUNCTION(BlueprintCallable)
		void AddToAttributePoints(int32 InPoints);

	void AddToXP(int32 InXP);
	UFUNCTION(BlueprintCallable)
		void AddToUpgradePointsXP(int32 InXP);
	void AddToSpellPoints(int32 InPoints);

	void SetLevel(int32 InLevel);
	void SetXP(int32 InXP);
	void SetAttributePoints(int32 InPoints);
	void SetSpellPoints(int32 InPoints);
	UFUNCTION(BlueprintCallable)
		void SetUpgradePoint(int32 InPoints);

	UFUNCTION(BlueprintCallable)
	void SetMaxManaAddition(float InAddition) { MaxManaAddition += InAddition;};

	UFUNCTION(BlueprintCallable)
	void SetMaxLifeAddition(float InAddition) { MaxLifeAddition += InAddition;};

	/* Cooldowns Getters*/

	FORCEINLINE float GetCooldownCOEFF() const { return CooldownCOEFF; }
	FORCEINLINE float GetFireBolt_CD() const { return FireBolt_CD; }
	FORCEINLINE float GetFireBlast_CD() const { return FireBlast_CD; }
	FORCEINLINE float GetDash_CD() const { return Dash_CD; }
	/* Cooldowns Getters End*/



protected:
	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
		TObjectPtr <UAttributeSet> AttributeSet;

	/* Cooldowns */
	UPROPERTY(EditAnywhere, Category = "Cooldowns")
		float CooldownCOEFF = 1.f;

	UPROPERTY(EditAnywhere, Category = "Cooldowns")
		float FireBolt_CD = 1.f;

	UPROPERTY(EditAnywhere, Category = "Cooldowns")
		float FireBlast_CD = 5.f;

	UPROPERTY(EditAnywhere, Category = "Cooldowns")
		float ArcaneShards_CD = 3.f;

	UPROPERTY(EditAnywhere, Category = "Cooldowns")
		float Dash_CD = 3.f;

	/* Cooldowns End*/

	/* Cooldowns Setters */
	UFUNCTION(BlueprintCallable)
		void SetCooldownCOEFF(float InCooldownReduce);
	/* Cooldowns Setters End */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int32 UpgradePointsXP_Needed = 10;

private:
	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_Level)
		int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_XP)
		int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_AttributePoints)
		int32 AttributePoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
		int32 SpellPoints = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
		int32 UpgradePointsXP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_SpellPoints)
		int32 UpgradePoints = 0;

	UPROPERTY()
		float MaxManaAddition = 0.f;

	UPROPERTY()
		float MaxLifeAddition = 0.f;


	UFUNCTION()
		void OnRep_Level(int32 OldLevel);

	UFUNCTION()
		void OnRep_XP(int32 OldXP);

	UFUNCTION()
		void OnRep_AttributePoints(int32 OldAttributePoints);

	UFUNCTION()
		void OnRep_SpellPoints(int32 OldSpellPoints);
};
