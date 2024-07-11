// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelInfoWidget.generated.h"

class ULevelModeController;

USTRUCT(BlueprintType)
struct FQuestInfo
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadOnly)
		FString QuestMessage = FString();

	UPROPERTY(BlueprintReadOnly)
		int32 QuestValue = 0;
};


/**
 *
 */
UCLASS()
class AURA_API ULevelInfoWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void SetLevelModeWidgetController(ULevelModeController* InLevelModeWidgetController);

protected:

	UPROPERTY(BlueprintReadOnly)
		ULevelModeController* LevelModeWidgetController;

	UFUNCTION(BlueprintImplementableEvent)
		void OnQuestValueChanged(int32 InQuestValue);
};
