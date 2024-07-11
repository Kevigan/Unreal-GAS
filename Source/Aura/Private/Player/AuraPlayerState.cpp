// Artic Mango Gaming.


#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"
#include "Game/AuraGameInstance.h"
#include "Game/AuraGameModeBase.h"

AAuraPlayerState::AAuraPlayerState()
{
	NetUpdateFrequency = 100.f;

	AbilitySystemComponent = CreateDefaultSubobject<UAuraAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UAuraAttributeSet>("AttributeSet");
}

void AAuraPlayerState::BeginPlay()
{
	
}

void AAuraPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AAuraPlayerState, Level);
	DOREPLIFETIME(AAuraPlayerState, XP);
	DOREPLIFETIME(AAuraPlayerState, AttributePoints);
	DOREPLIFETIME(AAuraPlayerState, SpellPoints);
}

UAbilitySystemComponent* AAuraPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AAuraPlayerState::AddToLevel(int32 InLevel)
{
	Level += InLevel;
	OnLevelChangedDelegate.Broadcast(Level, true);
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (AuraGameInstance)
	{
		AuraGameInstance->PlayerLevel += InLevel;
	}
}

void AAuraPlayerState::AddToXP(int32 InXP)
{
	XP += InXP;
	OnXPChangedDelegate.Broadcast(XP);
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (AuraGameInstance)
	{
		AuraGameInstance->PlayerXP += InXP;
	}
}

void AAuraPlayerState::AddToUpgradePointsXP(int32 InXP)
{
	UpgradePointsXP += InXP;
	if (UpgradePointsXP >= UpgradePointsXP_Needed)
	{
		UpgradePointsXP = UpgradePointsXP - UpgradePointsXP_Needed;
		UpgradePoints++;
		OnUpgradePointsAddedDelegate.Broadcast(UpgradePoints);
	}
	OnUpgradePointsXPChangedDelegate.Broadcast(UpgradePointsXP);
}

void AAuraPlayerState::AddToAttributePoints(int32 InPoints)
{
	//AttributePoints += InPoints;
	//OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (AuraGameInstance)
	{
		AuraGameInstance->SaveData->AttributePoints += InPoints;
		AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
		if (AuraGM)
		{
			AuraGM->SaveSlotData();
		}
	}
}

void AAuraPlayerState::AddToSpellPoints(int32 InPoints)
{
	SpellPoints += InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level, false);
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (AuraGameInstance)
	{
		//AuraGameInstance->PlayerLevel = InLevel;
	}
}

void AAuraPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(UGameplayStatics::GetGameInstance(this));
	if (AuraGameInstance)
	{
		//AuraGameInstance->PlayerXP = InXP;
	}
}

void AAuraPlayerState::SetAttributePoints(int32 InPoints)
{
	AttributePoints = InPoints;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::SetSpellPoints(int32 InPoints)
{
	SpellPoints = InPoints;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}

void AAuraPlayerState::SetUpgradePoint(int32 InPoints)
{
	UpgradePoints += InPoints;
	OnUpgradePointsRemovedDelegate.Broadcast(UpgradePoints);
}

void AAuraPlayerState::SetCooldownCOEFF(float InCooldownReduce)
{
	float PercentageAdded = InCooldownReduce / 100.f;
	CooldownCOEFF += PercentageAdded;
}

void AAuraPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void AAuraPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void AAuraPlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoints);
}

void AAuraPlayerState::OnRep_SpellPoints(int32 OldSpellPoints)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoints);
}
