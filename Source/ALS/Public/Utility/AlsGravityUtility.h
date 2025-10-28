#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AlsGravityUtility.generated.h"

class ACharacter;
class UCharacterMovementComponent;

/**
 * Utility functions for handling custom gravity in ALS.
 *
 * Note: UCharacterMovementComponent has GetGravityToWorldTransform/GetWorldToGravityTransform
 * but their names are backwards from what they actually return. These functions use correct naming.
 */
UCLASS()
class ALS_API UAlsGravityUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// === Core Transform Functions ===

	/**
	 * Get the quaternion transform from gravity space (Z-up) to world space.
	 * In gravity space, Z is always "up" relative to the gravity direction.
	 *
	 * Note: UCharacterMovementComponent::GetWorldToGravityTransform() returns this (name is backwards!)
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (ScriptName = "GetGravityToWorldTransform"))
	static FQuat GetGravityToWorldTransformByDirection(const FVector& GravityDirection);

	/** Convenience overload that gets gravity direction from character */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character", ScriptName = "GetGravityToWorldTransformFromCharacter"))
	static FQuat GetGravityToWorldTransformByCharacter(const ACharacter* Character);

	/**
	 * Get the quaternion transform from world space to gravity space (Z-up).
	 *
	 * Note: UCharacterMovementComponent::GetGravityToWorldTransform() returns this (name is backwards!)
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (ScriptName = "GetWorldToGravityTransform"))
	static FQuat GetWorldToGravityTransformByDirection(const FVector& GravityDirection);

	/** Convenience overload that gets gravity direction from character */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character", ScriptName = "GetWorldToGravityTransformFromCharacter"))
	static FQuat GetWorldToGravityTransformByCharacter(const ACharacter* Character);

	/**
	 * Convert a rotation from world space to gravity-relative space.
	 * In gravity-relative space, rotations are expressed as if Z is "up".
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (ScriptName = "WorldToGravityRotation"))
	static FRotator WorldToGravityRotationByDirection(const FRotator& WorldRotation, const FVector& GravityDirection);

	/** Convenience overload that gets gravity direction from character */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character", ScriptName = "WorldToGravityRotationFromCharacter"))
	static FRotator WorldToGravityRotationByCharacter(const FRotator& WorldRotation, const ACharacter* Character);

	/**
	 * Convert a rotation from gravity-relative space to world space.
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (ScriptName = "GravityToWorldRotation"))
	static FRotator GravityToWorldRotationByDirection(const FRotator& GravityRotation, const FVector& GravityDirection);

	/** Convenience overload that gets gravity direction from character */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character", ScriptName = "GravityToWorldRotationFromCharacter"))
	static FRotator GravityToWorldRotationByCharacter(const FRotator& GravityRotation, const ACharacter* Character);

	// === Yaw/Angle Functions ===

	/**
	 * Extract the yaw angle in gravity space from a world-space quaternion rotation.
	 * This is the rotation around the gravity-up axis.
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (ScriptName = "WorldRotationToGravityYaw"))
	static float WorldRotationToGravityYawByDirection(const FQuat& WorldRotation, const FVector& GravityDirection);

	/** Convenience overload that gets gravity direction from character */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character", ScriptName = "WorldRotationToGravityYawFromCharacter"))
	static float WorldRotationToGravityYawByCharacter(const FQuat& WorldRotation, const ACharacter* Character);

	/**
	 * Create a world-space quaternion rotation from a gravity-space yaw angle.
	 * The rotation will be around the gravity-up axis.
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (ScriptName = "GravityYawToWorldRotation"))
	static FQuat GravityYawToWorldRotationByDirection(float GravityYaw, const FVector& GravityDirection);

	/** Convenience overload that gets gravity direction from character */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character", ScriptName = "GravityYawToWorldRotationFromCharacter"))
	static FQuat GravityYawToWorldRotationByCharacter(float GravityYaw, const ACharacter* Character);

	/**
	 * Convert a world-space direction vector to a gravity-space yaw angle.
	 * This is useful for converting velocity or input direction to an angle.
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (ScriptName = "VectorToGravityYaw"))
	static float VectorToGravityYawByDirection(const FVector& WorldVector, const FVector& GravityDirection);

	/** Convenience overload that gets gravity direction from character */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character", ScriptName = "VectorToGravityYawFromCharacter"))
	static float VectorToGravityYawByCharacter(const FVector& WorldVector, const ACharacter* Character);

	// === Speed/Velocity Functions ===

	/**
	 * Calculate the speed in the gravity plane (perpendicular to gravity direction).
	 * This is the equivalent of Velocity.Size2D() for custom gravity.
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (ScriptName = "GetGravityPlaneSpeed"))
	static float GetGravityPlaneSpeedByDirection(const FVector& Velocity, const FVector& GravityDirection);

	/** Convenience overload that gets gravity direction from character */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character", ScriptName = "GetGravityPlaneSpeedFromCharacter"))
	static float GetGravityPlaneSpeedByCharacter(const FVector& Velocity, const ACharacter* Character);

	// === Movement Helper Functions ===

	/**
	 * Get forward and right vectors for movement, aligned to the gravity plane.
	 * These vectors are perpendicular to gravity and suitable for movement input.
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (ScriptName = "GetGravityAlignedMovementVectors"))
	static void GetGravityAlignedMovementVectorsByDirection(const FRotator& ViewRotation, const FVector& GravityDirection,
	                                              FVector& OutForward, FVector& OutRight);

	/** Convenience overload that gets gravity direction from character */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character", ScriptName = "GetGravityAlignedMovementVectorsFromCharacter"))
	static void GetGravityAlignedMovementVectorsByCharacter(const FRotator& ViewRotation, const ACharacter* Character,
	                                              FVector& OutForward, FVector& OutRight);

	// === Utility Functions ===

	/**
	 * Check if a character has custom gravity (non-default gravity direction).
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character"))
	static bool HasCustomGravity(const ACharacter* Character);

	/**
	 * Get the gravity direction for a character.
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character"))
	static FVector GetGravityDirection(const ACharacter* Character);

	/**
	 * Get the gravity up vector (opposite of gravity direction) for a character.
	 */
	UFUNCTION(BlueprintPure, Category = "ALS|Gravity Utility", meta = (DefaultToSelf = "Character"))
	static FVector GetGravityUpVector(const ACharacter* Character);

	// === C++ Convenience Wrappers (not exposed to Blueprints) ===
	// These provide clean function names for C++ code while maintaining Blueprint compatibility

	static FORCEINLINE FQuat GetGravityToWorldTransform(const FVector& GravityDirection)
	{
		return GetGravityToWorldTransformByDirection(GravityDirection);
	}

	static FORCEINLINE FQuat GetGravityToWorldTransform(const ACharacter* Character)
	{
		return GetGravityToWorldTransformByCharacter(Character);
	}

	static FORCEINLINE FQuat GetWorldToGravityTransform(const FVector& GravityDirection)
	{
		return GetWorldToGravityTransformByDirection(GravityDirection);
	}

	static FORCEINLINE FQuat GetWorldToGravityTransform(const ACharacter* Character)
	{
		return GetWorldToGravityTransformByCharacter(Character);
	}

	static FORCEINLINE FRotator WorldToGravityRotation(const FRotator& WorldRotation, const FVector& GravityDirection)
	{
		return WorldToGravityRotationByDirection(WorldRotation, GravityDirection);
	}

	static FORCEINLINE FRotator WorldToGravityRotation(const FRotator& WorldRotation, const ACharacter* Character)
	{
		return WorldToGravityRotationByCharacter(WorldRotation, Character);
	}

	static FORCEINLINE FRotator GravityToWorldRotation(const FRotator& GravityRotation, const FVector& GravityDirection)
	{
		return GravityToWorldRotationByDirection(GravityRotation, GravityDirection);
	}

	static FORCEINLINE FRotator GravityToWorldRotation(const FRotator& GravityRotation, const ACharacter* Character)
	{
		return GravityToWorldRotationByCharacter(GravityRotation, Character);
	}

	static FORCEINLINE float WorldRotationToGravityYaw(const FQuat& WorldRotation, const FVector& GravityDirection)
	{
		return WorldRotationToGravityYawByDirection(WorldRotation, GravityDirection);
	}

	static FORCEINLINE float WorldRotationToGravityYaw(const FQuat& WorldRotation, const ACharacter* Character)
	{
		return WorldRotationToGravityYawByCharacter(WorldRotation, Character);
	}

	static FORCEINLINE FQuat GravityYawToWorldRotation(float GravityYaw, const FVector& GravityDirection)
	{
		return GravityYawToWorldRotationByDirection(GravityYaw, GravityDirection);
	}

	static FORCEINLINE FQuat GravityYawToWorldRotation(float GravityYaw, const ACharacter* Character)
	{
		return GravityYawToWorldRotationByCharacter(GravityYaw, Character);
	}

	static FORCEINLINE float VectorToGravityYaw(const FVector& WorldVector, const FVector& GravityDirection)
	{
		return VectorToGravityYawByDirection(WorldVector, GravityDirection);
	}

	static FORCEINLINE float VectorToGravityYaw(const FVector& WorldVector, const ACharacter* Character)
	{
		return VectorToGravityYawByCharacter(WorldVector, Character);
	}

	static FORCEINLINE float GetGravityPlaneSpeed(const FVector& Velocity, const FVector& GravityDirection)
	{
		return GetGravityPlaneSpeedByDirection(Velocity, GravityDirection);
	}

	static FORCEINLINE float GetGravityPlaneSpeed(const FVector& Velocity, const ACharacter* Character)
	{
		return GetGravityPlaneSpeedByCharacter(Velocity, Character);
	}

	static FORCEINLINE void GetGravityAlignedMovementVectors(const FRotator& ViewRotation, const FVector& GravityDirection,
	                                                          FVector& OutForward, FVector& OutRight)
	{
		GetGravityAlignedMovementVectorsByDirection(ViewRotation, GravityDirection, OutForward, OutRight);
	}

	static FORCEINLINE void GetGravityAlignedMovementVectors(const FRotator& ViewRotation, const ACharacter* Character,
	                                                          FVector& OutForward, FVector& OutRight)
	{
		GetGravityAlignedMovementVectorsByCharacter(ViewRotation, Character, OutForward, OutRight);
	}
};