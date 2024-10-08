// Copyright (c) 2023 articy Software GmbH & Co. KG. All rights reserved.

#include "Slate/AssetPicker/SArticyObjectTileView.h"
#include <GenericPlatform/ICursor.h>
#include <Widgets/Layout/SBorder.h>
#include <Widgets/Text/STextBlock.h>
#include <Styling/SlateTypes.h>
#include <Widgets/Layout/SScaleBox.h>
#include <EditorStyleSet.h>
#include "Slate/AssetPicker/SArticyObjectToolTip.h"
#include "ArticyEditorStyle.h"
#include "Editor.h"
#include "ArticyEditorModule.h"
#include "Slate/UserInterfaceHelperFunctions.h"
#include "HAL/PlatformApplicationMisc.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Layout/WidgetPath.h"
#include "Framework/Application/SlateApplication.h"

#define LOCTEXT_NAMESPACE "ArticyObjectTileView"

/**
 * @brief Updates the widget with a new Articy ID.
 *
 * This method updates the cached Articy ID and object and refreshes the widget's display.
 *
 * @param NewArticyId The new Articy ID to display.
 */
void SArticyObjectTileView::Update(const FArticyId& NewArticyId)
{
	CachedArticyId = NewArticyId;
	CachedArticyObject = UArticyObject::FindAsset(CachedArticyId);

	UpdateWidget();
}

/**
 * @brief Updates the widget's display elements.
 *
 * This method refreshes the preview image and type image based on the current Articy object.
 */
void SArticyObjectTileView::UpdateWidget()
{
	bHasPreviewImage = UserInterfaceHelperFunctions::RetrievePreviewImage(CachedArticyObject.Get(), PreviewBrush);
	// if we failed getting a preview image, use the default type image instead
	if (!bHasPreviewImage)
	{
		PreviewBrush = *UserInterfaceHelperFunctions::GetArticyTypeImage(CachedArticyObject.Get(), UserInterfaceHelperFunctions::Large);
	}

	TypeImage = UserInterfaceHelperFunctions::GetArticyTypeImage(CachedArticyObject.Get(), UserInterfaceHelperFunctions::Medium);
}

/**
 * @brief Constructs the widget.
 *
 * This method initializes the widget with the given arguments and sets up the UI elements.
 *
 * @param InArgs The arguments for widget construction.
 */
void SArticyObjectTileView::Construct(const FArguments& InArgs)
{
	ArticyIdToDisplay = InArgs._ArticyIdToDisplay;
	OnArticyIdChanged = InArgs._OnArticyIdChanged;
	ThumbnailSize = InArgs._ThumbnailSize;
	ThumbnailPadding = InArgs._ThumbnailPadding;
	LabelVisibility = InArgs._LabelVisibility;
	bIsReadOnly = InArgs._bIsReadOnly;
	CopyAction = InArgs._CopyAction;
	PasteAction = InArgs._PasteAction;

	TAttribute<FOptionalSize> WidthScaleAttribute = ThumbnailSize.X / 3.f;
	TAttribute<FOptionalSize> HeightScaleAttribute = ThumbnailSize.Y / 3.f;

	SetCursor(EMouseCursor::Hand);

	PreviewBrush.ImageSize = ThumbnailSize;

	PreviewImage = SNew(SImage)
		.Image(this, &SArticyObjectTileView::OnGetEntityImage);

#if ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >0
	EntityNameTextStyle = MakeShareable(new FTextBlockStyle(FAppStyle::Get().GetWidgetStyle<FTextBlockStyle>("RichTextBlock.Bold")));
#else
	EntityNameTextStyle = MakeShareable(new FTextBlockStyle(FEditorStyle::Get().GetWidgetStyle<FTextBlockStyle>("RichTextBlock.Bold")));
#endif

	DisplayNameTextBlock = SNew(STextBlock)
		.Text(this, &SArticyObjectTileView::OnGetEntityName)
		.TextStyle(EntityNameTextStyle.Get())
		.Justification(ETextJustify::Center);

	Update(ArticyIdToDisplay.Get(FArticyId()));

	SetToolTip(SNew(SArticyObjectToolTip).ObjectToDisplay(ArticyIdToDisplay));

	this->SetOnMouseDoubleClick(InArgs._OnMouseDoubleClick);

	this->ChildSlot
		[
			SAssignNew(WidgetContainerBorder, SBorder)
				.ToolTip(GetToolTip())
				.BorderBackgroundColor(this, &SArticyObjectTileView::OnGetArticyObjectColor)
				.BorderImage(FArticyEditorStyle::Get().GetBrush("ArticyImporter.AssetPicker.TileBorder.16"))
				.HAlign(HAlign_Center)
				.VAlign(VAlign_Center)
				[
					SNew(SBox)
						.WidthOverride(ThumbnailSize.X)
						.HeightOverride(ThumbnailSize.Y)
						[
							SNew(SOverlay)
								+ SOverlay::Slot()
								[
									SNew(SVerticalBox)
										+ SVerticalBox::Slot()
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Center)
										.FillHeight(0.8)
										[
											SNew(SBox)
												.WidthOverride(ThumbnailSize.X)
												.HeightOverride(ThumbnailSize.Y)
												.Padding(ThumbnailPadding)
												.HAlign(HAlign_Center)
												.VAlign(VAlign_Center)
												[
													SNew(SScaleBox)
														.Stretch(EStretch::ScaleToFit)
														[
															PreviewImage.ToSharedRef()
														]
												]
										]
										+ SVerticalBox::Slot()
										.HAlign(HAlign_Center)
										.VAlign(VAlign_Bottom)
										.AutoHeight()
										//.Padding(2, 0, 2, 3)
										[
											SNew(SScaleBox)
												.Stretch(EStretch::ScaleToFit)
												.Visibility(LabelVisibility)
												[
													DisplayNameTextBlock.ToSharedRef()
												]
										]
								]
								// top left type image: only visible if there is an actual preview image to indicate the type
								+ SOverlay::Slot()
								.HAlign(HAlign_Left)
								.VAlign(VAlign_Top)
								.Padding(0.f, 3.f)
								[
									SNew(SBox)
										.WidthOverride(WidthScaleAttribute)
										.HeightOverride(HeightScaleAttribute)
										[
											SNew(SImage)
												.Image(this, &SArticyObjectTileView::GetTypeImage)
												.Visibility(this, &SArticyObjectTileView::OnHasPreviewImage)
										]
								]
						]
				]
		];
}

/**
 * @brief Ticks the widget for updates.
 *
 * This method is called each frame and updates the widget if the Articy ID has changed.
 *
 * @param AllottedGeometry The geometry of the widget.
 * @param InCurrentTime The current time.
 * @param InDeltaTime The time elapsed since the last tick.
 */
void SArticyObjectTileView::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	// if the Id is different from the cached Id, update the widget
	if (CachedArticyId != ArticyIdToDisplay.Get() || (!CachedArticyObject.IsValid() && !CachedArticyId.IsNull()))
	{
		Update(ArticyIdToDisplay.Get());
	}
}

/**
 * @brief Gets the entity name to display.
 *
 * @return The display name of the Articy object.
 */
FText SArticyObjectTileView::OnGetEntityName() const
{
	return FText::FromString(UserInterfaceHelperFunctions::GetDisplayName(CachedArticyObject.Get()));
}

/**
 * @brief Gets the entity image to display.
 *
 * @return The brush to use for the entity image.
 */
const FSlateBrush* SArticyObjectTileView::OnGetEntityImage() const
{
	if (PreviewBrush.GetRenderingResource().IsValid())
	{
		return &PreviewBrush;
	}

	return &PreviewBrush;
}

/**
 * @brief Determines if the preview image is visible.
 *
 * @return The visibility state of the preview image.
 */
EVisibility SArticyObjectTileView::OnHasPreviewImage() const
{
	return bHasPreviewImage ? EVisibility::Visible : EVisibility::Hidden;
}

/**
 * @brief Gets the color to use for the Articy object border.
 *
 * @return The color for the border.
 */
FSlateColor SArticyObjectTileView::OnGetArticyObjectColor() const
{
	return UserInterfaceHelperFunctions::GetColor(CachedArticyObject.Get());
}

/**
 * @brief Gets the type image to display.
 *
 * @return The brush to use for the type image.
 */
const FSlateBrush* SArticyObjectTileView::GetTypeImage() const
{
	return TypeImage;
}

/**
 * @brief Opens the context menu for the widget.
 *
 * This method constructs the context menu with copy and paste actions if available.
 *
 * @param Builder The menu builder for constructing the context menu.
 */
void SArticyObjectTileView::OnContextMenuOpening(FMenuBuilder& Builder) const
{
	// Hide separator line if it only contains the SearchWidget, making the next 2 elements the top of the list
	if (Builder.GetMultiBox()->GetBlocks().Num() > 1)
	{
		Builder.AddMenuSeparator();
	}

	if (CopyAction.IsBound())
	{
		Builder.AddMenuEntry(
			NSLOCTEXT("PropertyView", "CopyProperty", "Copy"),
			NSLOCTEXT("PropertyView", "CopyProperty_ToolTip", "Copy this property value"),
			FSlateIcon(FCoreStyle::Get().GetStyleSetName(), "GenericCommands.Copy"),
			CopyAction);

	}
	if (PasteAction.IsBound() && !bIsReadOnly.Get())
	{
		Builder.AddMenuEntry(
			NSLOCTEXT("PropertyView", "PasteProperty", "Paste"),
			NSLOCTEXT("PropertyView", "PasteProperty_ToolTip", "Paste the copied value here"),
			FSlateIcon(FCoreStyle::Get().GetStyleSetName(), "GenericCommands.Paste"),
			PasteAction);
	}
}

/**
 * @brief Handles mouse button down events.
 *
 * This method captures the mouse on right-click to open the context menu.
 *
 * @param MyGeometry The geometry of the widget.
 * @param MouseEvent The mouse event information.
 * @return A reply indicating how the event was handled.
 */
FReply SArticyObjectTileView::OnMouseButtonDown(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		FReply Reply = FReply::Handled();
		// this will register this widget as prioritised widget for the mouse button up event, even if child widgets would handle the button up event
		Reply.CaptureMouse(SharedThis(this));
		return Reply;
	}

	return FReply::Unhandled();
}

/**
 * @brief Handles mouse button up events.
 *
 * This method opens the context menu on right-click.
 *
 * @param MyGeometry The geometry of the widget.
 * @param MouseEvent The mouse event information.
 * @return A reply indicating how the event was handled.
 */
FReply SArticyObjectTileView::OnMouseButtonUp(const FGeometry& MyGeometry, const FPointerEvent& MouseEvent)
{
	if (MouseEvent.GetEffectingButton() == EKeys::RightMouseButton)
	{
		FMenuBuilder MenuBuilder(true, nullptr, nullptr, true);

		OnContextMenuOpening(MenuBuilder);
		FWidgetPath WidgetPath = MouseEvent.GetEventPath() != nullptr ? *MouseEvent.GetEventPath() : FWidgetPath();
		FSlateApplication::Get().PushMenu(AsShared(), WidgetPath, MenuBuilder.MakeWidget(), MouseEvent.GetScreenSpacePosition(), FPopupTransitionEffect::ContextMenu);

		return FReply::Handled();
	}

	return FReply::Unhandled();
}

#undef LOCTEXT_NAMESPACE
