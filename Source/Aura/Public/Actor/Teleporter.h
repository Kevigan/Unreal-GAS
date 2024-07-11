// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/HighlightInterface.h"
#include "Aura/Aura.h"
#include "Teleporter.generated.h"

class USphereComponent;
class AAuraPlayerController;

UCLASS()
class AURA_API ATeleporter : public AActor, public IHighlightInterface
{
	GENERATED_BODY()

public:
	ATeleporter();

	void ActivateTeleporter();

protected:
	virtual void BeginPlay() override;

	/* Highlight Interface */
	virtual void SetMoveToLocation_Implementation(FVector& OutDestination) override;
	virtual void HighlightActor_Implementation() override;
	virtual void UnHighlightActor_Implementation() override;
	/* Highlight Interface */

	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UStaticMeshComponent> TeleportMesh;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(BlueprintReadOnly)
		bool bIsInSphere = false;

	UFUNCTION(BlueprintCallable)
		void HandleGlowEffects();

	UPROPERTY(EditDefaultsOnly)
		int32 CustomDepthStencilOverride = CUSTOM_DEPTH_TAN;

	AAuraPlayerController* AuraPC;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TObjectPtr<USceneComponent> TeleportToComponent;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
		bool bIsActiveTeleporter = false;
};
