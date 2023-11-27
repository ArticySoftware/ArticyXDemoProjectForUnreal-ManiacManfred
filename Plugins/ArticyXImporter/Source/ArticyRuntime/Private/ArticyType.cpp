//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "ArticyType.h"

FArticyEnumValueInfo FArticyType::GetEnumValue(int Value) const
{
	for (const auto& EnumInfo : EnumValues)
	{
		if (EnumInfo.Value == Value)
		{
			return EnumInfo;
		}
	}
	return {};
}

FArticyEnumValueInfo FArticyType::GetEnumValue(const FString& ValueName) const
{
	for (const auto& EnumInfo : EnumValues)
	{
		if (EnumInfo.LocaKey_DisplayName.Equals(ValueName))
		{
			return EnumInfo;
		}
	}
	return {};
}

FString FArticyType::GetFeatureDisplayName(const FString& FeatureName) const
{
	return LocalizeString(FeatureName);
}

FString FArticyType::GetFeatureDisplayNameLocaKey(const FString& FeatureName) const
{
	return FeatureName;
}

TArray<FArticyPropertyInfo> FArticyType::GetProperties() const
{
	return Properties;
}

TArray<FArticyPropertyInfo> FArticyType::GetPropertiesInFeature(const FString& FeatureName) const
{
	// TODO: Implement this functionality
	return {};
}

FArticyPropertyInfo FArticyType::GetProperty(const FString& PropertyName) const
{
	for (const auto& PropertyInfo : Properties)
	{
		if (PropertyInfo.LocaKey_DisplayName.Equals(PropertyName))
		{
			return PropertyInfo;
		}
	}
	return {};
}

FString FArticyType::LocalizeString(const FString& Input)
{
	const FText MissingEntry = FText::FromString("<MISSING STRING TABLE ENTRY>");

	// Look up entry in specified string table
	TOptional<FString> TableName = FTextInspector::GetNamespace(FText::FromString(Input));
	if (!TableName.IsSet())
	{
		TableName = TEXT("ARTICY");
	}
	const FText SourceString = FText::FromStringTable(
		FName(TableName.GetValue()),
		Input,
		EStringTableLoadingPolicy::FindOrFullyLoad);
	const FString Decoded = SourceString.ToString();
	if (!SourceString.IsEmpty() && !SourceString.EqualTo(MissingEntry))
	{
		return SourceString.ToString();
	}

	// By default, return input
	return Input;
}

void FArticyType::MergeChild(const FArticyType& Child)
{
	HasTemplate |= Child.HasTemplate;
	IsEnum |= Child.IsEnum;
	if (!Child.CPPType.IsEmpty())
	{
		CPPType = Child.CPPType;
	}
	if (!Child.DisplayName.IsEmpty())
	{
		DisplayName = Child.DisplayName;
	}
	if (!Child.LocaKey_DisplayName.IsEmpty())
	{
		LocaKey_DisplayName = Child.LocaKey_DisplayName;
	}
	if (!Child.TechnicalName.IsEmpty())
	{
		TechnicalName = Child.TechnicalName;
	}
	if (Child.EnumValues.Num() > 0)
	{
		EnumValues = Child.EnumValues;
	}
	if (Child.Features.Num() > 0)
	{
		Features = Child.Features;
	}
	if (Child.Properties.Num() > 0)
	{
		Properties = Child.Properties;
	}
}

void FArticyType::MergeParent(const FArticyType& Parent)
{
	HasTemplate |= Parent.HasTemplate;
	IsEnum |= Parent.IsEnum;
	if (CPPType.IsEmpty())
	{
		CPPType = Parent.CPPType;
	}
	if (DisplayName.IsEmpty())
	{
		DisplayName = Parent.DisplayName;
	}
	if (LocaKey_DisplayName.IsEmpty())
	{
		LocaKey_DisplayName = Parent.LocaKey_DisplayName;
	}
	if (TechnicalName.IsEmpty())
	{
		TechnicalName = Parent.TechnicalName;
	}
	if (EnumValues.Num() == 0)
	{
		EnumValues = Parent.EnumValues;
	}
	if (Features.Num() == 0)
	{
		Features = Parent.Features;
	}
	if (Properties.Num() == 0)
	{
		Properties = Parent.Properties;
	}
}
