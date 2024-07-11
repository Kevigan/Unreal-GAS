// Artic Mango Gaming.


#include "Actor/AuraEnemySpawnVolume.h"
#include "Actor/AuraEnemySpawnPoint.h"
#include "Aura/Aura.h"
#include "Components/BoxComponent.h"
#include "Interaction/PlayerInterface.h"
#include "Character/AuraEnemy.h"
#include "Actor/Teleporter.h"
#include "UI/HUD/AuraHUD.h"
#include "UI/WidgetController/LevelModeController.h"
#include "Kismet/GameplayStatics.h"

AAuraEnemySpawnVolume::AAuraEnemySpawnVolume()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>("Box");
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionObjectType(ECC_WorldStatic);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

}

void AAuraEnemySpawnVolume::LoadActor_Implementation()
{
	if (bReached)
	{
		Destroy();
	}
}

void AAuraEnemySpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	Box->OnComponentBeginOverlap.AddDynamic(this, &AAuraEnemySpawnVolume::OnBoxOverlap);
}

void AAuraEnemySpawnVolume::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->Implements<UPlayerInterface>()) return;

	bReached = true;

	PlayerEntered();
	AuraHUD = Cast<AAuraHUD>(UGameplayStatics::GetPlayerController(this, 0)->GetHUD());
	if (AuraHUD)
	{
		LevelWidgetController = AuraHUD->InitLevelModeController();
		AuraHUD->AddLevelInfoWidget();
		if (LevelWidgetController)
		{
			LevelWidgetController->SetQuestText(QuestText);
			LevelWidgetController->SetQuestValue(QuestValue);
		}
	}

	Box->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AAuraEnemySpawnVolume::SpawnEnemies()
{
	if (CurrentSpawnRound < MaxSpawnRounds == false) return;
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	TSubclassOf<AAuraEnemy> SubEnemy;

	TArray<AAuraEnemySpawnPoint*> SpawnPointsTemp = SpawnPoints;

	for (int32 i = 0; i < MaxSpawnEnemies; i++)
	{
		if (CurrentSpawnRound < 6)
		{
			SubEnemy = EnemyClassesTier1[FMath::RandRange(0, EnemyClassesTier1.Num() - 1)];
		}
		else
		{
			SubEnemy = EnemyClassesTier2[FMath::RandRange(0, EnemyClassesTier2.Num() - 1)];
		}
		AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(SubEnemy, GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
		Enemy->SetLevel(EnemyLevel);
		Enemy->SetCharacterClass(CharacterClass);
		Enemy->SpawnDefaultController();
		//Enemy->FinishSpawning(GetActorTransform());
		Enemy->FinishSpawning(SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)]->GetActorTransform());
		Enemy->OnDeathDelegate.AddDynamic(this, &AAuraEnemySpawnVolume::OnEnemyDeath);
		CurrentSpawnedEnemies++;
	}
	CurrentSpawnRound++;
}

void AAuraEnemySpawnVolume::SetQuestTextAndValue(FString InQuestText, int32 InQuestValue)
{
	if (LevelWidgetController)
	{
		LevelWidgetController->SetQuestText(InQuestText);
		LevelWidgetController->SetQuestValue(InQuestValue);
	}
}

void AAuraEnemySpawnVolume::SpawnEnemy()
{

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	TSubclassOf<AAuraEnemy> SubEnemy;

	if (CurrentSpawnRound < 6)
	{
		SubEnemy = EnemyClassesTier1[FMath::RandRange(0, EnemyClassesTier1.Num() - 1)];
	}
	else
	{
		SubEnemy = EnemyClassesTier2[FMath::RandRange(0, EnemyClassesTier2.Num() - 1)];
	}
	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(SubEnemy, GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->SpawnDefaultController();
	Enemy->FinishSpawning(SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)]->GetActorTransform());
	Enemy->OnDeathDelegate.AddDynamic(this, &AAuraEnemySpawnVolume::OnEnemyDeath);
	CurrentSpawnedEnemies++;
}

void AAuraEnemySpawnVolume::OnEnemyDeath(AActor* DestroyedActor)
{
	if (bIsSurviveWavesLevelMode)
	{
		SurviveWavesCounter();
	}
	else if (bIsKillAllEnemiesLevelMode)
	{
		KillAllEnemiesCounter();
	}
}

void AAuraEnemySpawnVolume::SurviveWavesCounter()
{
	CurrentKilledEnemies++;
	if (CurrentKilledEnemies == MaxSpawnEnemies && CurrentSpawnRound < MaxSpawnRounds)
	{
		CurrentKilledEnemies = 0;
		MaxSpawnEnemies++;
		CurrentSpawnedEnemies = 0;
		CurrentSpawnRound++;
		QuestValue++;
		if (LevelWidgetController)
		{
			LevelWidgetController->SetQuestValue(QuestValue);
		}
		PlayerEntered();
	}
	if (CurrentSpawnRound == MaxSpawnRounds && CurrentKilledEnemies == MaxSpawnEnemies)
	{
		for (ATeleporter* TP : Teleporters)
		{
			if (TP != nullptr)
			{
				TP->ActivateTeleporter();
			}
		}
		if (AuraHUD)
		{
			AuraHUD->RemoveLevelInfoWidget();
		}
	}
}

void AAuraEnemySpawnVolume::KillAllEnemiesCounter()
{
	if (QuestValue > 0)
	{
		QuestValue--;
		LevelWidgetController->SetQuestValue(QuestValue);
		if (QuestValue > 0) PlayerEntered();
	}
	if (QuestValue == 0)
	{
		for (ATeleporter* TP : Teleporters)
		{
			if (TP != nullptr)
			{
				TP->ActivateTeleporter();
			}
		}
		if (AuraHUD)
		{
			AuraHUD->RemoveLevelInfoWidget();
		}
	}
}

