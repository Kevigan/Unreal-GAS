// Artic Mango Gaming.


#include "Game/AuraGameModeBase.h"
#include "EngineUtils.h"
#include "Aura/AuraLogChannels.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "Interaction/SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/MVVM_LoadSlot.h"
#include "Game/LoadScreenSaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "../Plugins/Runtime/ModelViewViewModel/Source/ModelViewViewModel/Public/MVVMViewModelBase.h"
#include "Game/AuraGameInstance.h"
#include "GameFramework/Character.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"

void AAuraGameModeBase::BeginPlay()
{
	Super::BeginPlay();
	Maps.Add(DefaultMapName, DefaultMap);
	SavedDelegate.BindUObject(this, &AAuraGameModeBase::SaveSlotCallBack);
	LoadedDelegate.BindUObject(this, &AAuraGameModeBase::LoadSlotCallBack);
	LoadSlotData();
}

void AAuraGameModeBase::LoadSlotData()
{

	if (UGameplayStatics::DoesSaveGameExist(NewLoadSlotName, NewSlotIndex))
	{
		UGameplayStatics::AsyncLoadGameFromSlot(NewLoadSlotName, NewSlotIndex, LoadedDelegate);
	}
	else
	{
		NewSaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
		NewLoadScreenSaveGame = Cast<ULoadScreenSaveGame>(NewSaveGameObject);
		NewLoadScreenSaveGame->SavedLoadoutAbilities = StartupAbilities;
		UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
		AuraGameInstance->SaveData->SavedLoadoutAbilities = StartupAbilities;
		AuraGameInstance->bIsFirstLoad = true;
		SaveSlotData();
	}
}

void AAuraGameModeBase::SaveSlotData()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	NewLoadScreenSaveGame->AttributePoints = AuraGameInstance->SaveData->AttributePoints;
	NewLoadScreenSaveGame->Strength = AuraGameInstance->SaveData->Strength;
	NewLoadScreenSaveGame->Intelligence = AuraGameInstance->SaveData->Intelligence;
	NewLoadScreenSaveGame->Resilience = AuraGameInstance->SaveData->Resilience;
	NewLoadScreenSaveGame->Vigor = AuraGameInstance->SaveData->Vigor;
	NewLoadScreenSaveGame->SavedLoadoutAbilities.Empty();
	NewLoadScreenSaveGame->SavedLoadoutAbilities = AuraGameInstance->SaveData->SavedLoadoutAbilities;
	NewLoadScreenSaveGame->SecondSavedLoadoutAbilities = AuraGameInstance->SaveData->SecondSavedLoadoutAbilities;
	NewLoadScreenSaveGame->bNewSpellUnlocked = AuraGameInstance->bNewSpellUnlocked;
	NewLoadScreenSaveGame->AllSavedAbilities = AuraGameInstance->SaveData->AllSavedAbilities;

	//AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	NewLoadScreenSaveGame->XP = AuraGameInstance->PlayerXP;
	NewLoadScreenSaveGame->PlayerLevel = AuraGameInstance->PlayerLevel;

	UGameplayStatics::AsyncSaveGameToSlot(NewLoadScreenSaveGame, NewLoadSlotName, NewSlotIndex, SavedDelegate);
}

void AAuraGameModeBase::SaveSlotCallBack(const FString& Name, const int32 Index, bool InBool)
{

}

void AAuraGameModeBase::LoadSlotCallBack(const FString& Name, const int32 Index, USaveGame* InSaveGame)
{
	NewSaveGameObject = InSaveGame;
	NewLoadScreenSaveGame = Cast<ULoadScreenSaveGame>(NewSaveGameObject);

	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	AuraGameInstance->SaveData->AttributePoints = NewLoadScreenSaveGame->AttributePoints;
	AuraGameInstance->SaveData->Strength = NewLoadScreenSaveGame->Strength;
	AuraGameInstance->SaveData->Intelligence = NewLoadScreenSaveGame->Intelligence;
	AuraGameInstance->SaveData->Resilience = NewLoadScreenSaveGame->Resilience;
	AuraGameInstance->SaveData->Vigor = NewLoadScreenSaveGame->Vigor;
	AuraGameInstance->SaveData->SavedLoadoutAbilities = NewLoadScreenSaveGame->SavedLoadoutAbilities;
	AuraGameInstance->SaveData->SecondSavedLoadoutAbilities = NewLoadScreenSaveGame->SecondSavedLoadoutAbilities;
	AuraGameInstance->SaveData->AllSavedAbilities = NewLoadScreenSaveGame->AllSavedAbilities;
	AuraGameInstance->PlayerXP = NewLoadScreenSaveGame->XP;
	AuraGameInstance->PlayerLevel = NewLoadScreenSaveGame->PlayerLevel;
	AuraGameInstance->bNewSpellUnlocked = NewLoadScreenSaveGame->bNewSpellUnlocked;

	//SetUnlockedSpell();
	//AuraGameInstance->SaveData->XP = NewLoadScreenSaveGame->XP;
}

void AAuraGameModeBase::CreateSaveGameCallBack()
{

}

void AAuraGameModeBase::UpgradeAttribute(EAttribute Attribute)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	if (AuraGameInstance->SaveData->AttributePoints < 1) return;
	switch (Attribute)
	{
	case EAttribute::Intelligence:
		AuraGameInstance->SaveData->Intelligence++;
		AuraGameInstance->TempIntelligence++;
		EnableButton(EAttribute::Intelligence, true);
		break;
	case EAttribute::Resillience:
		AuraGameInstance->SaveData->Resilience++;
		AuraGameInstance->TempResilience++;
		EnableButton(EAttribute::Resillience, true);
		break;
	case EAttribute::Strength:
		AuraGameInstance->SaveData->Strength++;
		AuraGameInstance->TempStrength++;
		EnableButton(EAttribute::Strength, true);
		break;
	case EAttribute::Vigor:
		AuraGameInstance->SaveData->Vigor++;
		AuraGameInstance->TempVigor++;
		EnableButton(EAttribute::Vigor, true);
		break;
	}
	AuraGameInstance->SaveData->AttributePoints--;
	if (AuraGameInstance->SaveData->AttributePoints < 1) OnAttributePointsEmpty.Broadcast(false);
	UpdateValues();
}

void AAuraGameModeBase::DowngradeAttribute(EAttribute Attribute)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	switch (Attribute)
	{
	case EAttribute::Intelligence:
		AuraGameInstance->SaveData->Intelligence--;
		AuraGameInstance->TempIntelligence--;
		if (AuraGameInstance->TempIntelligence < 1) EnableButton(EAttribute::Intelligence, false);
		break;
	case EAttribute::Resillience:
		AuraGameInstance->SaveData->Resilience--;
		AuraGameInstance->TempResilience--;
		if (AuraGameInstance->TempResilience < 1) EnableButton(EAttribute::Resillience, false);
		break;
	case EAttribute::Strength:
		AuraGameInstance->SaveData->Strength--;
		AuraGameInstance->TempStrength--;
		if (AuraGameInstance->TempStrength < 1) EnableButton(EAttribute::Strength, false);
		break;
	case EAttribute::Vigor:
		AuraGameInstance->SaveData->Vigor--;
		AuraGameInstance->TempVigor--;
		if (AuraGameInstance->TempVigor < 1) EnableButton(EAttribute::Vigor, false);
		break;
	}
	AuraGameInstance->SaveData->AttributePoints++;
	if (AuraGameInstance->SaveData->AttributePoints > 0) OnAttributePointsEmpty.Broadcast(true);
	UpdateValues();;
}

void AAuraGameModeBase::ResetAllAttributesToDefault()
{
	NewLoadScreenSaveGame->ResetToDefault();
	UGameplayStatics::AsyncSaveGameToSlot(NewLoadScreenSaveGame, NewLoadSlotName, NewSlotIndex, SavedDelegate);
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	AuraGameInstance->SaveData->AttributePoints = NewLoadScreenSaveGame->AttributePoints;
	AuraGameInstance->SaveData->Strength = NewLoadScreenSaveGame->Strength;
	AuraGameInstance->SaveData->Intelligence = NewLoadScreenSaveGame->Intelligence;
	AuraGameInstance->SaveData->Resilience = NewLoadScreenSaveGame->Resilience;
	AuraGameInstance->SaveData->Vigor = NewLoadScreenSaveGame->Vigor;
	AuraGameInstance->ResetTempValues();
	UpdateValues();
}

void AAuraGameModeBase::SaveSpells(TArray<FSavedAbility> InSavedAbilities, TArray<FSavedAbility> InSecondSavedAbilities)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	AuraGameInstance->SaveData->SavedLoadoutAbilities = InSavedAbilities;
	AuraGameInstance->SaveData->SecondSavedLoadoutAbilities = InSecondSavedAbilities;
}

TArray<FSavedAbility> AAuraGameModeBase::GetSavedAbilities()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	return AuraGameInstance->SaveData->SavedLoadoutAbilities;
}

TArray<FSavedAbility> AAuraGameModeBase::GetSecondSavedAbilities()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	return AuraGameInstance->SaveData->SecondSavedLoadoutAbilities;
}

void AAuraGameModeBase::UpgradeSpell(const FGameplayTag& AbilityTag, UAbilitySystemComponent* ASC)
{
	UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(ASC);
	AuraASC->ServerSpendSpellPoint(AbilityTag);
}

void AAuraGameModeBase::ApplyEffectToActor(TSubclassOf<UGameplayEffect> InstantGameplayEffectClass, AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass)
{
	if (TargetActor->ActorHasTag(FName("Enemy"))) return;
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	if (TargetASC == nullptr) return;

	check(GameplayEffectClass);
	FGameplayEffectContextHandle EffectContextHandle = TargetASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle EffectSpecHandle = TargetASC->MakeOutgoingSpec(GameplayEffectClass, 1.f, EffectContextHandle);
	const FActiveGameplayEffectHandle ActiveEffectHandle = TargetASC->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());
}

//void AAuraGameModeBase::SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex)
//{
//	if (UGameplayStatics::DoesSaveGameExist(LoadSlot->LoadSlotName, SlotIndex))
//	{
//		UGameplayStatics::DeleteGameInSlot(LoadSlot->LoadSlotName, SlotIndex);
//	}
//	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
//	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
//	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
//	LoadScreenSaveGame->SaveSlotStatus = Taken;
//	//LoadScreenSaveGame->MapName = LoadSlot->GetMapName();
//	//LoadScreenSaveGame->MapAssetName = LoadSlot->MapAssetName;
//	//LoadScreenSaveGame->PlayerStartTag = LoadSlot->PlayerStartTag;
//
//	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, LoadSlot->LoadSlotName, SlotIndex);
//}

ULoadScreenSaveGame* AAuraGameModeBase::GetSaveSlotData(const FString& SlotName, int32 SLotIndex) const
{
	USaveGame* SaveGameObject = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SLotIndex))
	{
		SaveGameObject = UGameplayStatics::LoadGameFromSlot(SlotName, SLotIndex);
	}
	else
	{
		SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	}
	ULoadScreenSaveGame* LoadScreenSaveGame = Cast<ULoadScreenSaveGame>(SaveGameObject);
	return LoadScreenSaveGame;
}

void AAuraGameModeBase::DeleteSlot(const FString& SlotName, int32 SlotIndex)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}
}

ULoadScreenSaveGame* AAuraGameModeBase::RetrieveInGameSaveData()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = NewLoadSlotName/*AuraGameInstance->LoadSlotName*/;
	const int32 InGameLoadSlotIndex = NewSlotIndex/*AuraGameInstance->LoadSlotIndex*/;

	return GetSaveSlotData(InGameLoadSlotName, InGameLoadSlotIndex);
}

void AAuraGameModeBase::SaveInGameProgressData(ULoadScreenSaveGame* SaveObject)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	const FString InGameLoadSlotName = AuraGameInstance->LoadSlotName;
	const int32 InGameLoadSlotIndex = AuraGameInstance->LoadSlotIndex;
	//AuraGameInstance->PlayerStartTag = SaveObject->PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveObject, InGameLoadSlotName, InGameLoadSlotIndex);
}

//void AAuraGameModeBase::SaveWorldState(UWorld* World, const FString& DestinationMapAssetName) const
//{
//	FString WorldName = World->GetMapName();
//	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);
//
//	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
//	check(AuraGI);
//
//	if (ULoadScreenSaveGame* SaveGame = GetSaveSlotData(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
//	{
//		if (DestinationMapAssetName != FString(""))
//		{
//			SaveGame->MapAssetName = DestinationMapAssetName;
//			SaveGame->MapName = GetMapNameFromMapAssetName(DestinationMapAssetName);
//		}
//		if (!SaveGame->HasMap(WorldName))
//		{
//			FSavedMap NewSavedMap;
//			NewSavedMap.MapAssetName = WorldName;
//			SaveGame->SavedMaps.Add(NewSavedMap);
//		}
//
//		FSavedMap SavedMap = SaveGame->GetSavedMapWithMapName(WorldName);
//		SavedMap.SavedActors.Empty(); // clear it out, we'll fill it in with "actors"
//
//		for (FActorIterator It(World); It; ++It)
//		{
//			AActor* Actor = *It;
//
//			if (!IsValid(Actor) || !Actor->Implements<USaveInterface>()) continue;
//
//			FSavedActor SavedActor;
//			SavedActor.ActorName = Actor->GetFName();
//			SavedActor.Transform = Actor->GetTransform();
//
//			FMemoryWriter MemoryWriter(SavedActor.Bytes);
//
//			FObjectAndNameAsStringProxyArchive Archive(MemoryWriter, true);
//			Archive.ArIsSaveGame = true;
//
//			Actor->Serialize(Archive);
//
//			SavedMap.SavedActors.AddUnique(SavedActor);
//		}
//
//		for (FSavedMap& MapToReplace : SaveGame->SavedMaps)
//		{
//			if (MapToReplace.MapAssetName == WorldName)
//			{
//				MapToReplace = SavedMap;
//			}
//		}
//		UGameplayStatics::SaveGameToSlot(SaveGame, AuraGI->LoadSlotName, AuraGI->LoadSlotIndex);
//	}
//}

FString AAuraGameModeBase::GetMapNameFromMapAssetName(const FString& MapAssetName) const
{
	for (auto& Map : Maps)
	{
		if (Map.Value.ToSoftObjectPath().GetAssetName() == MapAssetName)
		{
			return Map.Key;
		}
	}
	return FString();
}

void AAuraGameModeBase::LoadWorldState(UWorld* World) const
{
	FString WorldName = World->GetMapName();
	WorldName.RemoveFromStart(World->StreamingLevelsPrefix);

	UAuraGameInstance* AuraGI = Cast<UAuraGameInstance>(GetGameInstance());
	check(AuraGI);

	if (UGameplayStatics::DoesSaveGameExist(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex))
	{

		ULoadScreenSaveGame* SaveGame = Cast<ULoadScreenSaveGame>(UGameplayStatics::LoadGameFromSlot(AuraGI->LoadSlotName, AuraGI->LoadSlotIndex));
		if (SaveGame == nullptr)
		{
			UE_LOG(LogAura, Error, TEXT("Failed to load slot"));
			return;
		}

		for (FActorIterator It(World); It; ++It)
		{
			AActor* Actor = *It;

			if (!Actor->Implements<USaveInterface>()) continue;

			for (FSavedActor SavedActor : SaveGame->GetSavedMapWithMapName(WorldName).SavedActors)
			{
				if (SavedActor.ActorName == Actor->GetFName())
				{
					if (ISaveInterface::Execute_ShouldLoadTransform(Actor))
					{
						Actor->SetActorTransform(SavedActor.Transform);
					}

					FMemoryReader MemoryReader(SavedActor.Bytes);

					FObjectAndNameAsStringProxyArchive Archive(MemoryReader, true);
					Archive.ArIsSaveGame = true;
					Actor->Serialize(Archive); // converts binary bytes back into variables

					ISaveInterface::Execute_LoadActor(Actor);
				}
			}
		}
	}
}

void AAuraGameModeBase::TravelToMap(UMVVM_LoadSlot* Slot)
{
	const FString SlotName = Slot->LoadSlotName;
	const int32 SlotIndex = Slot->SlotIndex;

	UGameplayStatics::OpenLevelBySoftObjectPtr(Slot, Maps.FindChecked(Slot->GetMapName()));
}

AActor* AAuraGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Actors);
	if (Actors.Num() > 0)
	{
		AActor* SelectedActor = Actors[0];
		for (AActor* Actor : Actors)
		{
			if (APlayerStart* PlayerStart = Cast<APlayerStart>(Actor))
			{
				if (PlayerStart->PlayerStartTag == AuraGameInstance->PlayerStartTag)
				{
					SelectedActor = PlayerStart;
					break;
				}
			}
		}
		return SelectedActor;
	}
	return nullptr;
}

void AAuraGameModeBase::PlayerDied(ACharacter* DeadCharacter)
{
	ULoadScreenSaveGame* SaveGame = RetrieveInGameSaveData();
	if (!IsValid(SaveGame)) return;

	//UGameplayStatics::OpenLevel(DeadCharacter, FName(SaveGame->MapAssetName));
}

void AAuraGameModeBase::SetUnlockedSpell()
{
	UAuraGameInstance* AuraGameInstance = Cast<UAuraGameInstance>(GetGameInstance());
	for (FAuraAbilityInfo AInfo : AbilityInfo->AbilityInformation)
	{
		if (AInfo.LevelRequirement <= AuraGameInstance->PlayerLevel)
		{
			AuraGameInstance->UnlockedSkills.Add(AInfo.AbilityTag);
		}
	}
}
