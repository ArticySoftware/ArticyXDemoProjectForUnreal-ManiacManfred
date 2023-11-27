//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "ArticyObjectNotificationManager.h"
#include "ArticyBaseObject.h"
#include "ArticyDatabase.h"

UArticyObjectNotificationManager* UArticyObjectNotificationManager::Get()
{
	static TWeakObjectPtr<UArticyObjectNotificationManager> ArticyObjectNotificationManager;

	if (!ArticyObjectNotificationManager.IsValid())
	{
		ArticyObjectNotificationManager = TWeakObjectPtr<UArticyObjectNotificationManager>(NewObject<UArticyObjectNotificationManager>());
	}

	return ArticyObjectNotificationManager.Get();
}

void UArticyObjectNotificationManager::SplitInstance(const FString& InString, FString& OutName, FString& OutInstanceNumber)
{
	const FString SearchSubstr = TEXT("<");
	const int32 StartIdx = InString.Find(SearchSubstr);

	if (StartIdx != INDEX_NONE)
	{
		const int32 EndIdx = InString.Find(TEXT(">"), ESearchCase::CaseSensitive, ESearchDir::FromEnd, StartIdx);

		if (EndIdx != INDEX_NONE)
		{
			OutName = InString.Left(StartIdx);
			OutInstanceNumber = InString.Mid(StartIdx + 1, EndIdx - StartIdx - 1);
		}
		else
		{
			OutName = InString;
			OutInstanceNumber = TEXT("0");
		}
	}
	else
	{
		OutName = InString;
		OutInstanceNumber = TEXT("0");
	}
}

void UArticyObjectNotificationManager::AddListener(const FString& Filter, FArticyPropertyChangedFunction ChangedFunction)
{
	FString ObjectName, ObjectInstance;
	SplitInstance(Filter, ObjectName, ObjectInstance);
	UArticyObject* Object;
	auto DB = UArticyDatabase::Get(this);
	if (Filter.StartsWith(TEXT("0x")))
	{
		Object = DB->GetObject<UArticyObject>(FArticyId{ ArticyHelpers::HexToUint64(ObjectName) }, FCString::Atod(*ObjectInstance));
	}
	else if (Filter.IsNumeric())
	{
		Object = DB->GetObject<UArticyObject>(FArticyId{ FCString::Strtoui64(*ObjectName, nullptr, 10) }, FCString::Atod(*ObjectInstance));
	}
	else
	{
		Object = DB->GetObjectByName(*ObjectName, FCString::Atod(*ObjectInstance));
	}

	if (Object)
	{
		AddListener(Object, ChangedFunction);
	}
}

void UArticyObjectNotificationManager::AddListener(const FString& Filter, EArticyTypeProperties Flags, FArticyPropertyChangedFunction ChangedFunction)
{
}

void UArticyObjectNotificationManager::AddListener(UArticyBaseObject* Object, FArticyPropertyChangedFunction ChangedFunction)
{
	Object->ReportChanged.AddLambda([ChangedFunction](FArticyChangedProperty ChangedProperty)
	{
		ChangedFunction(ChangedProperty);
	});
}

void UArticyObjectNotificationManager::RemoveListeners(const FString& Filter)
{
	FString ObjectName, ObjectInstance;
	SplitInstance(Filter, ObjectName, ObjectInstance);
	UArticyObject* Object;
	auto DB = UArticyDatabase::Get(this);
	if (Filter.StartsWith(TEXT("0x")))
	{
		Object = DB->GetObject<UArticyObject>(FArticyId{ ArticyHelpers::HexToUint64(ObjectName) }, FCString::Atod(*ObjectInstance));
	}
	else if (Filter.IsNumeric())
	{
		Object = DB->GetObject<UArticyObject>(FArticyId{ FCString::Strtoui64(*ObjectName, nullptr, 10) }, FCString::Atod(*ObjectInstance));
	}
	else
	{
		Object = DB->GetObjectByName(*ObjectName, FCString::Atod(*ObjectInstance));
	}

	if (Object)
	{
		RemoveListeners(Object);
	}
}

void UArticyObjectNotificationManager::RemoveListeners(UArticyBaseObject* Object)
{
	Object->ReportChanged.Clear();
}
