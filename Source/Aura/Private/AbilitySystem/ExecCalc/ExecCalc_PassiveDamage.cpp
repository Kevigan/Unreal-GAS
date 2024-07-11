// Artic Mango Gaming.


#include "AbilitySystem/ExecCalc/ExecCalc_PassiveDamage.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "GameplayEffect.h"
#include "AuraGameplayTags.h"

UExecCalc_PassiveDamage::UExecCalc_PassiveDamage()
{
}

void UExecCalc_PassiveDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();
	float Damage = 0.f;
	Damage = Spec.GetSetByCallerMagnitude(FAuraGameplayTags::Get().Damage_Passive, false);
	const FGameplayModifierEvaluatedData EvaluatedData(UAuraAttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Additive, Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
