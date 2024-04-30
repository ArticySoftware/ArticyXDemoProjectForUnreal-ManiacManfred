//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "ArticyRuntimeModule.h"
#include "Internationalization/StringTableRegistry.h"
#include "Misc/Paths.h"
#include "HAL/PlatformFilemanager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "Internationalization/Internationalization.h"
#include "Internationalization/Culture.h"

#if WITH_EDITOR
#include "Editor.h"
#endif

DEFINE_LOG_CATEGORY(LogArticyRuntime)

void FArticyRuntimeModule::StartupModule()
{
}

void FArticyRuntimeModule::ShutdownModule()
{
}

IMPLEMENT_MODULE(FArticyRuntimeModule, ArticyRuntime)
