// Class a reduced version of VR_Pawn.h as part of my (Jacob Worgan's) Final Major Project (handin 15/06/2021)
// Extra code I have added for this project are bounded by ///// CULTURAL HERITAGE ... ///// \CULTURAL HERITAGE

#pragma once

#include "Engine.h"
#include "Core.h"
#include "GameFramework/Pawn.h"

#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "MotionControllerComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "XRMotionControllerBase.h"
#include "Components/SphereComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "NavMesh/RecastNavMesh.h"

#include "Runtime/HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"

#include "Collectible.h"

///// CULTURAL HERITAGE
#include "LangBoundGreaterActor.h"
#include "LangTree.h"
///// \CULTURAL HERITAGE


#include "VR_Pawn.generated.h"

UENUM(BlueprintType)
enum class EControllerLR : uint8
{
	Left = 0,
	Right = 1,
};

UCLASS()
class CULTURALHERITAGECPP_API AVR_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVR_Pawn();

	////// CULTURAL HERITAGE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		ALangBoundGreaterActor* LangBoundGreatorActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		ALangTree* CurrentLangTree;

	ULangBound* GetClosestGripLangBound(TArray<UPrimitiveComponent*> _OutOverlap, EControllerLR _ControllerLR);
	ALangBoundGreaterActor* GetClosestGripLangBoundGreater(TArray<AActor*> _OutOverlap, EControllerLR _ControllerLR);
	///// \CULTURAL HERITAGE

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	//  Scene Components  //

// VR Tracking Centre
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _hide)
		USceneComponent* VRTrackingCentre;
	// Camera at Head Position
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _hide)
		UCameraComponent* Head;
	// Text display in front of camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _hide)
		UTextRenderComponent* outputText;
	// Motion Controllers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _hide)
		UMotionControllerComponent* LeftController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _hide)
		UMotionControllerComponent* RightController;

	// TUT :: RayGun Meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _hide)
		UStaticMeshComponent* RayGun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _hide)
		UStaticMeshComponent* LeftCone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		USphereComponent* LeftBound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _hide)
		UStaticMeshComponent* RightMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
		USphereComponent* RightBound;

	// Movement Functions
	void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void Turn(float AxisValue);
	void TurnUp(float AxisValue);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _speed)
		float translationSpeed = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = _speed)
		float rotationSpeed = 1.0f;

	// Action Functions
	UFUNCTION(BlueprintCallable)
		void TeleportRightPressed();
	UFUNCTION(BlueprintCallable)
		void TeleportRightReleased();
	UFUNCTION(BlueprintCallable)
		void TeleportLeftPressed();
	UFUNCTION(BlueprintCallable)
		void TeleportLeftReleased();

	UFUNCTION(BlueprintCallable)
		void ExecuteTeleportation(EControllerLR _ControllerLR);

	bool RightTeleporterActive = false;
	bool LeftTeleporterActive = false;
	FVector TeleportLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Teleportation)
		UStaticMeshComponent* TeleportCylinder;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		ARecastNavMesh* navmesh;

	/* Grip */
	UFUNCTION(BlueprintCallable)
		void GripRightPressed();
	UFUNCTION(BlueprintCallable)
		void GripRightReleased();

	UFUNCTION(BlueprintCallable)
		void GripLeftPressed();
	UFUNCTION(BlueprintCallable)
		void GripLeftReleased();

	UFUNCTION(BlueprintCallable)
		ACollectible* GetClosestGripCollectible(TArray<AActor*> _OutOverlap, EControllerLR _ControllerLR);

	/* Remote Object Grabs */
	UFUNCTION(BlueprintCallable)
		void ShootAndGrabNoSnapPressed();
	UFUNCTION(BlueprintCallable)
		void ShootAndGrabNoSnapReleased();

	UFUNCTION(BlueprintCallable)
		void ShootAndGrabSnapPressed();
	UFUNCTION(BlueprintCallable)
		void ShootAndGrabSnapReleased();

	UFUNCTION(BlueprintCallable)
		void MagneticGripPressed();
	UFUNCTION(BlueprintCallable)
		void MagneticGripReleased();

	UFUNCTION(BlueprintCallable)
		void letGo(EControllerLR _ControllerLR);

	UFUNCTION(BlueprintCallable)
		void ForceGrab(bool shouldSnap);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		ACollectible* rightGrippedObject;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		ACollectible* leftGrippedObject;


	ACollectible* getClosestHitObject(TArray<FHitResult> hits);

	/* Action */
	UFUNCTION(BlueprintCallable)
		void ActionLeftPressed();
	UFUNCTION(BlueprintCallable)
		void ActionLeftReleased();

	UFUNCTION(BlueprintCallable)
		void ActionRightPressed();
	UFUNCTION(BlueprintCallable)
		void ActionRightReleased();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		bool IsRightActionHeld = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = VR)
		bool IsLeftActionHeld = false;
};
