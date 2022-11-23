// Jacob Worgan 2021 for CG and Animation for Cultural Heritage, Cultural Heritage Submission

#pragma once

#include "CoreMinimal.h"
#include "Collectible.h"

#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "Components/TextRenderComponent.h"

#include "LangBound.h"

#include "LangCollectible.generated.h"

USTRUCT(BlueprintType)
struct FLangTreeData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString Name;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		USoundCue* Vocal;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FString Parent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TArray<FString> Children;
};

/**
 * 
 */
UCLASS()
class CULTURALHERITAGECPP_API ALangCollectible : public ACollectible
{
	GENERATED_BODY()
	
public:
	ALangCollectible();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		class UDataTable* LanguageInfoTable;
	TArray<FLangTreeData*> LangInfos;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		TArray<ULangBound*> LangBounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		TArray<UTextRenderComponent*> LangTexts;

	/* Functions */
	UFUNCTION(BlueprintCallable)
		void LoadDataTable();
	UFUNCTION(BlueprintCallable)
		void RegenerateLang();

	virtual void PostActorCreated() override;
	virtual void PostLoad() override;
};
