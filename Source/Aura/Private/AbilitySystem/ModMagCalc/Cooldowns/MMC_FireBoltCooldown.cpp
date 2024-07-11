// Artic Mango Gaming.


#include "AbilitySystem/ModMagCalc/Cooldowns/MMC_FireBoltCooldown.h"
#include "Player/AuraPlayerState.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

float UMMC_FireBoltCooldown::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const UAbilitySystemComponent* OwningAbilitySystemComponent = Spec.GetContext().GetOriginalInstigatorAbilitySystemComponent();
	if (OwningAbilitySystemComponent)
	{
		AActor* Actor = OwningAbilitySystemComponent->GetOwnerActor();
		AAuraPlayerState* PS = Cast<AAuraPlayerState>(OwningAbilitySystemComponent->GetOwnerActor());
		if (PS)
		{
			float FireCD = PS->GetFireBolt_CD();
			float CooldownCoef = PS->GetCooldownCOEFF();
			float NewCD = PS->GetFireBolt_CD() * PS->GetCooldownCOEFF();
			return NewCD;
		}
	}

	return 0.0f;
}
