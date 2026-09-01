
#include "BaseActor.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

ABaseActor::ABaseActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Collision->SetupAttachment(Scene);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(Collision);

	Collision->OnComponentBeginOverlap.AddDynamic(this, &ABaseActor::OnItemOverlap);
	Collision->OnComponentEndOverlap.AddDynamic(this, &ABaseActor::OnItemEndOverlap);
}

void ABaseActor::OnItemOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if(OtherActor && OtherActor->ActorHasTag("Player"))
	{
		ActivateItem(OtherActor);
	}
}
void ABaseActor::OnItemEndOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{

}
void ABaseActor::ActivateItem(AActor* Activator)
{
	UParticleSystemComponent* Particle = nullptr;

	if(PickUpParticle)
	{
		Particle = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), PickUpParticle, GetActorLocation(),GetActorRotation(), true);
	}
	if(PickUpSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), PickUpSound, GetActorLocation());
	}

	if(Particle)
	{
		FTimerHandle DestroyParticleTimerHandle;
		GetWorld()->GetTimerManager().SetTimer(DestroyParticleTimerHandle, [Particle]()
		{
			Particle->DestroyComponent();
			}, 1.0f, false);
	}
}
FName ABaseActor::GetItemType() const
{
	return ItemType;
}

void ABaseActor::DestroyItem()
{
	Destroy();

	
}



