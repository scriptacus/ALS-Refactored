#include "AlsGravityCharacterExample.h"

#include "Utility/AlsGravityUtility.h"
#include "Utility/AlsVector.h"
#include "EnhancedInputSubsystems.h"

void AAlsGravityCharacterExample::Input_OnMove(const FInputActionValue& ActionValue)
{
	const auto Value{UAlsVector::ClampMagnitude012D(ActionValue.Get<FVector2D>())};

	auto ViewRotation{GetViewState().Rotation};

	if (IsValid(GetController()))
	{
		// Use exact camera rotation instead of target rotation whenever possible.

		FVector ViewLocation;
		GetController()->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	// Get gravity-aligned movement vectors for input
	FVector ForwardDirection, RightDirection;
	UAlsGravityUtility::GetGravityAlignedMovementVectors(ViewRotation, this, ForwardDirection, RightDirection);

	AddMovementInput(ForwardDirection * Value.Y + RightDirection * Value.X);
}
