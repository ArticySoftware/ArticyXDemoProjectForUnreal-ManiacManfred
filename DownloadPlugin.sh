#!/bin/bash

PLUGIN_NAME="ArticyXImporter"
CONFLICTING_PLUGIN="Articy3Importer"
PLUGIN_URL="https://github.com/ArticySoftware/ArticyXImporterForUnreal/archive/refs/heads/master.zip"
PROJECT_PLUGINS_DIR="$(pwd)/Plugins"
ENGINE_PLUGINS_DIR="$UE_ENGINE_PATH/Plugins"

PLUGIN_PATH_PROJECT="$PROJECT_PLUGINS_DIR/$PLUGIN_NAME"
PLUGIN_PATH_ENGINE="$ENGINE_PLUGINS_DIR/$PLUGIN_NAME"
CONFLICTING_PATH_PROJECT="$PROJECT_PLUGINS_DIR/$CONFLICTING_PLUGIN"
CONFLICTING_PATH_ENGINE="$ENGINE_PLUGINS_DIR/$CONFLICTING_PLUGIN"

if [ -d "$CONFLICTING_PATH_PROJECT" ] || [ -d "$CONFLICTING_PATH_ENGINE" ]; then
    echo "ERROR: Conflicting plugin '$CONFLICTING_PLUGIN' found! Please remove it before installing '$PLUGIN_NAME'."
    exit 1
fi

if [ -d "$PLUGIN_PATH_PROJECT" ] || [ -d "$PLUGIN_PATH_ENGINE" ]; then
    echo "Plugin '$PLUGIN_NAME' is already installed. Skipping download."
    exit 0
fi

echo "Downloading plugin from $PLUGIN_URL..."
ZIP_PATH="$PROJECT_PLUGINS_DIR/$PLUGIN_NAME.zip"
curl -L $PLUGIN_URL -o $ZIP_PATH

echo "Extracting plugin..."
unzip -o $ZIP_PATH -d "$PROJECT_PLUGINS_DIR/"

echo "Renaming extracted folder..."
mv "$PROJECT_PLUGINS_DIR/ArticyXImporterForUnreal-master" "$PLUGIN_PATH_PROJECT"

echo "Cleaning up..."
rm $ZIP_PATH

echo "Plugin installed successfully."
exit 0
