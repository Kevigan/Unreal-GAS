// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_Max_Mana.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UMMC_Max_Mana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:
	UMMC_Max_Mana();

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDef;
};
