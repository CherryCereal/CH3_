
#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "ReverseControlItem.generated.h"

UCLASS()
class CH3_API AReverseControlItem : public ABaseActor
{
	GENERATED_BODY()
public:
	AReverseControlItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Debuff")
	float ReverseDuration;

	virtual void ActivateItem(AActor* Activator) override;
};