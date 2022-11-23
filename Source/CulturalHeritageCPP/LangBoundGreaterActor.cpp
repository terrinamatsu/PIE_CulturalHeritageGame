// Fill out your copyright notice in the Description page of Project Settings.


#include "LangBoundGreaterActor.h"

// Sets default values
ALangBoundGreaterActor::ALangBoundGreaterActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LangBoundGreater = CreateDefaultSubobject<ULangBoundGreater>("LangBoundGreater");
}

// Called when the game starts or when spawned
void ALangBoundGreaterActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALangBoundGreaterActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

