// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_FireBoltCooldown.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_FireBoltCooldown : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:
	float FireboltCooldown = 1.f;
};
