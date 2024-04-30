#include "ArticyImportCommandlet.h"
#include "ArticyEditorFunctionLibrary.h"

int32 UArticyImportCommandlet::Main(const FString & Params)
{
    // Parse the command line
    TArray<FString> Tokens, Switches;
    ParseCommandLine(*Params, Tokens, Switches);

    // Determine which process to follow
    bool CompleteReimport = false;
    bool RegenerateAssets = false;
    for (int SwitchNum = 0; SwitchNum < Switches.Num(); SwitchNum++)
    {
        if (Switches[SwitchNum].Compare(TEXT("ArticyReimport"), ESearchCase::IgnoreCase) == 0)
        {
            CompleteReimport = true;
        }
        if (Switches[SwitchNum].Compare(TEXT("ArticyRegenerate"), ESearchCase::IgnoreCase) == 0)
        {
            RegenerateAssets = true;
        }
    }

    // Follow the appropriate process
    if (CompleteReimport) 
    {
        return FArticyEditorFunctionLibrary::ForceCompleteReimport();
    }
    if (RegenerateAssets)
    {
        return FArticyEditorFunctionLibrary::RegenerateAssets();
    }
    return FArticyEditorFunctionLibrary::ReimportChanges();
}