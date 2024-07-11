// Artic Mango Gaming.


#include "AbilitySystem/Data/AttributeInfo.h"
#include "Aura/AuraLogChannels.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFOund) const
{
	for (const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag == AttributeTag) // Also works --> Info.AttributeTag.MatchesTagExact((AttributeTag));
		{
			return Info;
		}
	}

	if (bLogNotFOund)
	{
		UE_LOG(LogAura, Error, TEXT("Can´t find Info for AttributeTag [%s] on AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}
