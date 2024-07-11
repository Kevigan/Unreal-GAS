// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MagicCircle.generated.h"

class UDecalComponent;

UCLASS()
class AURA_API AMagicCircle : public AActor
{
	GENERATED_BODY()

public:
	AMagicCircle();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UDecalComponent> MagicCircleDecal;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;


};
