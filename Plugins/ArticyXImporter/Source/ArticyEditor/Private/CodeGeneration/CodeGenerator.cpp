//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "CodeGenerator.h"
#include "ArticyImportData.h"
#include "DatabaseGenerator.h"
#include "GlobalVarsGenerator.h"
#include "InterfacesGenerator.h"
#include "Developer/HotReload/Public/IHotReload.h"
#include "ArticyGlobalVariables.h"
#include "ObjectDefinitionsGenerator.h"
#include "PackagesGenerator.h"
#include "ArticyDatabase.h"
#include "ExpressoScriptsGenerator.h"
#include "FileHelpers.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "ArticyEditorModule.h"
#include "ArticyPluginSettings.h"
#include "ArticyTypeGenerator.h"
#include "ArticyLocalizerGenerator.h"
#include "AssetToolsModule.h"
#include "UObject/ConstructorHelpers.h"
#include "Misc/FileHelper.h"
#include "ObjectTools.h"
#include "HAL/PlatformFilemanager.h"
#include "Misc/App.h"
#include "Misc/MessageDialog.h"
#include "Dialogs/Dialogs.h"
#include "ISourceControlModule.h"
#if WITH_LIVE_CODING && ENGINE_MAJOR_VERSION == 4
#include "Windows/LiveCoding/Public/ILiveCodingModule.h"
#endif

//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------// 
#define LOCTEXT_NAMESPACE "CodeGenerator"

TMap<FString, FString> CodeGenerator::CachedFiles;

FString CodeGenerator::GetSourceFolder()
{
	return FPaths::GameSourceDir() / FApp::GetProjectName() / TEXT("ArticyGenerated");
}

FString CodeGenerator::GetGeneratedInterfacesFilename(const UArticyImportData* Data)
{
	return Data->GetProject().TechnicalName + "Interfaces";
}

FString CodeGenerator::GetGeneratedTypesFilename(const UArticyImportData* Data)
{
	return Data->GetProject().TechnicalName + "ArticyTypes";
}

FString CodeGenerator::GetGlobalVarsClassname(const UArticyImportData* Data, const bool bOmittPrefix)
{
	return (bOmittPrefix ? "" : "U") + Data->GetProject().TechnicalName + "GlobalVariables";
}

FString CodeGenerator::GetGVNamespaceClassname(const UArticyImportData* Data, const FString& Namespace)
{
	return "U" + Data->GetProject().TechnicalName + Namespace + "Variables";
}

FString CodeGenerator::GetDatabaseClassname(const UArticyImportData* Data, const bool bOmittPrefix)
{
	return (bOmittPrefix ? "" : "U") + Data->GetProject().TechnicalName + "Database";
}

FString CodeGenerator::GetMethodsProviderClassname(const UArticyImportData* Data, const bool bOmittPrefix)
{
	return (bOmittPrefix ? "" : "U") + Data->GetProject().TechnicalName + "MethodsProvider";
}

FString CodeGenerator::GetExpressoScriptsClassname(const UArticyImportData* Data, const bool bOmittPrefix)
{
	return (bOmittPrefix ? "" : "U") + Data->GetProject().TechnicalName + "ExpressoScripts";
}

FString CodeGenerator::GetFeatureInterfaceClassName(const UArticyImportData* Data, const FArticyTemplateFeatureDef& Feature, const bool bOmittPrefix)
{
	return (bOmittPrefix ? "" : "I") + Data->GetProject().TechnicalName + "ObjectWith" + Feature.GetTechnicalName() + "Feature";
}

FString CodeGenerator::GetArticyTypeClassname(const UArticyImportData* Data, const bool bOmittPrefix)
{
	return (bOmittPrefix ? "" : "U") + Data->GetProject().TechnicalName + "TypeSystem";
}

FString CodeGenerator::GetArticyLocalizerClassname(const UArticyImportData* Data, const bool bOmittPrefix)
{
	return (bOmittPrefix ? "" : "U") + Data->GetProject().TechnicalName + "LocalizerSystem";
}

bool CodeGenerator::DeleteGeneratedCode(const FString& Filename)
{
	if (Filename.IsEmpty())
		return FPlatformFileManager::Get().GetPlatformFile().DeleteDirectoryRecursively(*GetSourceFolder());

	return FPlatformFileManager::Get().GetPlatformFile().DeleteFile(*(GetSourceFolder() / Filename));
}

bool CodeGenerator::DeleteExtraCode(const TArray<FString>& GeneratedFiles)
{
	const auto& SourceFolder = GetSourceFolder();
	IFileManager& FileManager = IFileManager::Get();

	// Create a directory visitor class
	class FDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
	{
	public:
		TArray<FString> const& GeneratedFiles;
		IFileManager& FileManager;

		FDirectoryVisitor(TArray<FString> const& InGeneratedFiles, IFileManager& InFileManager)
			: GeneratedFiles(InGeneratedFiles), FileManager(InFileManager) {}

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			if (bIsDirectory)
			{
				// Continue searching
				return true; 
			}

			FString FilePath(FilenameOrDirectory);
			FString FileName = FPaths::GetCleanFilename(FilePath);

			// Only applies to .h files - though there should not be anything else
			if (FileName.EndsWith(TEXT(".h")))
			{
				// Check if the filename starts with any of the strings from the generated files
				bool bShouldKeep = false;
				for (const FString& Prefix : GeneratedFiles)
				{
					if (FileName.StartsWith(Prefix))
					{
						bShouldKeep = true;
						break;
					}
				}

				// Delete file if it does not match any prefix
				if (!bShouldKeep)
				{
					return FileManager.Delete(*FilePath);
				}
			}

			// Continue searching
			return true;
		}
	};

	// Use the directory visitor to iterate through the files in the source folder
	FDirectoryVisitor Visitor(GeneratedFiles, FileManager);
	return FileManager.IterateDirectory(*SourceFolder, Visitor);
}

void CodeGenerator::CacheCodeFiles()
{
	TArray<FString> FileNames;
	IFileManager::Get().FindFiles(FileNames, *GetSourceFolder());
	TArray<FString> FileContents;
	FileContents.SetNumZeroed(FileNames.Num());

	for (int32 i = 0; i < FileNames.Num(); i++)
	{
		FileNames[i] = GetSourceFolder() / FileNames[i];
	}
	
	for(int32 i=0; i < FileNames.Num(); i++)
	{
		FFileHelper::LoadFileToString(FileContents[i], *FileNames[i]);
		CachedFiles.Add(FileNames[i], FileContents[i]);
	}
}

bool CodeGenerator::GenerateCode(UArticyImportData* Data)
{
	if (!Data)
		return false;

	bool bCodeGenerated = false;

	CacheCodeFiles();

	//generate all files if ObjectDefs or GVs changed
	if (Data->GetSettings().DidObjectDefsOrGVsChange())
	{
		//DeleteGeneratedCode();

		TArray<FString> OutFiles;
		FString OutFile;

		GlobalVarsGenerator::GenerateCode(Data, OutFile);
		OutFiles.Add(OutFile);
		DatabaseGenerator::GenerateCode(Data, OutFile);
		OutFiles.Add(OutFile);
		InterfacesGenerator::GenerateCode(Data, OutFile);
		OutFiles.Add(OutFile);
		ObjectDefinitionsGenerator::GenerateCode(Data, OutFile);
		OutFiles.Add(OutFile);
		/* generate scripts as well due to them including the generated global variables
		 * if we remove a GV set but don't regenerate expresso scripts, the resulting class won't compile */
		ExpressoScriptsGenerator::GenerateCode(Data, OutFile);
		OutFiles.Add(OutFile);
		ArticyTypeGenerator::GenerateCode(Data, OutFile);
		OutFiles.Add(OutFile);
		ArticyLocalizerGenerator::GenerateCode(Data, OutFile);
		OutFiles.Add(OutFile);

		bCodeGenerated = true;
		DeleteExtraCode(OutFiles);
	}
	// if object defs of GVs didn't change, but scripts changed, regenerate only expresso scripts
	else if (Data->GetSettings().DidScriptFragmentsChange())
	{
		FString OutFile;
		ExpressoScriptsGenerator::GenerateCode(Data, OutFile);
		bCodeGenerated = true;
	}

	return bCodeGenerated;
}

void CodeGenerator::Recompile(UArticyImportData* Data)
{
	Compile(Data);
}

bool CodeGenerator::DeleteGeneratedAssets(const FArticyPackageDefs& PackageDefs)
{
	FAssetRegistryModule& AssetRegistry = FModuleManager::Get().GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> OutAssets;
	AssetRegistry.Get().GetAssetsByPath(FName(*ArticyHelpers::GetArticyGeneratedFolder()), OutAssets, true, false);
	
	TArray<UObject*> ExistingAssets;
	TArray<FAssetData> InvalidAssets;
	for(FAssetData Data : OutAssets)
	{
		if (Data.IsValid())
		{
			UObject* Asset = Data.GetAsset();
			// if the class is missing (generated code deleted for example), the asset data will be valid but return a nullptr
			if(!Asset)
			{
				InvalidAssets.Add(Data);
				continue;
			}

			bool ExcludeAsset = false;
			if (const UArticyPackage* PackageAsset = Cast<UArticyPackage>(Asset))
			{
				for (const FArticyPackageDef& PackageDef : PackageDefs.GetPackages())
				{
					// Don't delete package assets that are not included in the import
					if (!PackageDef.GetIsIncluded() && PackageAsset->Name.Equals(PackageDef.GetName()))
					{
						ExcludeAsset = true;
					}
				}
			}

			if (!ExcludeAsset)
			{
				ExistingAssets.Add(Asset);
			}
		}
	}

	bool bInvalidDeletionSuccess = true;
	if(InvalidAssets.Num() > 0)
	{
		bInvalidDeletionSuccess = InvalidAssets.Num() == ObjectTools::DeleteAssets(InvalidAssets, false);
	}
	
	if(ExistingAssets.Num() > 0)
	{
		return ObjectTools::ForceDeleteObjects(ExistingAssets, false) > 0 && bInvalidDeletionSuccess;
	}

	// returns true if there is nothing to delete to not trigger the ensure
	return true;
}

bool CodeGenerator::RenameGeneratedAssets(const FArticyPackageDefs& PackageDefs)
{
	const FAssetRegistryModule& AssetRegistry = FModuleManager::Get().GetModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> OutAssets;
	AssetRegistry.Get().GetAssetsByPath(FName(*ArticyHelpers::GetArticyGeneratedFolder()), OutAssets, true, false);
	
	for(FAssetData Data : OutAssets)
	{
		if (Data.IsValid())
		{
			UObject* Asset = Data.GetAsset();
			
			// Skip invalid assets
			if(!Asset)
			{
				continue;
			}

			if (UArticyPackage* PackageAsset = Cast<UArticyPackage>(Asset))
			{
				for (const FArticyPackageDef& PackageDef : PackageDefs.GetPackages())
				{
					// Skip included packages - we delete them anyway
					if (PackageDef.GetIsIncluded())
					{
						continue;
					}
					
					// Only check packages with name changes
					if (PackageDef.GetName().Equals(PackageDef.GetPreviousName()))
					{
						continue;
					}

					// Rename the asset
					FAssetToolsModule& AssetToolsModule = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools");
					TArray<FAssetRenameData> AssetsAndNames;
					const FString OldName = PackageAsset->GetName();
					const FString PackagePath = FPackageName::GetLongPackagePath(PackageAsset->GetOutermost()->GetName());
					new(AssetsAndNames)FAssetRenameData(PackageAsset, PackagePath, PackageDef.GetName());
					AssetToolsModule.Get().RenameAssets(AssetsAndNames);

					FAssetRegistryModule::AssetRenamed(PackageAsset, PackagePath / OldName);
					PackageAsset->MarkPackageDirty();
					PackageAsset->GetOuter()->MarkPackageDirty();
				}
			}
		}
	}

	// returns true if there are no failures
	return true;
}

void CodeGenerator::Compile(UArticyImportData* Data)
{
#if WITH_LIVE_CODING && ENGINE_MAJOR_VERSION == 4
	ILiveCodingModule& LiveCodingModule = FModuleManager::LoadModuleChecked<ILiveCodingModule>("LiveCoding");
	if (LiveCodingModule.IsEnabledForSession())
	{
		// Cancel
		FText ErrorTitle = FText(LOCTEXT("LiveCodingErrorTitle", "Disable Experimental Live Coding"));
		FText ErrorText = FText(LOCTEXT("LiveCodingErrorMessage", "Unable to reimport Articy:Draft project changes because Experimental Live Coding is enabled. Please disable Live Coding and run a Full Reimport to continue."));
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 24
		EAppReturnType::Type ReturnType = OpenMsgDlgInt(EAppMsgType::Ok, ErrorText, ErrorTitle);
#elif ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
		EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, ErrorText, ErrorTitle);
#else
		EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, ErrorText, &ErrorTitle);
#endif
		return;
	}
#endif

	bool bWaitingForOtherCompile = false;

	// We can only hot-reload via DoHotReloadFromEditor when we already had code in our project
	IHotReloadInterface& HotReloadSupport = FModuleManager::LoadModuleChecked<IHotReloadInterface>("HotReload");
	if (HotReloadSupport.IsCurrentlyCompiling())
	{
		bWaitingForOtherCompile = true;
		UE_LOG(LogArticyEditor, Warning, TEXT("Already compiling, waiting until it's done."));
	}

	static FDelegateHandle AfterCompileLambda;
	if(AfterCompileLambda.IsValid())
	{
#if ENGINE_MAJOR_VERSION >= 5
		FCoreUObjectDelegates::ReloadCompleteDelegate.Remove(AfterCompileLambda);
#else
		IHotReloadModule::Get().OnHotReload().Remove(AfterCompileLambda);
#endif
		AfterCompileLambda.Reset();
	}

#if ENGINE_MAJOR_VERSION >= 5
	AfterCompileLambda = FCoreUObjectDelegates::ReloadCompleteDelegate.AddLambda([=](EReloadCompleteReason ReloadCompleteReason)
	{
		OnCompiled(Data);
	});
#else
	AfterCompileLambda = IHotReloadModule::Get().OnHotReload().AddLambda([=](bool bWasTriggeredAutomatically)
	{
		OnCompiled(Data);
	});
#endif


	// register a lambda to handle failure in code generation (compilation failed due to generated articy code)
	// detection of faulty articy code is a heuristic and not optimal!
	static FDelegateHandle RestoreCachedVersionHandle;
	if(RestoreCachedVersionHandle.IsValid())
	{
		IHotReloadModule::Get().OnModuleCompilerFinished().Remove(RestoreCachedVersionHandle);
		RestoreCachedVersionHandle.Reset();
	}
	RestoreCachedVersionHandle = IHotReloadModule::Get().OnModuleCompilerFinished().AddLambda([=](const FString& Log, ECompilationResult::Type Type, bool bSuccess)
	{
		if (Type == ECompilationResult::Succeeded || Type == ECompilationResult::UpToDate)
		{
			// do nothing in case compilation succeeded without problems
		}
		else if(Type == ECompilationResult::OtherCompilationError)
		{
			/** if compile error is due to articy, restore the previous data */
			const bool bErrorInGeneratedCode = ParseForError(Log);
			
			if (bErrorInGeneratedCode)
			{
				const bool bCanContinue = RestorePreviousImport(Data, true, Type);
				if(bCanContinue)
				{
					OnCompiled(Data);
				}
			}
		}
		// in case the compilation was neither successful nor had compile errors, revert just to be safe
		else
		{
			const bool bCanContinue = RestorePreviousImport(Data, true, Type);
			if (bCanContinue)
			{
				OnCompiled(Data);
			}
		}
	});
	
	if (!bWaitingForOtherCompile)
	{
		HotReloadSupport.DoHotReloadFromEditor(EHotReloadFlags::None /*async*/);
	}
}

void CodeGenerator::GenerateAssets(UArticyImportData* Data)
{
	TGuardValue<bool> GuardIsInitialLoad(GIsInitialLoad, false);

	ensure(Data);
	
	//compiling is done!
	//check if UArticyBaseGlobalVariables can be found, otherwise something went wrong!
	const auto ClassName = GetGlobalVarsClassname(Data, true);
	auto FullClassName = FString::Printf(TEXT("Class'/Script/%s.%s'"), FApp::GetProjectName(), *ClassName);
	if (!ConstructorHelpersInternal::FindOrLoadClass(FullClassName, UArticyGlobalVariables::StaticClass()))
	{
		if (!ensure(ConstructorHelpersInternal::FindOrLoadClass(FullClassName, UArticyGlobalVariables::StaticClass())))
		UE_LOG(LogArticyEditor, Error, TEXT("Could not find generated global variables class after compile!"));
	}

	if(!ensureAlwaysMsgf(RenameGeneratedAssets(Data->GetPackageDefs()),
		TEXT("RenameGeneratedAssets() has failed. The Articy X Importer can not proceed without\n"
		"being able to rename previously generated assets for packages with new names.\n"
		"Please make sure the Generated folder in ArticyContent is editable.")))
	{
		// Failed to rename generated assets. We can't continue
		return;
	}
	
	if(!ensureAlwaysMsgf(DeleteGeneratedAssets(Data->GetPackageDefs()), 
		TEXT("DeletedGeneratedAssets() has failed. The Articy X Importer can not proceed without\n"
		"being able to delete the previously generated assets to replace them with new ones.\n"
		"Please make sure the Generated folder in ArticyContent is editable.")))
	{
		// Failed to delete generated assets. We can't continue
		return;
	}

	//generate the global variables asset
	GlobalVarsGenerator::GenerateAsset(Data);
	//generate the database asset
	UArticyDatabase* ArticyDatabase = DatabaseGenerator::GenerateAsset(Data);
	if (!ensureAlwaysMsgf(ArticyDatabase != nullptr, TEXT("Could not create ArticyDatabase asset!")))
	{
		// Somehow, we failed to load the database. We just need to stop right here and right now.
		// In the future, it'd be nice to have a popup or notification here. For now, we'll
		//  have to settle with the ensures.
		return;
	}
	ArticyTypeGenerator::GenerateAsset(Data);

	//generate assets for all the imported objects
	PackagesGenerator::GenerateAssets(Data);
	ArticyDatabase->SetLoadedPackages(Data->GetPackagesDirect());

	//gather all articy assets to save them
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	TArray<FAssetData> GeneratedAssets;
	AssetRegistryModule.Get().GetAssetsByPath(FName(*ArticyHelpers::GetArticyGeneratedFolder()), GeneratedAssets, true);

	TArray<UPackage*> PackagesToSave;
	
	PackagesToSave.Add(Data->GetOutermost());
	for (FAssetData AssetData : GeneratedAssets)
	{
		PackagesToSave.Add(AssetData.GetAsset()->GetOutermost());
	}

	// Check out all the assets we want to save (if source control is enabled)
	ISourceControlProvider& SourceControlProvider = ISourceControlModule::Get().GetProvider();
	if (SourceControlProvider.IsEnabled())
	{
		TArray<UPackage*> CheckedOutPackages;
		FEditorFileUtils::CheckoutPackages(PackagesToSave, &CheckedOutPackages, false);
	}

	// Save the packages to disk
	for (auto Package : PackagesToSave) { Package->SetDirtyFlag(true); }
	if (!UEditorLoadingAndSavingUtils::SavePackages(PackagesToSave, true))
	{
		UE_LOG(LogArticyEditor, Error, TEXT("Failed to save packages. Make sure to save before submitting in Perforce."));
	}

	FArticyEditorModule::Get().OnAssetsGenerated.Broadcast();

	// update the internal save state of the package settings (add settings for new packages, remove outdated package settings, restore previous settings for still existing packages)
	UArticyPluginSettings* ArticyPluginSettings = GetMutableDefault<UArticyPluginSettings>();
	ArticyPluginSettings->UpdatePackageSettings();
}

void CodeGenerator::OnCompiled(UArticyImportData* Data)
{
	Data->GetSettings().SetObjectDefinitionsRebuilt();
	Data->GetSettings().SetScriptFragmentsRebuilt();
	// broadcast that compilation has finished. ArticyImportData will then generate the assets and perform post import operations
	FArticyEditorModule::Get().OnCompilationFinished.Broadcast(Data);
}

bool CodeGenerator::ParseForError(const FString& Log)
{
	TArray<FString> Lines;
	// parsing into individual FStrings for each line. Using \n as delimiter, should cover both Mac OSX and Windows
	Log.ParseIntoArray(Lines, TEXT("\n"));

	// heuristic: error due to articy?
	bool bErrorInGeneratedCode = false;
	for (const FString& Line : Lines)
	{
		if (Line.Contains(TEXT("error")) && Line.Contains(TEXT("ArticyGenerated")))
		{
			bErrorInGeneratedCode = true;
			break;
		}
	}

	return bErrorInGeneratedCode;
}

bool CodeGenerator::RestorePreviousImport(UArticyImportData* Data, const bool& bNotifyUser, ECompilationResult::Type Reason)
{
	ensure(Data);

	const FText ArticyImportErrorText = FText::FromString(TEXT("Articy import error"));
	FText ReasonForRestoreText = FText::FromString(ECompilationResult::ToString(Reason));

	if(!Data->HasCachedVersion())
	{
		const FText CacheNotAvailableText = FText::Format(LOCTEXT("NoCacheAvailable", "Aborting import process. No cache available to restore. Deleting import asset but leaving generated code intact. Please delete manually in Source/ArticyGenerated if necessary and rebuild. Reason: {0}."), ReasonForRestoreText);
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 24
		EAppReturnType::Type ReturnType = OpenMsgDlgInt(EAppMsgType::Ok, CacheNotAvailableText, ArticyImportErrorText);
#elif ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
		EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, CacheNotAvailableText, ArticyImportErrorText);
#else
		EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, CacheNotAvailableText, &ArticyImportErrorText);
#endif
		ObjectTools::DeleteAssets({ Data }, false);
		
		return false;
	}
	
	// transfer the cached data into the current one
	Data->ResolveCachedVersion();

	// attempt to restore all generated files
	const bool bFilesRestored = RestoreCachedFiles();


	// Reason is "-1" for cancelled for some reason
	if(Reason == -1)
	{
		ReasonForRestoreText = FText::FromString(TEXT("Compilation cancelled"));
	}
	else if(Reason == ECompilationResult::OtherCompilationError)
	{
		ReasonForRestoreText = FText::FromString(TEXT("Error in compiled articy code"));
	}

	// if we succeeded, tell the user and call OnCompiled - which will then create the assets
	if (bFilesRestored)
	{
		if (bNotifyUser)
		{
			const FText CacheRestoredText = FText::Format(LOCTEXT("ImportDataCacheRestoredText", "Restored previously generated articy code. Reason: {0}. Continuing import with last valid state."), ReasonForRestoreText);
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 24
			EAppReturnType::Type ReturnType = OpenMsgDlgInt(EAppMsgType::Ok, CacheRestoredText, ArticyImportErrorText);
#elif ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
			EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, CacheRestoredText, ArticyImportErrorText);
#else
			EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, CacheRestoredText, &ArticyImportErrorText);
#endif
			return true;
		}
	}
	else
	{
		// if there were no previous files or not all files could be restored, delete them instead
		if (DeleteGeneratedCode())
		{
			if (bNotifyUser)
			{
				const FText CacheDeletedText = FText::Format(LOCTEXT("ImportDataCacheDeletedText", "Deleted generated articy code. Reason: {0}. Aborting import process."), ReasonForRestoreText);
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 24
				EAppReturnType::Type ReturnType = OpenMsgDlgInt(EAppMsgType::Ok, CacheDeletedText, ArticyImportErrorText);
#elif ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
				EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, CacheDeletedText, ArticyImportErrorText);
#else
				EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, CacheDeletedText, &ArticyImportErrorText);
#endif
			}
		}
		// if deletion didn't work for some reason, notify the user
		else
		{
			if (bNotifyUser)
			{
				const FText CacheDeletionFailedText = FText::Format(LOCTEXT("ImportDataCacheDeletionFailedText", "Tried to delete generated articy code. Reason: {0}. Failed to delete. Aborting import process."), ReasonForRestoreText);
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION <= 24
				EAppReturnType::Type ReturnType = OpenMsgDlgInt(EAppMsgType::Ok, CacheDeletionFailedText, ArticyImportErrorText);
#elif ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3
				EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, CacheDeletionFailedText, ArticyImportErrorText);
#else
				EAppReturnType::Type ReturnType = FMessageDialog::Open(EAppMsgType::Ok, CacheDeletionFailedText, &ArticyImportErrorText);
#endif
			}
		}
	}

	return false;
}

bool CodeGenerator::RestoreCachedFiles()
{
	bool bFilesRestored = CachedFiles.Num() > 0 ? true : false;

	for(auto& CachedFile : CachedFiles)
	{
		bFilesRestored = bFilesRestored && FFileHelper::SaveStringToFile(CachedFile.Value, *CachedFile.Key);
	}

	return bFilesRestored;
}

#undef LOCTEXT_NAMESPACE
