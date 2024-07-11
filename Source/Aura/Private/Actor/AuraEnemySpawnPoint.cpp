// Artic Mango Gaming.


#include "Actor/AuraEnemySpawnPoint.h"
#include "Character/AuraEnemy.h"

void AAuraEnemySpawnPoint::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	//AYourCharacter* SpawnedCharacter = GetWorld()->SpawnActorDeferred<AYourCharacter>(AYourCharacter::StaticClass(), SpawnLocation, SpawnRotation, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	AAuraEnemy* Enemy = GetWorld()->SpawnActorDeferred<AAuraEnemy>(EnemyClass, GetActorTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	Enemy->SetLevel(EnemyLevel);
	Enemy->SetCharacterClass(CharacterClass);
	Enemy->FinishSpawning(GetActorTransform());
	Enemy->SpawnDefaultController();
}
