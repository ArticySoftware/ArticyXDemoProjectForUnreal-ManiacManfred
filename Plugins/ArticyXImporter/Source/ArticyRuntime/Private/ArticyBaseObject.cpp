//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "ArticyBaseObject.h"
#include "ArticyPrimitive.h"
#include "ArticyTypeSystem.h"
#include "ArticyHelpers.h"

UArticyPrimitive* UArticyBaseObject::GetSubobject(FArticyId Id) const
{
	auto obj = Subobjects.Find(Id);
	return obj ? *obj : nullptr;
}

void UArticyBaseObject::AddSubobject(UArticyPrimitive* Obj)
{
	Subobjects.Add(Obj->GetId(), Obj);
}

FArticyType UArticyBaseObject::GetArticyType() const
{
	return ArticyType;
}

FText UArticyBaseObject::GetPropertyText(const FText Property)
{
	return ArticyHelpers::LocalizeString(this, Property, true, &Property);
}
