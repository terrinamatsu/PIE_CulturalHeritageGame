// Jacob Worgan 2021 for CG and Animation for Cultural Heritage, Cultural Heritage Submission


#include "LangCollectible.h"

ALangCollectible::ALangCollectible()
{
	RegenerateLang();
}

void ALangCollectible::RegenerateLang()
{
	LoadDataTable();

	//LangBounds.Empty();
	//LangTexts.Empty();

	//// Create LangBounds & Texts for each LangInfo
	//for (auto& LI : LangInfos)
	//{
	//	this->Name = LI->Name;
	//	// Create Bound
	//	FName BoundNameTemp = FName(*LI->Name);
	//	LangBounds.Add(CreateDefaultSubobject<ULangBound>(BoundNameTemp));
	//	LangBounds.Last()->SetupAttachment(RootComponent);
	//	// Create Text
	//	BoundNameTemp = FName(*(LI->Name + "Txt"));
	//	LangTexts.Add(CreateDefaultSubobject<UTextRenderComponent>(BoundNameTemp));
	//	LangTexts.Last()->SetupAttachment(RootComponent);
	//	// Link Text to Bound
	//	LangBounds.Last()->AddTextLB(LangTexts.Last());
	//	// Link Bound to Parent
	//	//find parent
	//	for (auto& LB : LangBounds)
	//	{

	//	}
	//}

}

void ALangCollectible::LoadDataTable()
{
	if (LanguageInfoTable)
	{
		static const FString ContextString(TEXT("Language Context"));
		LanguageInfoTable->GetAllRows(ContextString, LangInfos);
	}
}

void ALangCollectible::PostActorCreated()
{
	Super::PostActorCreated();

	//RegenerateLang();
}

void ALangCollectible::PostLoad()
{
	Super::PostLoad();

	//RegenerateLang();
}