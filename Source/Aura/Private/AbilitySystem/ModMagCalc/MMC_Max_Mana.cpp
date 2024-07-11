// Artic Mango Gaming.


#include "AbilitySystem/ModMagCalc/MMC_Max_Mana.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "Player/AuraPlayerState.h"
#include "Kismet/GameplayStatics.h"

UMMC_Max_Mana::UMMC_Max_Mana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_Max_Mana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target
	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluationParameters, Intelligence);
	Intelligence = FMath::Max<float>(Intelligence, 0.f);

	/*int32 PlayerLevel = 1;
	if (Spec.GetContext().GetSourceObject()->Implements<UCombatInterface>())
	{
		PlayerLevel = ICombatInterface::Execute_GetPlayerLevel(Spec.GetContext().GetSourceObject());
	}*/
	float MaxManaAddition = 0.f;
	AAuraPlayerState* AuraPS = Cast<AAuraPlayerState>(UGameplayStatics::GetPlayerState(Spec.GetContext().GetSourceObject(),0));
	if (AuraPS)
	{
		MaxManaAddition += AuraPS->GetMaxManaAddition();
	}

	return 100 + 1.25f * Intelligence + MaxManaAddition;
}
