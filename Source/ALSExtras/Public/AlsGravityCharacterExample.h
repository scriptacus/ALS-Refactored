#pragma once

#include "CoreMinimal.h"
#include "AlsCharacterExample.h"
#include "AlsGravityCharacterExample.generated.h"

UCLASS()
class ALSEXTRAS_API AAlsGravityCharacterExample : public AAlsCharacterExample
{
	GENERATED_BODY()

protected:
	virtual void Input_OnMove(const FInputActionValue& ActionValue) override;
};
