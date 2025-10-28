#include "Utility/AlsGravityUtility.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Utility/AlsVector.h"

// === Core Transform Functions ===

FQuat UAlsGravityUtility::GetGravityToWorldTransformByDirection(const FVector& GravityDirection)
{
	// Transform from gravity space (Z-up) to world space
	// Note: UCharacterMovementComponent::GetWorldToGravityTransform() returns this (backwards naming!)
	return FQuat::FindBetweenNormals(UCharacterMovementComponent::DefaultGravityDirection, GravityDirection);
}

FQuat UAlsGravityUtility::GetGravityToWorldTransformByCharacter(const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		return FQuat::Identity;
	}

	return GetGravityToWorldTransformByDirection(Character->GetCharacterMovement()->GetGravityDirection());
}

FQuat UAlsGravityUtility::GetWorldToGravityTransformByDirection(const FVector& GravityDirection)
{
	// Transform from world space to gravity space (Z-up)
	// Note: UCharacterMovementComponent::GetGravityToWorldTransform() returns this (backwards naming!)
	return FQuat::FindBetweenNormals(GravityDirection, UCharacterMovementComponent::DefaultGravityDirection);
}

FQuat UAlsGravityUtility::GetWorldToGravityTransformByCharacter(const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		return FQuat::Identity;
	}

	return GetWorldToGravityTransformByDirection(Character->GetCharacterMovement()->GetGravityDirection());
}

FRotator UAlsGravityUtility::WorldToGravityRotationByDirection(const FRotator& WorldRotation, const FVector& GravityDirection)
{
	const FQuat WorldToGravity = GetWorldToGravityTransformByDirection(GravityDirection);
	const FQuat GravityQuat = WorldToGravity * WorldRotation.Quaternion();
	return GravityQuat.Rotator();
}

FRotator UAlsGravityUtility::WorldToGravityRotationByCharacter(const FRotator& WorldRotation, const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		return WorldRotation;
	}

	return WorldToGravityRotationByDirection(WorldRotation, Character->GetCharacterMovement()->GetGravityDirection());
}

FRotator UAlsGravityUtility::GravityToWorldRotationByDirection(const FRotator& GravityRotation, const FVector& GravityDirection)
{
	const FQuat GravityToWorld = GetGravityToWorldTransformByDirection(GravityDirection);
	const FQuat WorldQuat = GravityToWorld * GravityRotation.Quaternion();
	return WorldQuat.Rotator();
}

FRotator UAlsGravityUtility::GravityToWorldRotationByCharacter(const FRotator& GravityRotation, const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		return GravityRotation;
	}

	return GravityToWorldRotationByDirection(GravityRotation, Character->GetCharacterMovement()->GetGravityDirection());
}

// === Yaw/Angle Functions ===

float UAlsGravityUtility::WorldRotationToGravityYawByDirection(const FQuat& WorldRotation, const FVector& GravityDirection)
{
	// Transform world rotation to gravity space
	const FQuat WorldToGravity = GetWorldToGravityTransformByDirection(GravityDirection);
	const FQuat GravitySpaceRotation = WorldToGravity * WorldRotation;

	// Extract yaw in gravity space (rotation around Z-up)
	const FVector GravityForward = GravitySpaceRotation.RotateVector(FVector::ForwardVector);

	return UE_REAL_TO_FLOAT(FMath::RadiansToDegrees(FMath::Atan2(GravityForward.Y, GravityForward.X)));
}

float UAlsGravityUtility::WorldRotationToGravityYawByCharacter(const FQuat& WorldRotation, const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		// Fallback to standard yaw extraction
		return UE_REAL_TO_FLOAT(WorldRotation.Rotator().Yaw);
	}

	return WorldRotationToGravityYawByDirection(WorldRotation, Character->GetCharacterMovement()->GetGravityDirection());
}

FQuat UAlsGravityUtility::GravityYawToWorldRotationByDirection(const float GravityYaw, const FVector& GravityDirection)
{
	// Create yaw rotation in gravity space (around Z-up)
	const FQuat GravitySpaceRotation(FVector::UpVector, FMath::DegreesToRadians(GravityYaw));

	// Transform to world space
	const FQuat GravityToWorld = GetGravityToWorldTransformByDirection(GravityDirection);
	return GravityToWorld * GravitySpaceRotation;
}

FQuat UAlsGravityUtility::GravityYawToWorldRotationByCharacter(const float GravityYaw, const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		// Fallback to standard rotation
		return FRotator(0.0f, GravityYaw, 0.0f).Quaternion();
	}

	return GravityYawToWorldRotationByDirection(GravityYaw, Character->GetCharacterMovement()->GetGravityDirection());
}

float UAlsGravityUtility::VectorToGravityYawByDirection(const FVector& WorldVector, const FVector& GravityDirection)
{
	// Transform vector to gravity space
	const FQuat WorldToGravity = GetWorldToGravityTransformByDirection(GravityDirection);
	const FVector GravitySpaceVector = WorldToGravity.RotateVector(WorldVector);

	// Calculate yaw angle in gravity space (angle in XY plane)
	return UE_REAL_TO_FLOAT(UAlsVector::DirectionToAngleXY(GravitySpaceVector));
}

float UAlsGravityUtility::VectorToGravityYawByCharacter(const FVector& WorldVector, const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		// Fallback to standard XY angle
		return UE_REAL_TO_FLOAT(UAlsVector::DirectionToAngleXY(WorldVector));
	}

	return VectorToGravityYawByDirection(WorldVector, Character->GetCharacterMovement()->GetGravityDirection());
}

// === Speed/Velocity Functions ===

float UAlsGravityUtility::GetGravityPlaneSpeedByDirection(const FVector& Velocity, const FVector& GravityDirection)
{
	// Project velocity onto the plane perpendicular to gravity
	const FVector GravityUp = -GravityDirection;
	const float VelocityAlongGravity = FVector::DotProduct(Velocity, GravityUp);
	const FVector VelocityInPlane = Velocity - (VelocityAlongGravity * GravityUp);

	return UE_REAL_TO_FLOAT(VelocityInPlane.Size());
}

float UAlsGravityUtility::GetGravityPlaneSpeedByCharacter(const FVector& Velocity, const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		// Fallback to 2D speed
		return UE_REAL_TO_FLOAT(Velocity.Size2D());
	}

	return GetGravityPlaneSpeedByDirection(Velocity, Character->GetCharacterMovement()->GetGravityDirection());
}

// === Movement Helper Functions ===

void UAlsGravityUtility::GetGravityAlignedMovementVectorsByDirection(const FRotator& ViewRotation, const FVector& GravityDirection,
                                                           FVector& OutForward, FVector& OutRight)
{
	// Get view direction vectors
	FVector ViewForward;
	FVector ViewRight;
	FVector ViewUp;
	UKismetMathLibrary::GetAxes(ViewRotation, ViewForward, ViewRight, ViewUp);

	const FVector GravityUp = -GravityDirection;

	// Project forward vector onto gravity plane
	const float ForwardDotGravity = FVector::DotProduct(ViewForward, GravityUp);
	OutForward = (ViewForward - ForwardDotGravity * GravityUp).GetSafeNormal();

	// Right vector is perpendicular to both forward and gravity-up
	OutRight = FVector::CrossProduct(GravityUp, OutForward).GetSafeNormal();
}

void UAlsGravityUtility::GetGravityAlignedMovementVectorsByCharacter(const FRotator& ViewRotation, const ACharacter* Character,
                                                           FVector& OutForward, FVector& OutRight)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		// Fallback to standard axes
		UKismetMathLibrary::GetAxes(ViewRotation, OutForward, OutRight, OutForward); // Reuse OutForward as dummy for Up
		OutForward.Z = 0.0f;
		OutForward.Normalize();
		OutRight.Z = 0.0f;
		OutRight.Normalize();
		return;
	}

	GetGravityAlignedMovementVectorsByDirection(ViewRotation, Character->GetCharacterMovement()->GetGravityDirection(),
	                                 OutForward, OutRight);
}

// === Utility Functions ===

bool UAlsGravityUtility::HasCustomGravity(const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		return false;
	}

	return Character->GetCharacterMovement()->HasCustomGravity();
}

FVector UAlsGravityUtility::GetGravityDirection(const ACharacter* Character)
{
	if (!IsValid(Character) || !IsValid(Character->GetCharacterMovement()))
	{
		return UCharacterMovementComponent::DefaultGravityDirection;
	}

	return Character->GetCharacterMovement()->GetGravityDirection();
}

FVector UAlsGravityUtility::GetGravityUpVector(const ACharacter* Character)
{
	return -GetGravityDirection(Character);
}