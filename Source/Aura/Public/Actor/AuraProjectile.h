// Artic Mango Gaming.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AuraAbilityTypes.h"
#include "AuraProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;
class UNiagaraSystem;

UCLASS()
class AURA_API AAuraProjectile : public AActor
{
	GENERATED_BODY()

public:
	AAuraProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		TObjectPtr<UProjectileMovementComponent> ProjectileMovement;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = true))
		FDamageEffectParams DamageEffectParams;

	UPROPERTY()
		TObjectPtr<USceneComponent> HomingTargetSceneComponent;

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
		virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
		virtual void OnHit();

	bool IsValidOverlap(AActor* OtherActor);
	bool bHit = false;

	UPROPERTY()
		TObjectPtr<UAudioComponent> LoopingSoundComponent;

private:
	UPROPERTY(EditDefaultsOnly)
		float LifeSpan = 15.f;

	virtual void Destroyed() override;

	UPROPERTY(VisibleAnywhere)
		TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(EditAnywhere)
		TObjectPtr<UNiagaraSystem> ImpactEffect;

	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> ImpactSound;

	UPROPERTY(EditAnywhere)
		TObjectPtr<USoundBase> LoopingSound;


};
