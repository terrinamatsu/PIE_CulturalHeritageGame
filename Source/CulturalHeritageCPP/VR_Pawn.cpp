// Class a reduced version of VR_Pawn.cpp as part of my (Jacob Worgan's) Final Major Project (handin 15/06/2021)


#include "VR_Pawn.h"

// Sets default values
AVR_Pawn::AVR_Pawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add Components To Scene
	VRTrackingCentre = CreateDefaultSubobject<USceneComponent>(TEXT("VRTackingCentre"));
	Head = CreateDefaultSubobject<UCameraComponent>(TEXT("Head"));
	outputText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("outputText"));

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftController"));
	RayGun = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RayGun"));
	LeftCone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftCone"));

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightController"));
	RightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightMesh"));

	TeleportCylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportCylinder"));

	// Set Attachments
	this->RootComponent = VRTrackingCentre;

	Head->SetupAttachment(VRTrackingCentre);
	outputText->SetupAttachment(Head);

	LeftController->SetupAttachment(VRTrackingCentre);
	LeftCone->SetupAttachment(LeftController);
	RayGun->SetupAttachment(LeftCone);

	RightController->SetupAttachment(VRTrackingCentre);
	RightMesh->SetupAttachment(RightController);

	TeleportCylinder->SetupAttachment(RootComponent);

	// Import Static Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> loadedObj(TEXT("StaticMesh'/Game/StarterContent/Geometry/Meshes/1M_Cube.1M_Cube'"));
	this->RayGun->SetStaticMesh(loadedObj.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> cone(TEXT("StaticMesh'/Game/Meshes/__Controller/Hand/Only_hand_mash.Only_hand_mash'"));
	this->LeftCone->SetStaticMesh(cone.Object);
	this->RightMesh->SetStaticMesh(cone.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> cylinder(TEXT("StaticMesh'/Engine/BasicShapes/Cylinder.Cylinder'"));
	this->TeleportCylinder->SetStaticMesh(cylinder.Object);
	this->TeleportCylinder->SetVisibility(false);

	// Import Material
	static ConstructorHelpers::FObjectFinder<UMaterial> greymat(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

	LeftCone->SetMaterial(0, greymat.Object);
	RightMesh->SetMaterial(0, greymat.Object);

	// Set Motion Controller Hands
	RightController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	LeftController->MotionSource = FXRMotionControllerBase::LeftHandSourceId;

	// Scale Objects
	RayGun->SetRelativeLocation(FVector(15, 0, 0));
	RayGun->SetRelativeScale3D(FVector(0.1f, 0.1f, 0.1f));

	//LeftCone->SetRelativeLocation(FVector(26, 0, 6));
	LeftCone->SetRelativeRotation(FRotator(-90, 180, 540));
	LeftCone->SetRelativeScale3D(FVector(1.0f, -1.0f, 1.0f));

	RightMesh->SetRelativeRotation(FRotator(-90, 180, 180));
	RightMesh->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	TeleportCylinder->SetRelativeScale3D(FVector(0.5, 0.5, 0.5));

	// Setup Text
	static ConstructorHelpers::FObjectFinder<UMaterial> unlitText(TEXT("Material'/Engine/EngineMaterials/DefaultTextMaterialTranslucent.DefaultTextMaterialTranslucent'"));
	outputText->SetMaterial(0, unlitText.Object);
	outputText->SetTextRenderColor(FColor::Red);

	outputText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	outputText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;
	outputText->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	outputText->SetRelativeLocation(FVector(150.0f, 0.0f, 0.0f));

	// Set Player to Receive Input
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Set Physics & Collision
	RayGun->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	LeftCone->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	RightMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	RayGun->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	LeftCone->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RightMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);

	/* Hand Bound Spheres */
	RightBound = CreateDefaultSubobject<USphereComponent>(TEXT("RightBound"));
	RightBound->SetupAttachment(RightController);
	RightBound->SetRelativeLocation(FVector(0, 0, 0));
	RightBound->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.1f));
	RightBound->SetHiddenInGame(true);    // hidden in game
	RightBound->SetGenerateOverlapEvents(true);    // Generate OverLap Events
	//RightBound->OnComponentBeginOverlap.AddDynamic(this, &ACauldron::OnBoundBeginOverlap);

	LeftBound = CreateDefaultSubobject<USphereComponent>(TEXT("LeftBound"));
	LeftBound->SetupAttachment(LeftController);
	LeftBound->SetRelativeLocation(FVector(0, 0, 0));
	LeftBound->SetRelativeScale3D(FVector(1.1f, 1.1f, 1.1f));
	LeftBound->SetHiddenInGame(true);    // hidden in game
	LeftBound->SetGenerateOverlapEvents(true);    // Generate OverLap Events
	//LeftBound->OnComponentBeginOverlap.AddDynamic(this, &ACauldron::OnBoundBeginOverlap);

	// Set Up NavMesh
	navmesh = dynamic_cast<ARecastNavMesh*>(UGameplayStatics::GetActorOfClass(GetWorld(), ARecastNavMesh::StaticClass()));
}

// Called when the game starts or when spawned
void AVR_Pawn::BeginPlay()
{
	Super::BeginPlay();

	// Set the tracking origin to the floor if not using PSVR headset
	if (UHeadMountedDisplayFunctionLibrary::GetHMDDeviceName() != "PSVR")
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::Floor);
	}
}

// Called every frame
void AVR_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	/* Teleportation */
	if (LeftTeleporterActive)
	{
		// Make Trace Hit Result
		FHitResult hit;

		// Set Up Trace Ignore Array
		TArray<AActor*> ignored;
		if (rightGrippedObject) ignored.Add(rightGrippedObject);
		if (leftGrippedObject) ignored.Add(leftGrippedObject);
		ignored.Add(this); // Not really necessary

		// Trace
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),
			LeftController->GetComponentLocation(),
			LeftController->GetComponentLocation() + (LeftCone->GetUpVector() * 10000.0f),
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic),
			false,
			ignored,
			EDrawDebugTrace::None,
			hit,
			true))
		{
			// Show the text of the hit actor
			if (hit.GetActor())
			{
				this->outputText->SetText(hit.GetActor()->GetName());
			}

			// Move the Teleport Cylinder location to the new point, if valid
			FNavLocation outnav;

			FBox b = FBox(FVector(-1000, -1000, -1000), FVector(1000, 1000, 1000));
			TArray<FNavPoly> pol;
			navmesh->GetPolysInBox(b, pol);


			if (navmesh->ProjectPoint(hit.ImpactPoint, outnav, FVector(1000, 1000, 1000)))
			{
				TeleportLocation = (outnav.Location);
				TeleportCylinder->SetWorldLocation(TeleportLocation);
			}
		}
	}
	if (RightTeleporterActive)
	{
		// Make Trace Hit Result
		FHitResult hit;

		// Set Up Trace Ignore Array
		TArray<AActor*> ignored;
		if (rightGrippedObject) ignored.Add(rightGrippedObject);
		if (leftGrippedObject) ignored.Add(leftGrippedObject);
		ignored.Add(this); // Not really necessary

		// Trace
		if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),
			RightController->GetComponentLocation(),
			RightController->GetComponentLocation() + (RightMesh->GetUpVector() * 10000.0f),
			UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic),
			false,
			ignored,
			EDrawDebugTrace::None,
			hit,
			true))
		{
			// Show the text of the hit actor
			if (hit.GetActor())
			{
				this->outputText->SetText(hit.GetActor()->GetName());
			}

			// Move the Teleport Cylinder location to the new point, if valid
			FNavLocation outnav;

			if (navmesh->ProjectPoint(hit.ImpactPoint, outnav, FVector(1000, 1000, 1000)))
			{
				TeleportLocation = (outnav.Location);
				TeleportCylinder->SetWorldLocation(TeleportLocation);
			}
		}
	}

	/* Object Action */
	if (IsLeftActionHeld)
	{
		leftGrippedObject->ObjectActionHold(DeltaTime);
	}
	if (IsRightActionHeld)
	{
		rightGrippedObject->ObjectActionHold(DeltaTime);
	}
}

// Called to bind functionality to input
void AVR_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Actions

	// Teleportation
	PlayerInputComponent->BindAction("TeleportRight", EInputEvent::IE_Pressed, this, &AVR_Pawn::TeleportRightPressed);
	PlayerInputComponent->BindAction("TeleportRight", EInputEvent::IE_Released, this, &AVR_Pawn::TeleportRightReleased);

	PlayerInputComponent->BindAction("TeleportLeft", EInputEvent::IE_Pressed, this, &AVR_Pawn::TeleportLeftPressed);
	PlayerInputComponent->BindAction("TeleportLeft", EInputEvent::IE_Released, this, &AVR_Pawn::TeleportLeftReleased);

	// Gripping
	PlayerInputComponent->BindAction("GripRight", EInputEvent::IE_Pressed, this, &AVR_Pawn::GripRightPressed);
	PlayerInputComponent->BindAction("GripRight", EInputEvent::IE_Released, this, &AVR_Pawn::GripRightReleased);

	PlayerInputComponent->BindAction("GripLeft", EInputEvent::IE_Pressed, this, &AVR_Pawn::GripLeftPressed);
	PlayerInputComponent->BindAction("GripLeft", EInputEvent::IE_Released, this, &AVR_Pawn::GripLeftReleased);


	PlayerInputComponent->BindAction("ShootAndGrabNoSnap", EInputEvent::IE_Pressed, this, &AVR_Pawn::ShootAndGrabNoSnapPressed);
	PlayerInputComponent->BindAction("ShootAndGrabNoSnap", EInputEvent::IE_Released, this, &AVR_Pawn::ShootAndGrabNoSnapReleased);

	PlayerInputComponent->BindAction("ShootAndGrabSnap", EInputEvent::IE_Pressed, this, &AVR_Pawn::ShootAndGrabSnapPressed);
	PlayerInputComponent->BindAction("ShootAndGrabSnap", EInputEvent::IE_Released, this, &AVR_Pawn::ShootAndGrabSnapReleased);

	PlayerInputComponent->BindAction("MagneticGrip", EInputEvent::IE_Pressed, this, &AVR_Pawn::MagneticGripPressed);
	PlayerInputComponent->BindAction("MagneticGrip", EInputEvent::IE_Released, this, &AVR_Pawn::MagneticGripReleased);

	// Action
	PlayerInputComponent->BindAction("ActionLeft", EInputEvent::IE_Pressed, this, &AVR_Pawn::ActionLeftPressed);
	PlayerInputComponent->BindAction("ActionLeft", EInputEvent::IE_Released, this, &AVR_Pawn::ActionLeftReleased);

	PlayerInputComponent->BindAction("ActionRight", EInputEvent::IE_Pressed, this, &AVR_Pawn::ActionRightPressed);
	PlayerInputComponent->BindAction("ActionRight", EInputEvent::IE_Released, this, &AVR_Pawn::ActionRightReleased);

	// Bind Axis
	PlayerInputComponent->BindAxis("MoveForward", this, &AVR_Pawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AVR_Pawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AVR_Pawn::Turn);
	//PlayerInputComponent->BindAxis("LookUp", this, &AVR_Pawn::TurnUp);
}

void AVR_Pawn::MoveForward(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		// add movement in that direction
		this->AddMovementInput(Head->GetForwardVector(), AxisValue * translationSpeed);
		//this->AddActorWorldOffset(Head->GetForwardVector() * AxisValue * translationSpeed);
	}
}

void AVR_Pawn::MoveRight(float AxisValue)
{
	if (AxisValue != 0.0f)
	{
		// add movement in that direction
		this->AddMovementInput(Head->GetRightVector(), AxisValue * translationSpeed);
		//this->AddActorWorldOffset(Head->GetRightVector() * AxisValue * translationSpeed);
	}
}

void AVR_Pawn::Turn(float AxisValue)
{
	//// [ToDo::] Set Rotation Centre as Headset Location
	//FVector CurLoc = this->GetActorLocation();

	//FRotator HeadOrientation;
	//FVector HeadLoc;
	//UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HeadOrientation, HeadLoc);

	////CurLoc.RotateAngleAxis(AxisValue * rotationSpeed, HeadLoc);
	////Head->AddRelativeRotation(FRotator(0, AxisValue * rotationSpeed, 0));

	//this->SetActorLocation(CurLoc);

	////this->AddActorWorldRotation(FRotator(0, AxisValue * rotationSpeed, 0));

	FVector CurLoc = VRTrackingCentre->GetComponentLocation();

	FRotator HeadOrientation;
	FVector HeadLoc;// = Head->GetComponentLocation();
	UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(HeadOrientation, HeadLoc);

	GLog->Log(FString::Printf(TEXT("%f, %f, %f"), HeadLoc.X, HeadLoc.Y, HeadLoc.Z));

	FVector NewVec = CurLoc - HeadLoc;
	NewVec.RotateAngleAxis(AxisValue * rotationSpeed, FVector(0.0f, 0.0f, 1.0f));
	NewVec += HeadLoc;

	this->SetActorLocation(NewVec);
	this->AddActorLocalRotation(FRotator(0, AxisValue * rotationSpeed, 0));
}

void AVR_Pawn::TurnUp(float AxisValue)
{
	/*
		Handles vertical camera movement outside of VR.
	*/
	// Currently disabled, only should update when not in VR, but I don't know how to check for this yet.
	this->AddActorLocalRotation(FRotator(-AxisValue * rotationSpeed, 0, 0));
}

void AVR_Pawn::TeleportRightPressed()
{
	// Activate Right Teleporter
		/*
			Set true IsTeleporterActive
			Set visible TeleportCylinder



		*/

		// Disable Left Teleporter

	RightTeleporterActive = true;
	LeftTeleporterActive = false;

	TeleportCylinder->SetVisibility(true);
}

void AVR_Pawn::TeleportRightReleased()
{
	// If Teleporter is active
	// Execute Teleportation

	ExecuteTeleportation(EControllerLR::Right);

	/*
	if (RightTeleporterActive)
	{
		RightTeleporterActive = false;
		this->SetActorLocation(TeleportLocation);
		TeleportCylinder->SetVisibility(false);
	}*/
}

void AVR_Pawn::TeleportLeftPressed()
{
	LeftTeleporterActive = true;
	RightTeleporterActive = false;

	TeleportCylinder->SetVisibility(true);
}

void AVR_Pawn::TeleportLeftReleased()
{
	// If Teleporter is active
	// Execute Teleportation

	ExecuteTeleportation(EControllerLR::Left);

	/*
	if (LeftTeleporterActive)
	{
		LeftTeleporterActive = false;
		this->SetActorLocation(TeleportLocation);
		TeleportCylinder->SetVisibility(false);
	}*/
}

void AVR_Pawn::ExecuteTeleportation(EControllerLR _ControllerLR)
{
	// If A Teleporter is Active
	if ((_ControllerLR == EControllerLR::Left) ? LeftTeleporterActive : RightTeleporterActive)
	{
		LeftTeleporterActive = false;
		RightTeleporterActive = false;

		// Moving Teleport Location Relative to Headset Location (from tracking centre)
		FRotator Orientation;
		FVector Position;
		//UHeadMountedDisplayFunctionLibrary::GetOrientationAndPosition(Orientation, Position);
		Position = Head->GetRelativeLocation();
		Orientation = Head->GetRelativeRotation();
		TeleportLocation -= FVector(Position.X, Position.Y, 0.0f);

		GLog->Log("ASDFKH");
		GLog->Log(FString::Printf(TEXT("%d, %d"), Position.X, Position.Y));

		// Fade Out
		APlayerCameraManager* cam = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (cam)
		{
			cam->StartCameraFade(1.0f, 0.0f, 0.1f, FLinearColor::Black, false, false);
		}

		// Teleport
		this->SetActorLocation(TeleportLocation);
		TeleportCylinder->SetVisibility(false);
	}
}

void AVR_Pawn::GripRightPressed()
{
	// Get Overlapping Actors
	TArray<AActor*> outOverlap;
	RightBound->GetOverlappingActors(outOverlap);

	if (outOverlap.Num() > 0)
	{
		// Get Closest of the Collectible Class
		ACollectible* trycast = GetClosestGripCollectible(outOverlap, EControllerLR::Right);

		if (trycast)
		{
			this->outputText->SetText(trycast->GetName());

			trycast->Grab(RightController);
			trycast->AttachToComponent(RightController, FAttachmentTransformRules::KeepWorldTransform);

			rightGrippedObject = trycast;
		}
	}
}
void AVR_Pawn::GripRightReleased()
{
	letGo(EControllerLR::Right);
}

ACollectible* AVR_Pawn::GetClosestGripCollectible(TArray<AActor*> _OutOverlap, EControllerLR _ControllerLR)
{
	FVector HandLoc = (_ControllerLR == EControllerLR::Left) ? LeftController->GetComponentLocation() : RightController->GetComponentLocation();
	ACollectible* ClosestCollectible = dynamic_cast<ACollectible*>(_OutOverlap[0]);
	float ClosestDistance = 99999999.9f;

	for (auto& overlap : _OutOverlap)
	{
		ACollectible* trycast = dynamic_cast<ACollectible*>(overlap);

		if (trycast)
		{
			// Calculate Distance to Hand
			float CurrentDistance = (HandLoc - trycast->GetActorLocation()).Size();

			// If Greater than the current longest distance...
			if (CurrentDistance < ClosestDistance)
			{
				//...Set this as the current Closest Collectible
				ClosestCollectible = trycast;
				ClosestDistance = CurrentDistance;
			}
		}
	}

	return ClosestCollectible;
}

void AVR_Pawn::GripLeftPressed()
{
	// Get Overlapping Actors
	TArray<AActor*> outOverlap;
	LeftBound->GetOverlappingActors(outOverlap);

	if (outOverlap.Num() > 0)
	{
		// Get Closest of the Collectible Class
		ACollectible* trycast = GetClosestGripCollectible(outOverlap, EControllerLR::Left);

		if (trycast)
		{
			this->outputText->SetText(trycast->GetName());

			trycast->Grab(LeftController);
			trycast->AttachToComponent(LeftController, FAttachmentTransformRules::KeepWorldTransform);

			leftGrippedObject = trycast;
		}
	}

	///// CULTURAL HERITAGE
	//TArray<UPrimitiveComponent*> outOverlap2;
	//LeftBound->GetOverlappingComponents(outOverlap2);

	// If overlapping a LangBoundGreater, spawn a LangTree. 
	if (outOverlap.Num() > 0)
	{
		// Get Closest of the LangBoundGreater Class
		ALangBoundGreaterActor* trycast = GetClosestGripLangBoundGreater(outOverlap, EControllerLR::Left);

		if (trycast)
		{
			// spawn LangTree
			const FVector loc = Head->GetComponentLocation() + (Head->GetForwardVector() * 50);
			const FRotator rot = UKismetMathLibrary::MakeRotFromX(-Head->GetForwardVector());
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			CurrentLangTree = GetWorld()->SpawnActor<ALangTree>(trycast->LangTreeToSpawn.Get(), loc, rot, SpawnInfo);
		}
	}

	///// \CULTURAL HERITAGE
}

void AVR_Pawn::GripLeftReleased()
{
	letGo(EControllerLR::Left);

	///// CULTURAL HERITAGE
	if (CurrentLangTree)
	{
		CurrentLangTree->Destroy();
	}
	///// \CULTURAL HERITAGE
}

void AVR_Pawn::ShootAndGrabNoSnapPressed()
{
	this->outputText->SetText("Shoot and Grab no Snap");

	ForceGrab(false);
}

void AVR_Pawn::ShootAndGrabNoSnapReleased()
{
	this->outputText->SetText("Shoot and Grab no Snap Released");

	letGo(EControllerLR::Right);
}

void AVR_Pawn::ForceGrab(bool shouldSnap)
{
	// Make Trace Hit Result
	FHitResult hit;

	// Set Up Trace Ignore Array
	TArray<AActor*> ignored;
	ignored.Add(this); // Not really necessary


	// Trace
	if (UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		RightController->GetComponentLocation(),
		RightController->GetComponentLocation() + (RightMesh->GetUpVector() * 10000.0f),
		UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_WorldStatic),
		false,
		ignored,
		EDrawDebugTrace::None,
		hit,
		true))
	{
		if (hit.GetActor())
		{
			// Cast to Collectible Class
			ACollectible* trycast = dynamic_cast<ACollectible*>(hit.GetActor());
			if (trycast)
			{
				this->outputText->SetText("Succeeded");
				// Disable Physics on object, disbale gravity, attach to Controller
				trycast->GetStaticMeshComponent()->SetSimulatePhysics(false);
				trycast->AttachToComponent(RightController, shouldSnap ? FAttachmentTransformRules::SnapToTargetNotIncludingScale : FAttachmentTransformRules::KeepWorldTransform);

				rightGrippedObject = trycast;
			}
		}
	}
}

void AVR_Pawn::ShootAndGrabSnapPressed()
{
	this->outputText->SetText("Shoot and Grap Snap");

	ForceGrab(true);
}
void AVR_Pawn::ShootAndGrabSnapReleased()
{
	this->outputText->SetText("Shoot and Grab Snap Released");

	letGo(EControllerLR::Right);
}

void AVR_Pawn::MagneticGripPressed()
{
	this->outputText->SetText("Mag Grip");

	// Make Trace Hits Result
	TArray<FHitResult> hits;

	// Set Up Trace Ignore Array
	TArray<AActor*> ignored;
	ignored.Add(this); // Not really necessary

	TArray<TEnumAsByte<EObjectTypeQuery>> objTypes;
	objTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_PhysicsBody));


	if (UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), //world
		RightMesh->GetComponentLocation(), //start
		RightMesh->GetComponentLocation() + FVector(0, 0, 0.1f), //end, has to be different from start
		1000, //sphere radius
		objTypes, //object types
		false, //trace complex?
		ignored, //ignored actors
		EDrawDebugTrace::None, //debug draw type
		hits, //out hit
		true)) //ignore self
	{
		ACollectible* closestObject = getClosestHitObject(hits);

		if (closestObject)
		{
			closestObject->GetStaticMeshComponent()->SetSimulatePhysics(false);
			closestObject->AttachToComponent(RightBound, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

			closestObject->Grab(RightController);

			rightGrippedObject = closestObject;
		}
	}
}
void AVR_Pawn::MagneticGripReleased()
{
	this->outputText->SetText("Mag Grip Released");

	letGo(EControllerLR::Right);
}

ACollectible* AVR_Pawn::getClosestHitObject(TArray<FHitResult> hits)
{
	float cloasestDistance = 10000000.0f;
	ACollectible* closestObject = nullptr;

	for (FHitResult hit : hits)
	{
		ACollectible* trycast = dynamic_cast<ACollectible*>(hit.GetActor());
		if (trycast)
		{
			if ((RightMesh->GetComponentLocation() - trycast->GetActorLocation()).Size() < cloasestDistance)
			{
				cloasestDistance = (RightMesh->GetComponentLocation() - trycast->GetActorLocation()).Size();
				closestObject = trycast;
			}
		}
	}

	return closestObject;
}

void AVR_Pawn::letGo(EControllerLR _ControllerLR)
{
	// Set Controller
	ACollectible* GrippedObject = (_ControllerLR == EControllerLR::Left) ? leftGrippedObject : rightGrippedObject;
	UMotionControllerComponent* ControllerComp = (_ControllerLR == EControllerLR::Left) ? LeftController : RightController;

	//If Object is valid
	if (GrippedObject)
	{
		// If rightObject's parent is right controller
		if (GrippedObject->GetRootComponent()->GetAttachParent() == ControllerComp)
		{
			GrippedObject->Drop();

			GrippedObject = nullptr;
		}
	}
}

void AVR_Pawn::ActionLeftPressed()
{
	if (leftGrippedObject)
	{
		IsLeftActionHeld = true;

		leftGrippedObject->ObjectActionPress();

		leftGrippedObject->Controller = LeftController;
	}
}

void AVR_Pawn::ActionLeftReleased()
{
	if (leftGrippedObject)
	{
		IsLeftActionHeld = false;

		leftGrippedObject->ObjectActionRelease();
	}
}


void AVR_Pawn::ActionRightPressed()
{
	if (rightGrippedObject)
	{
		IsRightActionHeld = true;

		rightGrippedObject->ObjectActionPress();

		rightGrippedObject->Controller = RightController;
	}

	///// CULTURAL HERITAGE
	//if overlapping a LangBound, play its sound.
	// Get Overlapping Actors
	TArray<UPrimitiveComponent*> outOverlap;
	RightBound->GetOverlappingComponents(outOverlap);

	if (outOverlap.Num() > 0)
	{
		// Get Closest of the Collectible Class

		ULangBound* trycast = GetClosestGripLangBound(outOverlap, EControllerLR::Right);

		if (trycast)
		{
			//Play Sound Here
			if (trycast->Vocal)
			{
				UAudioComponent* Audio = UGameplayStatics::SpawnSound2D(this, trycast->Vocal);
			}
		}
	}
	///// \CULTURAL HERITAGE
}

///// CULTURAL HERITAGE
ULangBound* AVR_Pawn::GetClosestGripLangBound(TArray<UPrimitiveComponent*> _OutOverlap, EControllerLR _ControllerLR)
{
	FVector HandLoc = (_ControllerLR == EControllerLR::Left) ? LeftController->GetComponentLocation() : RightController->GetComponentLocation();
	ULangBound* ClosestLangBound = dynamic_cast<ULangBound*>(_OutOverlap[0]);
	float ClosestDistance = 99999999.9f;

	for (auto& overlap : _OutOverlap)
	{
		ULangBound* trycast = dynamic_cast<ULangBound*>(overlap);

		if (trycast)
		{
			// Calculate Distance to Hand
			float CurrentDistance = (HandLoc - trycast->GetComponentLocation()).Size();

			// If Greater than the current longest distance...
			if (CurrentDistance < ClosestDistance)
			{
				//...Set this as the current Closest Collectible
				ClosestLangBound = trycast;
				ClosestDistance = CurrentDistance;
			}
		}
	}

	return ClosestLangBound;
}

ALangBoundGreaterActor* AVR_Pawn::GetClosestGripLangBoundGreater(TArray<AActor*> _OutOverlap, EControllerLR _ControllerLR)
{
	FVector HandLoc = (_ControllerLR == EControllerLR::Left) ? LeftController->GetComponentLocation() : RightController->GetComponentLocation();
	ALangBoundGreaterActor* ClosestLangBoundGreater = dynamic_cast<ALangBoundGreaterActor*>(_OutOverlap[0]);
	float ClosestDistance = 99999999.9f;

	for (auto& overlap : _OutOverlap)
	{
		ALangBoundGreaterActor* trycast = dynamic_cast<ALangBoundGreaterActor*>(overlap);

		if (trycast)
		{
			// Calculate Distance to Hand
			float CurrentDistance = (HandLoc - trycast->GetActorLocation()).Size();

			// If Greater than the current longest distance...
			if (CurrentDistance < ClosestDistance)
			{
				//...Set this as the current Closest Collectible
				ClosestLangBoundGreater = trycast;
				ClosestDistance = CurrentDistance;
			}
		}
	}

	return ClosestLangBoundGreater;
}
///// \CULTURAL HERITAGE

void AVR_Pawn::ActionRightReleased()
{
	if (rightGrippedObject)
	{
		IsRightActionHeld = false;

		rightGrippedObject->ObjectActionRelease();
	}
}