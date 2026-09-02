
#include "ReverseControlItem.h"
#include "SpartaCharacter.h"

AReverseControlItem::AReverseControlItem()
{
	ReverseDuration = 5.0f;
	ItemType = "ReverseControl";
}

void AReverseControlItem::ActivateItem(AActor* Activator)
{
	Super::ActivateItem(Activator);

	if (Activator && Activator->ActorHasTag("Player"))
	{
		if (ASpartaCharacter* PlayerCharacter = Cast<ASpartaCharacter>(Activator))
		{
			PlayerCharacter->ApplyDebuff(EDebuffType::ReverseControl, ReverseDuration);
		}
		DestroyItem();
	}
}