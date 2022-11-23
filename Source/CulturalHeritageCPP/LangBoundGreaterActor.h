// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "LangBoundGreater.h"

#include "LangBoundGreaterActor.generated.h"

UCLASS()
class CULTURALHERITAGECPP_API ALangBoundGreaterActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALangBoundGreaterActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		ULangBoundGreater* LangBoundGreater;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		TSubclassOf<ALangTree> LangTreeToSpawn;
};
