// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "AuraHUD.generated.h"

class UAuraUserWidget;
class UOverlayWidgetController;
class UAttributeMenuWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;
class USpellMenuWidgetController;
class ULevelModeController;
class ULevelInfoWidget;
struct FWidgetControllerParmas;

/**
 *
 */
UCLASS()
class AURA_API AAuraHUD : public AHUD
{
	GENERATED_BODY()

public:


	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParmas& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuWidgetController(const FWidgetControllerParmas& WCParams);
	USpellMenuWidgetController* GetSpellMenuWidgetController(const FWidgetControllerParmas& WCParams);

	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);

	UFUNCTION(BlueprintCallable)
		ULevelModeController* InitLevelModeController();

	UFUNCTION()
		void AddLevelInfoWidget();

	UFUNCTION()
		void RemoveLevelInfoWidget();

protected:

private:
	UPROPERTY()
		TObjectPtr<UAuraUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UAuraUserWidget> OverlayWidgetClass;

	UPROPERTY()
		TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
		TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
		TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
		TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
		TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;

	UPROPERTY()
		TObjectPtr<ULevelInfoWidget> LevelInfoWidget;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ULevelInfoWidget> LevelInfoWidgetClass;

	UPROPERTY()
		TObjectPtr<ULevelModeController> LevelModeController;

	UPROPERTY(EditAnywhere)
		TSubclassOf<ULevelModeController> LevelModeControllerClass;
};
