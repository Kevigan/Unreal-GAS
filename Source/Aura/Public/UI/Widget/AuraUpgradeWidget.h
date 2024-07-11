// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "UI/Widget/AuraUserWidget.h"
#include "AuraUpgradeWidget.generated.h"

/**
 *
 */
UCLASS()
class AURA_API UAuraUpgradeWidget : public UAuraUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 CurrentLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 MaxLevel = 0;

	UFUNCTION(BlueprintImplementableEvent)
		void CreateUpgrade();
};
