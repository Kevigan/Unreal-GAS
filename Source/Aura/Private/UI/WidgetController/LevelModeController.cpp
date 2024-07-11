// Artic Mango Gaming.


#include "UI/WidgetController/LevelModeController.h"

void ULevelModeController::SetQuestText(FString QuestText)
{
	OnQuestText.Broadcast(QuestText);
}

void ULevelModeController::SetQuestValue(int32 QuestValue)
{
	OnQuestValueChanged.Broadcast(QuestValue);
}
