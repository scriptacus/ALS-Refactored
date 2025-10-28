#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AlsGravityControllerExample.generated.h"

/**
 * 
 */
UCLASS()
class ALSEXTRAS_API AAlsGravityControllerExample : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void UpdateRotation(float DeltaTime) override;

private:
	FVector LastFrameGravity = FVector::ZeroVector;
};
