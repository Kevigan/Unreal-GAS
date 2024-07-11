// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraBeamSpell.h"
#include "Electricute.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UElectricute : public UAuraBeamSpell
{
	GENERATED_BODY()
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
