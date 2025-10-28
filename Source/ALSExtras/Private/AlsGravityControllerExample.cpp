#include "AlsGravityControllerExample.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Utility/AlsGravityUtility.h"

void AAlsGravityControllerExample::UpdateRotation(float DeltaTime)
{
	FVector GravityDirection = UCharacterMovementComponent::DefaultGravityDirection;
	if (ACharacter* PlayerCharacter = Cast<ACharacter>(GetPawn()))
	{
		if (UCharacterMovementComponent* MoveComp = PlayerCharacter->GetCharacterMovement())
		{
			GravityDirection = MoveComp->GetGravityDirection();
		}
	}

	// Get the current control rotation in world space
	FRotator ViewRotation = GetControlRotation();

	// Add any rotation from the gravity changes, if any happened.
	// Delete this code block if you don't want the camera to automatically compensate for gravity rotation.
	if (!LastFrameGravity.Equals(FVector::ZeroVector))
	{
		const FQuat DeltaGravityRotation = FQuat::FindBetweenNormals(LastFrameGravity, GravityDirection);
		const FQuat WarpedCameraRotation = DeltaGravityRotation * FQuat(ViewRotation);

		ViewRotation = WarpedCameraRotation.Rotator();
	}
	LastFrameGravity = GravityDirection;

	// Convert the view rotation from world space to gravity relative space.
	// Now we can work with the rotation as if no custom gravity was affecting it.
	ViewRotation = UAlsGravityUtility::WorldToGravityRotation(ViewRotation, GravityDirection);

	// Calculate Delta to be applied on ViewRotation
	FRotator DeltaRot(RotationInput);

	if (PlayerCameraManager)
	{
		PlayerCameraManager->ProcessViewRotation(DeltaTime, ViewRotation, DeltaRot);

		// Zero the roll of the camera as we always want it horizontal in relation to the gravity.
		ViewRotation.Roll = 0;

		// Convert the rotation back to world space, and set it as the current control rotation.
		SetControlRotation(UAlsGravityUtility::GravityToWorldRotation(ViewRotation, GravityDirection));
	}

	APawn* const P = GetPawnOrSpectator();
	if (P)
	{
		P->FaceRotation(ViewRotation, DeltaTime);
	}
}
