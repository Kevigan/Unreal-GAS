// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/SaveInterface.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AuraEnemySpawnVolume.generated.h"

class AAuraEnemySpawnPoint;
class UBoxComponent;
class ATeleporter;
class ULevelModeController;
class AAuraHUD;
class AAuraEnemy;

UCLASS()
class AURA_API AAuraEnemySpawnVolume : public AActor, public ISaveInterface
{
	GENERATED_BODY()

public:
	AAuraEnemySpawnVolume();

	/* Save Interface */
	virtual void LoadActor_Implementation() override;
	/* end Save Interface */

	UPROPERTY(BlueprintReadOnly, SaveGame)
		bool bReached = false;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
		TArray<AAuraEnemySpawnPoint*> SpawnPoints;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		TSubclassOf<AAuraEnemy> EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		TArray<TSubclassOf<AAuraEnemy>> EnemyClassesTier1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		TArray<TSubclassOf<AAuraEnemy>> EnemyClassesTier2;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		ECharacterClass CharacterClass = ECharacterClass::Warrior;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		TArray<ATeleporter*> Teleporters;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		int32 MaxSpawnRounds = 10;
	UPROPERTY(BlueprintReadOnly, Category = "Spawn Settings")
		int32 CurrentSpawnRound = 1;
	UPROPERTY(BlueprintReadOnly, Category = "Spawn Settings")
		int32 MaxSpawnEnemies = 1.f;
	UPROPERTY(BlueprintReadOnly, Category = "Spawn Settings")
		int32 CurrentKilledEnemies = 0.f;

	UPROPERTY(BlueprintReadOnly, Category = "Spawn Settings")
		int32 CurrentSpawnedEnemies = 0.f;

	UFUNCTION(BlueprintCallable)
		void SpawnEnemy();

	UFUNCTION(BlueprintImplementableEvent)
		void PlayerEntered();

	UPROPERTY(BlueprintReadWrite, Category = "Spawn Settings")
		ULevelModeController* LevelWidgetController;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		FString QuestText;

	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
		int32 QuestValue = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		bool bIsSurviveWavesLevelMode = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spawn Settings")
		bool bIsKillAllEnemiesLevelMode = false;

	UFUNCTION()
		void SetQuestTextAndValue(FString InQuestText, int32 InQuestValue);


private:

	AAuraHUD* AuraHUD;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<UBoxComponent> Box;

	void SpawnEnemies();

	UFUNCTION()
		void OnEnemyDeath(AActor* DestroyedActor);

	void SurviveWavesCounter();
	void KillAllEnemiesCounter();
};
