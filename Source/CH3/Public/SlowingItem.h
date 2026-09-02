
#pragma once

#include "CoreMinimal.h"
#include "BaseActor.h"
#include "SlowingItem.generated.h"

UCLASS()
class CH3_API ASlowingItem : public ABaseActor
{
	GENERATED_BODY()
public:
	ASlowingItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Debuff")
	float SlowDuration;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item|Debuff")
	float SlowMultiplier;

	virtual void ActivateItem(AActor* Activator) override;
};