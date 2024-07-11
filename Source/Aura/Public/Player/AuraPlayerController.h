// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "AuraPlayerController.generated.h"


class UInputMappingContext;
class UInputAction;
class IHighlightInterface;
class UAuraInputConfig;
class UAuraAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;
class UNiagaraSystem;
class AMagicCircle;
struct FInputActionValue;

DECLARE_MULTICAST_DELEGATE(FOnPlayerInteract)
DECLARE_MULTICAST_DELEGATE(FOnOpenUpgrade)

enum class ETargetingStatus : uint8
{
	TargetingEnemy,
	TargetingNonEnemy,
	NotTargeting
};

/**
 *
 */
UCLASS()
class AURA_API AAuraPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAuraPlayerController();

	UFUNCTION(Client, Reliable)
		void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);

	UFUNCTION(BlueprintCallable)
		void ShowMagicCircle(UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintCallable)
		void HideMagicCircle();

	FOnPlayerInteract OnPlayerInteract;
	FOnOpenUpgrade OnOpenUpgrade;
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void PlayerTick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
		void TestUnit(bool InVisibility);

private:
	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr<UInputMappingContext> AuraContext;

	UPROPERTY(EditAnywhere, Category = "Input")
		TObjectPtr <UInputAction> MoveAction;

	void Move(const FInputActionValue& InputActionValue);
	void Interact();
	void Upgrade();
	void SwitchLoadout();

	void CursorTrace();
	TObjectPtr<AActor> LastActor;
	TObjectPtr<AActor> ThisActor;
	FHitResult CursorHit;

	static void HighlightActor(AActor* InActor);
	static void UnHighlightActor(AActor* InActor);

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UAuraInputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> ShiftAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> UpgradeAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
		TObjectPtr<UInputAction> SwitchLoadoutAction;

	void ShiftPressed() { bForceHalt = true; }
	void ShiftReleased() { bForceHalt = false; }
	bool bForceHalt = false;

	UPROPERTY()
		TObjectPtr<UAuraAbilitySystemComponent> AuraAbilitySystemComponent;

	UAuraAbilitySystemComponent* GetASC();

	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	ETargetingStatus TargetingStatus = ETargetingStatus::NotTargeting;
	bool bForceMove = false;

	UPROPERTY(EditDefaultsOnly)
		float AutoRUnAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr< USplineComponent> Spline;

	UPROPERTY(EditDefaultsOnly)
		TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;

	void AutoRun();
	void MoveToTarget();

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
		TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircleLocation();

	bool bShouldRotate = false;
	bool bIsChanneling = false;
	void RotateAura();
	FTimerHandle RotateTimer;
};
