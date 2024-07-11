// Artic Mango Gaming.


#include "Actor/Teleporter.h"
#include "Interaction/PlayerInterface.h"
#include "Components/SphereComponent.h"
#include "Player/AuraPlayerController.h"

// Sets default values
ATeleporter::ATeleporter()
{
	PrimaryActorTick.bCanEverTick = false;

	TeleportMesh = CreateDefaultSubobject<UStaticMeshComponent>("CheckpointMesh");
	TeleportMesh->SetupAttachment(GetRootComponent());
	TeleportMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	TeleportMesh->SetCollisionResponseToAllChannels(ECR_Block);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(TeleportMesh);
	Sphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Sphere->SetCollisionResponseToAllChannels(ECR_Ignore);
	Sphere->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	TeleportToComponent = CreateDefaultSubobject<USceneComponent>("MoveToComponent");
	TeleportToComponent->SetupAttachment(GetRootComponent());
}

void ATeleporter::ActivateTeleporter()
{
	bIsActiveTeleporter = true;
	TeleportMesh->SetVisibility(true);
}

void ATeleporter::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATeleporter::OnSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ATeleporter::OnSphereEndOverlap);

	AuraPC = Cast<AAuraPlayerController>(GetWorld()->GetFirstPlayerController());
}

void ATeleporter::SetMoveToLocation_Implementation(FVector& OutDestination)
{

}

void ATeleporter::HighlightActor_Implementation()
{
	TeleportMesh->SetRenderCustomDepth(true);
}

void ATeleporter::UnHighlightActor_Implementation()
{
	TeleportMesh->SetRenderCustomDepth(false);
}

void ATeleporter::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>() && bIsActiveTeleporter)
	{
		bIsInSphere = true;
		HighlightActor_Implementation();
	}
}

void ATeleporter::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->Implements<UPlayerInterface>() && bIsActiveTeleporter)
	{
		UnHighlightActor_Implementation();
	}
}

void ATeleporter::HandleGlowEffects()
{
	UMaterialInstanceDynamic* DynamicMaterialInstace = UMaterialInstanceDynamic::Create(TeleportMesh->GetMaterial(0), this);
	TeleportMesh->SetMaterial(0, DynamicMaterialInstace);
}





