// Artic Mango Gaming.


#include "UI/Widget/LevelInfoWidget.h"
#include "UI/WidgetController/LevelModeController.h"

void ULevelInfoWidget::SetLevelModeWidgetController(ULevelModeController* InLevelModeWidgetController)
{
	if (InLevelModeWidgetController)
	{
		LevelModeWidgetController = InLevelModeWidgetController;
	}
}
