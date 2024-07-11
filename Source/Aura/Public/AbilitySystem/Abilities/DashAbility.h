// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/AuraGameplayAbility.h"
#include "DashAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UDashAbility : public UAuraGameplayAbility
{
	GENERATED_BODY()
	
public:
	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
};
