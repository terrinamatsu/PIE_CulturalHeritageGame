// Class a reduced version of Collectible.cpp as part of my (Jacob Worgan's) Final Major Project (handin 15/06/2021)


#include "Collectible.h"

ACollectible::ACollectible()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set Default Mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> loadedObj(TEXT("/Game/Geometry/Meshes/1M_Cube"));
	this->GetStaticMeshComponent()->SetStaticMesh(loadedObj.Object);

	// Set Scale
	this->GetStaticMeshComponent()->SetWorldScale3D(FVector(0.2f, 0.2f, 0.2f));

	// Set Physics
	this->GetStaticMeshComponent()->SetSimulatePhysics(true);
	this->SetMobility(EComponentMobility::Movable);

	// Set Tags
	this->GetStaticMeshComponent()->SetCanEverAffectNavigation(false);
	this->GetStaticMeshComponent()->SetGenerateOverlapEvents(true);

	// Set Mass
	this->GetStaticMeshComponent()->BodyInstance.bOverrideMass = true;
	this->GetStaticMeshComponent()->BodyInstance.SetMassOverride(5.0f);

	// Make it a Physics Body
	this->GetStaticMeshComponent()->SetNotifyRigidBodyCollision(true);
	this->GetStaticMeshComponent()->SetCollisionProfileName("PhysicsActor");
}

// Called every frame
void ACollectible::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACollectible::Drop()
{
	// Detach & Enable Physics on Object
	this->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	this->GetStaticMeshComponent()->SetSimulatePhysics(true);

	//rightGrippedObject->GetStaticMeshComponent()->SetEnableGravity(true);
	//ToggleGlow(false);

	IsPickedUp = false;
}

void ACollectible::Grab(USceneComponent* AttachTo)
{
	// Attach & Disable Physics on Object
	//RootComponent->AttachToComponent(AttachTo, FAttachmentTransformRules::KeepWorldTransform);
	this->GetStaticMeshComponent()->SetSimulatePhysics(false);

	//trycast->GetStaticMeshComponent()->SetEnableGravity(false);

	IsPickedUp = true;
}


void ACollectible::ObjectActionPress_Implementation()
{
	// Nothing, to be implemented by child classes
}

void ACollectible::ObjectActionHold_Implementation(float DeltaTime)
{
	// Nada, to be implemented by child classes
}

void ACollectible::ObjectActionRelease_Implementation()
{
	// Rien, to be implemented by child classes
}