// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "LevelModeController.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestText, FString, QuestText);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestValueChanged, int32, QuestValue);

/**
 *
 */
UCLASS(BlueprintType, Blueprintable)
class AURA_API ULevelModeController : public UObject
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, Category = "LevelMode")
		FOnQuestText OnQuestText;
	UPROPERTY(BlueprintAssignable, Category = "LevelMode")
		FOnQuestValueChanged OnQuestValueChanged;

	void SetQuestText(FString QuestText);
	void SetQuestValue(int32 QuestValue);

};
