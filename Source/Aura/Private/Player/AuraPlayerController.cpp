// Artic Mango Gaming.


#include "Player/AuraPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"
#include "GameplayTagContainer.h"
#include "Input/AuraInputComponent.h"
#include "../Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Actor/MagicCircle.h"
#include "Components/SplineComponent.h"
#include "AuraGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"
#include "UI/Widget/DamageTextComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "../Aura.h"
#include "Components/DecalComponent.h"
#include "Interaction/HighlightInterface.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/AuraPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Character/AuraCharacter.h"

AAuraPlayerController::AAuraPlayerController()
{
	bReplicates = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void AAuraPlayerController::ShowDamageNumber_Implementation(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit)
{
	if (IsValid(TargetCharacter) && DamageTextComponentClass && IsLocalController())
	{
		UDamageTextComponent* DamageText = NewObject<UDamageTextComponent>(TargetCharacter, DamageTextComponentClass);
		DamageText->RegisterComponent();
		DamageText->AttachToComponent(TargetCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
		DamageText->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		DamageText->SetDamageText(DamageAmount, bBlockedHit, bCriticalHit);
	}
}

void AAuraPlayerController::ShowMagicCircle(UMaterialInterface* DecalMaterial)
{
	if (!IsValid(MagicCircle))
	{
		MagicCircle = GetWorld()->SpawnActor<AMagicCircle>(MagicCircleClass);
		if (DecalMaterial)
		{
			MagicCircle->MagicCircleDecal->SetMaterial(0, DecalMaterial);
		}
	}
}

void AAuraPlayerController::HideMagicCircle()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->Destroy();
	}
}

void AAuraPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(AuraContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(AuraContext, 0);
	}

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void AAuraPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UAuraInputComponent* AuraInputComponent = CastChecked<UAuraInputComponent>(InputComponent);

	AuraInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AAuraPlayerController::Move);
	AuraInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::AbilityInputTagPressed, &ThisClass::AbilityInputTagReleased, &ThisClass::AbilityInputTagHeld);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &AAuraPlayerController::ShiftPressed);
	AuraInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &AAuraPlayerController::ShiftReleased);
	AuraInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AAuraPlayerController::Interact);
	AuraInputComponent->BindAction(UpgradeAction, ETriggerEvent::Started, this, &AAuraPlayerController::Upgrade);
	AuraInputComponent->BindAction(SwitchLoadoutAction, ETriggerEvent::Started, this, &AAuraPlayerController::SwitchLoadout);
}

void AAuraPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();
	UpdateMagicCircleLocation();
	RotateAura();
}

void AAuraPlayerController::AutoRun()
{
	if (!bAutoRunning) return;
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRUnAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void AAuraPlayerController::MoveToTarget()
{
	if (bForceHalt) return;

	FollowTime += GetWorld()->GetDeltaSeconds();

	if (CursorHit.bBlockingHit) CachedDestination = CursorHit.ImpactPoint;

	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
		ControlledPawn->AddMovementInput(WorldDirection);
	}
}

void AAuraPlayerController::UpdateMagicCircleLocation()
{
	if (IsValid(MagicCircle))
	{
		MagicCircle->SetActorLocation(CursorHit.ImpactPoint);
	}
}

void AAuraPlayerController::RotateAura()
{
	if (!bShouldRotate) return;
	ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player);
	if (LocalPlayer && LocalPlayer->ViewportClient)
	{
	}
	FVector2D MousePosition;
	GetMousePosition(MousePosition.X, MousePosition.Y);
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	APawn* ControlledPawn = GetPawn<APawn>();
	FVector V1 = FVector(ControlledPawn->GetActorLocation().X, ControlledPawn->GetActorLocation().Y, 0.f);
	FVector V2 = FVector(HitResult.Location.X, HitResult.Location.Y, 0.f);
	ControlledPawn->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(V1, V2));
}

void AAuraPlayerController::Move(const FInputActionValue& InputActionValue)
{
	/*if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
	{
		return;
	}*/
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

void AAuraPlayerController::Interact()
{
	OnPlayerInteract.Broadcast();
}

void AAuraPlayerController::Upgrade()
{
	AAuraPlayerState* AuraPS = Cast<AAuraPlayerState>(UGameplayStatics::GetPlayerState(this, 0));
	if (AuraPS->GetUpgradePoints() > 0)
	{
		OnOpenUpgrade.Broadcast();
	}
}

void AAuraPlayerController::SwitchLoadout()
{
	AAuraCharacter* AuraChraracter = Cast<AAuraCharacter>(GetCharacter());
	if (AuraChraracter)
	{
		AuraChraracter->SwitchLoadout();
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, ("FUCK off!"));
	}
}

void AAuraPlayerController::HighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>() && InActor->Implements<UEnemyInterface>())
	{
		IHighlightInterface::Execute_HighlightActor(InActor);
	}
}

void AAuraPlayerController::UnHighlightActor(AActor* InActor)
{
	if (IsValid(InActor) && InActor->Implements<UHighlightInterface>() && InActor->Implements<UEnemyInterface>())
	{
		IHighlightInterface::Execute_UnHighlightActor(InActor);
	}
}

void AAuraPlayerController::CursorTrace()
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_CursorTrace))
	{
		if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
		{
			UnHighlightActor(LastActor);
			UnHighlightActor(ThisActor);
		}
		LastActor = nullptr;
		ThisActor = nullptr;
		return;
	}
	const ECollisionChannel TraceChannel = IsValid(MagicCircle) ? ECC_ExcludePlayers : ECC_Visibility;
	GetHitResultUnderCursor(TraceChannel, false, CursorHit);
	if (!CursorHit.bBlockingHit) return;

	LastActor = ThisActor;
	if (IsValid(CursorHit.GetActor()) && CursorHit.GetActor()->Implements<UHighlightInterface>())
	{
		ThisActor = CursorHit.GetActor();
	}
	else
	{
		ThisActor = nullptr;
	}

	if (LastActor != ThisActor)
	{
		UnHighlightActor(LastActor);
		HighlightActor(ThisActor);
	}
}

void AAuraPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed) ||
		GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Dash))
	{
		return;
	}
	bool bHasSomething = GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Abilities_Fire_FireBolt);
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) || InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Space))
	{
		if (IsValid(ThisActor))
		{
			TargetingStatus = ThisActor->Implements<UEnemyInterface>() ? ETargetingStatus::TargetingEnemy : ETargetingStatus::TargetingNonEnemy;
		}
		else
		{
			TargetingStatus = ETargetingStatus::NotTargeting;
		}
		bAutoRunning = false;
	}
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Alt))
	{
		//bForceHalt = true;
		//StopMovement();
		//bTargeting = ThisActor ? true : false;
		//bAutoRunning = false;
	}
	if (GetASC()) GetASC()->AbilityInputTagPressed(InputTag);
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Space) && !InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Interact))
	{
		bShouldRotate = true;
		TestUnit(true);
	}
}

void AAuraPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	/*if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputReleased))
	{
		return;
	}*/
	if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Alt)) bForceHalt = false;
	if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) && !InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Space))
	{
		if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
		return;
	}

	if (GetASC()) GetASC()->AbilityInputTagReleased(InputTag);
	bIsChanneling = false;
	/*if (TargetingStatus != ETargetingStatus::TargetingEnemy && !bForceHalt)
	{
		if (bForceHalt) return;
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (IsValid(ThisActor) && ThisActor->Implements<UHighlightInterface>())
			{
				IHighlightInterface::Execute_SetMoveToLocation(ThisActor, CachedDestination);
			}
			else if (GetASC() && !GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputPressed))
			{
				UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ClickNiagaraSystem, CachedDestination);
			}
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints[NavPath->PathPoints.Num() - 1];
					bAutoRunning = true;
				}
			}
		}
		FollowTime = 0.f;
		TargetingStatus = ETargetingStatus::NotTargeting;
	}*/

	FTimerDelegate RotateTimerDelegate;
	RotateTimerDelegate.BindLambda([this]()
		{
			if (!bIsChanneling)
			{
				bShouldRotate = false;
				TestUnit(false);
			}
		});
	GetWorldTimerManager().SetTimer(RotateTimer, RotateTimerDelegate, 1.0f, false);
}

void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() && GetASC()->HasMatchingGameplayTag(FAuraGameplayTags::Get().Player_Block_InputHeld))
	{
		return;
	}
	//if (!InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_LMB) /*&& !InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Space)*/)
	//{
	//	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	//	return;
	//}
	//if (InputTag.MatchesTagExact(FAuraGameplayTags::Get().InputTag_Space))
	//{
	//	//MoveToTarget();
	//	return;
	//}
	if (TargetingStatus == ETargetingStatus::TargetingEnemy || bForceHalt)
	{
	}
	else
	{
		//MoveToTarget();
	}
	if (GetASC()) GetASC()->AbilityInputTagHeld(InputTag);
	bIsChanneling = true;
}

UAuraAbilitySystemComponent* AAuraPlayerController::GetASC()
{
	if (AuraAbilitySystemComponent == nullptr)
	{
		AuraAbilitySystemComponent = Cast<UAuraAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return AuraAbilitySystemComponent;
}


