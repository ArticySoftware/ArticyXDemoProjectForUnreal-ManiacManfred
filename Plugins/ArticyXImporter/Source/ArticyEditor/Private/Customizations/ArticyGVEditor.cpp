//  
// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.  
//

#include "Customizations/ArticyGVEditor.h"
#include "ArticyEditorStyle.h"
#include "Editor.h"
#include "Widgets/Docking/SDockTab.h"

#define LOCTEXT_NAMESPACE "ArticyGVEditor"

/**
 * @brief Tab identifier for Articy Global Variables editor.
 */
const FName FArticyGVEditor::ArticyGVTab(TEXT("ArticyGVTab"));

/**
 * @brief Destructor for FArticyGVEditor.
 *
 * This destructor unregisters the editor from the undo system.
 */
FArticyGVEditor::~FArticyGVEditor()
{
	GEditor->UnregisterForUndo(this);
}

/**
 * @brief Initializes the Articy Global Variables editor.
 *
 * This function sets up the editor for editing Articy Global Variables, including registering undo capabilities and initializing the editor layout.
 *
 * @param Mode The toolkit mode (standalone or world-centric).
 * @param InitToolkitHost The toolkit host for integration.
 * @param ObjectToEdit The ArticyGlobalVariables object to be edited.
 */
void FArticyGVEditor::InitArticyGVEditor(const EToolkitMode::Type Mode, const TSharedPtr<IToolkitHost>& InitToolkitHost, UArticyGlobalVariables* ObjectToEdit)
{
	check(ObjectToEdit)

		GlobalVariables = ObjectToEdit;

	GlobalVariables->SetFlags(RF_Transactional);
	for (UArticyBaseVariableSet* VarSet : GlobalVariables->GetVariableSets())
	{
		VarSet->SetFlags(RF_Transactional);
		for (UArticyVariable* Var : VarSet->GetVariables())
		{
			Var->SetFlags(RF_Transactional);
		}
	}

	GEditor->RegisterForUndo(this);

	const bool bCreateDefaultStandaloneMenu = true;
	const bool bCreateDefaultToolbar = true;

	const TSharedRef<FTabManager::FLayout> StandaloneDefaultLayout = FTabManager::NewLayout("Standalone_ArticyGV_Layout")
		->AddArea
		(
			FTabManager::NewPrimaryArea()->SetOrientation(Orient_Vertical)
			->Split
			(
				FTabManager::NewStack()
				->AddTab(ArticyGVTab, ETabState::OpenedTab)->SetHideTabWell(false)
			)
		);

	GlobalVariablesWidget = SNew(SArticyGlobalVariables, GlobalVariables).bInitiallyCollapsed(false);

	// creates tabmanager and initializes many things
	FAssetEditorToolkit::InitAssetEditor(Mode, InitToolkitHost, TEXT("ArticyExtensionEditor"), StandaloneDefaultLayout, bCreateDefaultStandaloneMenu, bCreateDefaultToolbar, GlobalVariables.Get(), false);
}

/**
 * @brief Adds referenced objects to the garbage collector.
 *
 * This function is called to ensure that referenced objects are not garbage collected while the editor is active.
 *
 * @param Collector The reference collector.
 */
void FArticyGVEditor::AddReferencedObjects(FReferenceCollector& Collector)
{
}

/**
 * @brief Gets the color scale for world-centric tabs.
 *
 * @return The color scale used for world-centric tabs.
 */
FLinearColor FArticyGVEditor::GetWorldCentricTabColorScale() const
{
	return FLinearColor(0.3f, 0.2f, 0.5f, 0.5f);
}

/**
 * @brief Gets the toolkit's FName identifier.
 *
 * @return The FName identifier for the toolkit.
 */
FName FArticyGVEditor::GetToolkitFName() const
{
	return FName("ArticyGVEditor");
}

/**
 * @brief Gets the base toolkit name.
 *
 * @return The base name of the toolkit.
 */
FText FArticyGVEditor::GetBaseToolkitName() const
{
	return LOCTEXT("AppLabel", "ArticyGVEditor");
}

/**
 * @brief Gets the prefix for world-centric tab titles.
 *
 * @return The prefix for world-centric tab titles.
 */
FString FArticyGVEditor::GetWorldCentricTabPrefix() const
{
	return LOCTEXT("WorldCentricTabPrefix", "ArticyGV ").ToString();
}

/**
 * @brief Registers tab spawners for the editor.
 *
 * This function registers the tabs that can be spawned by the editor, such as the Details tab.
 *
 * @param InTabManager The tab manager for registering tab spawners.
 */
void FArticyGVEditor::RegisterTabSpawners(const TSharedRef<FTabManager>& InTabManager)
{
	WorkspaceMenuCategory = InTabManager->AddLocalWorkspaceMenuCategory(LOCTEXT("WorkspaceMenu_ArticyGVEditor", "Articy Global Variables Editor"));
	auto WorkspaceMenuCategoryRef = WorkspaceMenuCategory.ToSharedRef();

	// Registers toolbar
	FAssetEditorToolkit::RegisterTabSpawners(InTabManager);

	InTabManager->RegisterTabSpawner(ArticyGVTab, FOnSpawnTab::CreateSP(this, &FArticyGVEditor::SpawnTab_ArticyGV))
		.SetDisplayName(LOCTEXT("ArticyGVTab", "Details"))
		.SetIcon(FSlateIcon(FArticyEditorStyle::GetStyleSetName(), "ArticyImporter.ArticyImporter.16", "ArticyImporter.ArticyImporter.8"))
		.SetGroup(WorkspaceMenuCategoryRef);
}

/**
 * @brief Gets the referencer name for the editor.
 *
 * @return The referencer name.
 */
FString FArticyGVEditor::GetReferencerName() const
{
	return TEXT("FArticyGVEditor");
}

/**
 * @brief Spawns the Articy Global Variables tab.
 *
 * This function creates the content for the Articy Global Variables tab.
 *
 * @param Args The arguments for spawning the tab.
 * @return A shared reference to the created SDockTab.
 */
TSharedRef<SDockTab> FArticyGVEditor::SpawnTab_ArticyGV(const FSpawnTabArgs& Args) const
{
	check(Args.GetTabId() == ArticyGVTab);

	TSharedRef<SDockTab> DockTab = SNew(SDockTab).Label(LOCTEXT("ArticyGVEditorTitle", "Articy Global Variables"));
	if (GlobalVariablesWidget.IsValid())
	{
		DockTab->SetContent(GlobalVariablesWidget.ToSharedRef());
	}

	return DockTab;
}

#undef LOCTEXT_NAMESPACE
