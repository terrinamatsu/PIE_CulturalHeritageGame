// Jacob Worgan 2021 for CG and Animation for Cultural Heritage, Cultural Heritage Submission

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "Components/TextRenderComponent.h"
#include "Sound/SoundCue.h"


#include "LangBound.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (LangBound), meta = (BlueprintSpawnableComponent))
class CULTURALHERITAGECPP_API ULangBound : public UBoxComponent
{
	GENERATED_BODY()
	
public: 
	// Bound Info
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		FString BoundWord;
	// Add: Sound Recording
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		USoundCue* Vocal;

	//// Tree Structure
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
	//	TArray<ULangBound*> ChildLangBounds;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
	//	ULangBound* ParentLangBound;

	//UFUNCTION()
	//	void AddParentLB(ULangBound* Parent);
	//UFUNCTION()
	//	void AddChildLB(ULangBound* Child);
	//UFUNCTION()
	//	void AddTextLB(UTextRenderComponent* TextRender);

	// Text
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Info)
		UTextRenderComponent* Text;*/
};
