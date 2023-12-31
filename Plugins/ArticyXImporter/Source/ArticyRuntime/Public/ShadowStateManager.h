//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#pragma once

#include "UObject/Interface.h"
#include "ShadowStateManager.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UShadowStateManager : public UInterface
{
	GENERATED_BODY()
};

/**
 * This interface can be used for all classes that need to keep track of shadow
 * states. It provides PushState and PopState methods, which push/pop on an array
 * of FOnPopState delegates, where interested objects can register themselves to
 * know when a state they were cloned in is destroyed.
 */
class IShadowStateManager
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	
	DECLARE_MULTICAST_DELEGATE(FOnPopState)

	template<typename LambdaType>
	FDelegateHandle RegisterOnPopState(LambdaType Lambda);
	void UnregisterOnPopState(FDelegateHandle Delegate);

	uint32 GetShadowLevel() const { return ShadowLevel; }

private:

	/** The current shadow level of this GV instance. */
	uint32 ShadowLevel = 0;

	/** A stack of OnPopState delegates. The last one is the one for the current shadow level. */
	TArray<FOnPopState> OnPopStateDelegates;

	friend class UArticyFlowPlayer;

	void PushState(uint32 NewShadowLevel);
	void PopState(uint32 CurrShadowLevel);
};

template <typename LambdaType>
FDelegateHandle IShadowStateManager::RegisterOnPopState(LambdaType Lambda)
{
	return OnPopStateDelegates.Last().AddLambda(Lambda);
}
