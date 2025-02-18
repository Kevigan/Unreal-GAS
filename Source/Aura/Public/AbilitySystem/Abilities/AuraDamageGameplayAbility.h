// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "Interaction/CombatInterface.h"
#include "AuraAbilityTypes.h"
#include "AuraDamageGameplayAbility.generated.h"

class UGameplayEffect;

/**
 *
 */
UCLASS()
class AURA_API UAuraDamageGameplayAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
		void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
		FDamageEffectParams MakeDamageEffectParamsFromClassDefaults(
		AActor* TargetActor = nullptr, 
		FVector InRadialDamageOrigin = FVector::ZeroVector,
		bool bOverrideKnockbackDirection = false,
		FVector KnockbackDirectionOverride = FVector::ZeroVector,
		bool bOverrideDeathImpulse = false,
		FVector DeathImpulseDirectionOverride = FVector::ZeroVector,
		bool bOverridePitch = false,
		float PitchOverride = 0.f
		) const;

	UFUNCTION(BlueprintPure)
		float GetDamageAtLevel() const;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<UGameplayEffect> DamageEffectClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		FGameplayTag DamageType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		FScalableFloat Damage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float DebuffChance = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float DebuffDamage = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float DebuffFrequency = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float DebuffDuration = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float DeathImpulseMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float KnockbackForceMagnitude = 1000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float KnockbackChance = 0.f;

	UFUNCTION(BlueprintPure)
		FTaggedMontage GetRandomTaggedMontageFromArray(const TArray<FTaggedMontage>& TaggedMontages) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		bool bIsRadialDamage = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float RadialDamageInnerRadius = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Damage")
		float RadialDamageOuterRadius = 0.f;
};
