//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

#include "CoreMinimal.h"
#include "ArticyChangedProperty.h"
#include "ArticyObjectNotificationManager.generated.h"

using FArticyPropertyChangedFunction = void(*)(FArticyChangedProperty);

enum class EArticyTypeProperties : uint8 {
	IncludeBaseType = 1,
	Template = 2,
	General = 4,
	ArticyObject = 8,
	All = 15
};

UCLASS(BlueprintType)
class ARTICYRUNTIME_API UArticyObjectNotificationManager : public UObject
{
	GENERATED_BODY()

public:
	static UArticyObjectNotificationManager* Get();

	void AddListener(const FString& Filter, FArticyPropertyChangedFunction ChangedFunction);
	void AddListener(const FString& Filter, EArticyTypeProperties Flags, FArticyPropertyChangedFunction ChangedFunction);
	void AddListener(UArticyBaseObject* Object, FArticyPropertyChangedFunction ChangedFunction);
	void RemoveListeners(const FString& Filter);
	void RemoveListeners(UArticyBaseObject* Object);

protected:
	void SplitInstance(const FString& InString, FString& OutName, FString& OutInstanceNumber);
};
