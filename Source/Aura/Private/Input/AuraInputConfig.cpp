// Artic Mango Gaming.


#include "Input/AuraInputConfig.h"
#include "../Plugins/EnhancedInput/Source/EnhancedInput/Public/InputAction.h"

const UInputAction* UAuraInputConfig::FindAbilityInputActionForTag(const FGameplayTag& InputTag, bool bLogNotFOund) const
{
	for (const FAuraInputAction& Action : AbilityInputActions)
	{
		if (Action.InputAction && Action.InputTag == InputTag)
		{
			return Action.InputAction;
		}
	}

	if (bLogNotFOund)
	{
		UE_LOG(LogTemp, Error, TEXT("Can´t find AbilityInputAction for InputTag [%s], on InputConfig [%s]"), *InputTag.ToString(), *GetNameSafe(this));
	}

	return nullptr;
}

