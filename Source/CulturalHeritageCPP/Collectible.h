// Class a reduced version of Collectible.h as part of my (Jacob Worgan's) Final Major Project (handin 15/06/2021)

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"

#include "MotionControllerComponent.h"

#include "UObject/ConstructorHelpers.h"
#include "Materials/Material.h"
#include "Engine/StaticMesh.h"

#include "Collectible.generated.h"

/**
 *  Base Class for any item that can be picked up by the VRPawn player character
 */
UCLASS()
class CULTURALHERITAGECPP_API ACollectible : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:
	ACollectible();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/* Information */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Details)
		FString Name;

	/* Object Grab */
	bool IsPickedUp = false;

	UFUNCTION()
		virtual void Grab(USceneComponent* AttachTo);
	UFUNCTION()
		virtual void Drop();

	/* Object Actions */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Action)
		UMotionControllerComponent* Controller;

	// On Press
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ObjectActionPress();
	virtual void ObjectActionPress_Implementation();

	// On Hold (Called every tick when held)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ObjectActionHold(float DeltaTime);
	virtual void ObjectActionHold_Implementation(float DeltaTime);

	// On Release
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void ObjectActionRelease();
	virtual void ObjectActionRelease_Implementation();

};
