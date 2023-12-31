//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "Interfaces/ArticyNode.h"

void UArticyNode::Explore(UArticyFlowPlayer* Player, TArray<FArticyBranch>& OutBranches, const uint32& Depth)
{
	//default implementation: continue on output pins
	IArticyOutputPinsProvider::Explore(Player, OutBranches, Depth + 1);
}
