// Artic Mango Gaming.


#include "Character/AuraCharacter.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerState.h"
#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "Player/AuraPlayerController.h"
#include "UI/HUD/AuraHUD.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "Camera/CameraComponent.h"
#include "AuraGameplayTags.h"
#include "Character/AuraCharacterBase.h"
#include "AbilitySystem/AuraAbilitySystemLibrary.h"
#include "Game/AuraGameModeBase.h"
#include "Game/LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "AbilitySystem/AuraAttributeSet.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "Components/DecalComponent.h"
#include "Kismet/KismetMathLibrary.h"

AAuraCharacter::AAuraCharacter()
{
	PrimaryActorTick.TickInterval = 0.2f;
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopdownCameraCompontent = CreateDefaultSubobject<UCameraComponent>("TopdownCameraComponent");
	TopdownCameraCompontent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopdownCameraCompontent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elementalist;

	LookAtDecal = CreateDefaultSubobject<UDecalComponent>("LookAtDecal");
	LookAtDecal->SetupAttachment(GetRootComponent());
	LookAtDecal->SetVisibility(false);
}

void AAuraCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	LifeSyphonEnemyInRadiusCounter();
	//MoveCameraWithMouse();
}

void AAuraCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init ability actor info for the Server
	InitAbilityActorInfo();
	LoadProgress();
	if (AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this)))
	{
		AuraGameMode->LoadWorldState(GetWorld());
		AuraGameMode->SetUnlockedSpell();
	}
}

void AAuraCharacter::LoadProgress()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameMode)
	{
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;


		/*if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
		}
		else
		{
		}*/
		AddCharacterAbilities();
		if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
		{
			AuraASC->AddCharacterAbilitiesFromSaveData(SaveData, true);
		}
		if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
		{
			AuraPlayerState->SetLevel(SaveData->PlayerLevel);
			AuraPlayerState->SetXP(SaveData->XP);
			AuraPlayerState->SetAttributePoints(SaveData->AttributePoints);
			//AuraPlayerState->SetSpellPoints(SaveData->SpellPoints);
		}
		UAuraAbilitySystemLibrary::InitializeDefaultAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
	}
}

void AAuraCharacter::SetVisibilityLookAt(bool InVisisible)
{
	LookAtDecal->SetVisibility(InVisisible);
}

void AAuraCharacter::SetLookAtDecalRotation()
{
	if (!LookAtDecal->IsVisible()) return;
	PC = nullptr ? UGameplayStatics::GetPlayerController(this, 0) : PC;
	FVector2D MousePosition;
	PC->GetMousePosition(MousePosition.X, MousePosition.Y);
	FHitResult HitResult;
	PC->GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	APawn* ControlledPawn = PC->GetPawn<APawn>();
	FVector V1 = FVector(ControlledPawn->GetActorLocation().X, ControlledPawn->GetActorLocation().Y, 0.f);
	FVector V2 = FVector(HitResult.Location.X, HitResult.Location.Y, 0.f);
	ControlledPawn->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(V1, V2));
}

void AAuraCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for the Client
	InitAbilityActorInfo();
}

void AAuraCharacter::AddToXP_Implementation(int32 InXP)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToXP(InXP);
}

void AAuraCharacter::LevelUp_Implementation()
{
	MulticastLevelUpParticles();
}

void AAuraCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopdownCameraCompontent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotation = (CameraLocation - NiagaraSystemLocation).Rotation();
		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotation);
		LevelUpNiagaraComponent->Activate(true);
	}
}

int32 AAuraCharacter::GetXP_Implementation() const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetXP();
}

int32 AAuraCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 AAuraCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 AAuraCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void AAuraCharacter::AddToPlayerLevel_Implementation(int32 InPlayerlevel)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToLevel(InPlayerlevel);

	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		AuraASC->UpdateAbilityStatuses(AuraPlayerState->GetPlayerLevel());
	}
}

void AAuraCharacter::AddToAttributePoints_Implementation(int32 InAttributePoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToAttributePoints(InAttributePoints);
}

void AAuraCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 AAuraCharacter::GetAttributePoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetAttributePoints();
}

int32 AAuraCharacter::GetSpellPoints_Implementation() const
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetSpellPoints();
}

void AAuraCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->ShowMagicCircle(DecalMaterial);
		AuraPlayerController->bShowMouseCursor = false;
	}
}

void AAuraCharacter::HideMagicCircle_Implementation()
{
	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		AuraPlayerController->HideMagicCircle();
		AuraPlayerController->bShowMouseCursor = true;
	}
}

void AAuraCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameMode)
	{
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr) return;

		//SaveData->PlayerStartTag = CheckpointTag;

		if (AAuraPlayerState* AuraPlayerState = Cast<AAuraPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = AuraPlayerState->GetPlayerLevel();
			SaveData->XP = AuraPlayerState->GetXP();
			//SaveData->SpellPoints = AuraPlayerState->GetSpellPoints();
			SaveData->AttributePoints = AuraPlayerState->GetAttributePoints();
		}
		SaveData->Strength = UAuraAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UAuraAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UAuraAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UAuraAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());

		SaveData->bFirstTimeLoadIn = false;

		if (!HasAuthority()) return;


		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveData->SavedLoadoutAbilities.Empty();
		SaveAbilityDelegate.BindLambda([this, AuraASC, SaveData](const FGameplayAbilitySpec& AbilitySpec)
			{
				const FGameplayTag AbilityTag = AuraASC->GetAbilityTagFromSpec(AbilitySpec);
				UAbilityInfo* AbilityInfo = UAuraAbilitySystemLibrary::GetAbilityInfo(this);
				FAuraAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

				FSavedAbility SavedAbility;
				SavedAbility.GameplayAbility = Info.Ability;
				SavedAbility.AbilityLevel = AbilitySpec.Level;
				SavedAbility.AbilitySlot = AuraASC->GetSlotFromAbilityTag(AbilityTag);
				SavedAbility.AbilityStatus = AuraASC->GetStatusFromAbilityTag(AbilityTag);
				SavedAbility.AbilityTag = AbilityTag;
				SavedAbility.AbilityType = Info.AbilityType;

				SaveData->SavedLoadoutAbilities.AddUnique(SavedAbility);
			});
		AuraASC->ForEachAbility(SaveAbilityDelegate);

		AuraGameMode->SaveInGameProgressData(SaveData);
	}
}

void AAuraCharacter::OnRep_Stunned()
{
	if (UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FAuraGameplayTags& GameplayTags = FAuraGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTags.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTags.Player_Block_InputReleased);
		if (bIsStunned)
		{
			AuraASC->AddLooseGameplayTags(BlockedTags);
			StunDebuffComp->Activate();
		}
		else
		{
			AuraASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComp->Deactivate();
		}
	}
}

void AAuraCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComp->Activate();
	}
	else
	{
		BurnDebuffComp->Deactivate();
	}
}

int32 AAuraCharacter::GetPlayerLevel_Implementation()
{
	const AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	return AuraPlayerState->GetPlayerLevel();
}

void AAuraCharacter::Die(const FVector& DeathImpulse)
{
	Super::Die(DeathImpulse);

	FTimerDelegate DeathTimerDelegate;
	DeathTimerDelegate.BindLambda([this]()
		{
			AAuraGameModeBase* AuraGM = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
			if (AuraGM)
			{
				AuraGM->PlayerDied(this);
			}
		});
	GetWorldTimerManager().SetTimer(DeathTimer, DeathTimerDelegate, DeathTime, false);
	TopdownCameraCompontent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void AAuraCharacter::SwitchLoadout()
{
	AAuraGameModeBase* AuraGameMode = Cast<AAuraGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (AuraGameMode)
	{
		UAuraAbilitySystemComponent* AuraASC = Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent);
		ULoadScreenSaveGame* SaveData = AuraGameMode->RetrieveInGameSaveData();
		for (FSavedAbility Ability : SaveData->SavedLoadoutAbilities)
		{
			if (AuraASC)
			{
				AuraASC->ServerRemoveLoadoutSlot(Ability.AbilityTag, Ability.AbilitySlot);
			}
		}
		if (bIsFirstLoadout) AuraASC->AddCharacterAbilitiesFromSaveData(SaveData, false);
		else AuraASC->AddCharacterAbilitiesFromSaveData(SaveData, true);
		bIsFirstLoadout = !bIsFirstLoadout;
	}
}

void AAuraCharacter::InitAbilityActorInfo()
{
	AAuraPlayerState* AuraPlayerState = GetPlayerState<AAuraPlayerState>();
	check(AuraPlayerState);
	AuraPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(AuraPlayerState, this);
	Cast<UAuraAbilitySystemComponent>(AuraPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = AuraPlayerState->GetAbilitySystemComponent();
	AttributeSet = AuraPlayerState->GetAttributeSet();
	OnASCRegistered.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FAuraGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &AAuraCharacter::StunTagChanged);

	if (AAuraPlayerController* AuraPlayerController = Cast<AAuraPlayerController>(GetController()))
	{
		if (AAuraHUD* AuraHUD = Cast<AAuraHUD>(AuraPlayerController->GetHUD()))
		{
			AuraHUD->InitOverlay(AuraPlayerController, AuraPlayerState, AbilitySystemComponent, AttributeSet);
		}
	}
}

void AAuraCharacter::SetHasLifeSyphon(bool HasLifeSyphon)
{
	bHasLifeSyphon = HasLifeSyphon;
}

void AAuraCharacter::ResetSiphon()
{
	OverlappingActors.Empty();
	ActorsToRemoveEffect.Empty();
}

void AAuraCharacter::LifeSyphonEnemyInRadiusCounter()
{
	if (!bHasLifeSyphon) return;

	OverlappingActors.Empty();
	FVector SphereOrigin = GetActorLocation();
	TArray<AActor*> ActorsToIgnore = TArray<AActor*>();
	UAuraAbilitySystemLibrary::GetLivePlayersWithinRadius(this, OverlappingActors, ActorsToIgnore, 500.f, SphereOrigin);
	for (AActor* Enemie : OverlappingActors)
	{
		if (Enemie == this) continue;
		float Distance = FVector::Distance(Enemie->GetActorLocation(), SphereOrigin);
		if (Distance > 500.f)
		{
			ActorsToRemoveEffect.AddUnique(Enemie);
			OverlappingActors.Remove(Enemie);
		}
	}
	//if(OverlappingActors.Num() < 1) bHasLifeSyphon = false;
	OnEnemyExitLifeSyphon();
	OnEnemyEnterLifeSyphon();

	//ActorsToRemoveEffect.Empty();
}

void AAuraCharacter::MoveCameraWithMouse()
{
	if (PC == nullptr) PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!IsValid(PC)) return;
	int32 ViewportSizeX = 0;
	int32 ViewportSizeY = 0;
	PC->GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ViewportSize;
	ViewportSize.X = ViewportSizeX;
	ViewportSize.Y = ViewportSizeY;

	FVector2D MousePosition;
	if (PC->GetMousePosition(MousePosition.X, MousePosition.Y))
	{
		FVector2D CameraOffset = (MousePosition - ViewportSize / 2) * 0.1f;

		CameraBoom->AddRelativeLocation(FVector(CameraOffset, 0));
	}
}


