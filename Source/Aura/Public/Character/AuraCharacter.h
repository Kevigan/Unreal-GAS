// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "Character/AuraCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "AuraCharacter.generated.h"

class UNiagaraComponent;
class UCameraComponent;
class USpringArmComponent;
class UDecalComponent;
class APlayerController;
/**
 *
 */
UCLASS()
class AURA_API AAuraCharacter : public AAuraCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	/* TESTING */
	UPROPERTY(BlueprintReadWrite)
		bool bIsFirstLoadout = true;
	/* TESTING END*/
	AAuraCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	/**Player Interface*/
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 FindLevelForXP_Implementation(int32 InXP) const override;
	virtual int32 GetAttributePointsReward_Implementation(int32 Level) const override;
	virtual int32 GetSpellPointsReward_Implementation(int32 Level) const override;
	virtual void AddToPlayerLevel_Implementation(int32 InPlayerlevel) override;
	virtual void AddToAttributePoints_Implementation(int32 InAttributePoints) override;
	virtual void AddToSpellPoints_Implementation(int32 InSpellPoints) override;
	virtual int32 GetAttributePoints_Implementation() const override;
	virtual int32 GetSpellPoints_Implementation() const override;
	virtual void ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial /* = nullptr */) override;
	virtual void HideMagicCircle_Implementation() override;
	virtual void SaveProgress_Implementation(const FName& CheckpointTag) override;
	virtual void OnRep_Stunned() override;
	virtual void OnRep_Burned() override;

	/**End Player Interface*/

	/**Combat Interface*/
	virtual int32 GetPlayerLevel_Implementation() override;
	virtual void Die(const FVector& DeathImpulse) override;
	/*end Combat Interface*/

	void SwitchLoadout();

	UPROPERTY(EditDefaultsOnly)
		float DeathTime = 5.f;

	FTimerHandle DeathTimer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UNiagaraComponent> LevelUpNiagaraComponent;

	UFUNCTION(BlueprintCallable)
		void SetHasLifeSyphon(bool HasLifeSyphon);

	UFUNCTION(BlueprintImplementableEvent)
		void OnEnemyEnterLifeSyphon(/*TArray<AActor*>& ActorsToAddEffect*/);

	UFUNCTION(BlueprintImplementableEvent)
		void OnEnemyExitLifeSyphon(/*TArray<AActor*>& ActorsToRemoveEffect*/);

	UFUNCTION(BlueprintCallable)
		void ResetSiphon();

	UPROPERTY(BlueprintReadOnly)
		TArray<AActor*> OverlappingActors = TArray<AActor*>();
	UPROPERTY(BlueprintReadOnly)
		TArray<AActor*> ActorsToRemoveEffect = TArray<AActor*>();

	void LoadProgress();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UDecalComponent> LookAtDecal;

	UFUNCTION(BlueprintCallable)
		void SetVisibilityLookAt(bool InVisisible);
	void SetLookAtDecalRotation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<UCameraComponent> TopdownCameraCompontent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TObjectPtr<USpringArmComponent> CameraBoom;
private:
	APlayerController* PC;

	virtual void InitAbilityActorInfo() override;

	UFUNCTION(NetMulticast, Reliable)
		void MulticastLevelUpParticles() const;

	bool bHasLifeSyphon = false;

	void LifeSyphonEnemyInRadiusCounter();

	void MoveCameraWithMouse();
};
