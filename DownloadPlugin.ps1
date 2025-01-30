param (
    [string]$pluginName = "ArticyXImporter",
    [string]$conflictingPlugin = "Articy3Importer",
    [string]$pluginURL = "https://github.com/ArticySoftware/ArticyXImporterForUnreal/archive/refs/heads/master.zip"
)

$projectPluginsDir = "$PSScriptRoot\Plugins"
$enginePluginsDir = "${env:UE_ENGINE_PATH}\Plugins"

$pluginPathProject = "$projectPluginsDir\$pluginName"
$pluginPathEngine = "$enginePluginsDir\$pluginName"
$conflictingPathProject = "$projectPluginsDir\$conflictingPlugin"
$conflictingPathEngine = "$enginePluginsDir\$conflictingPlugin"

if ((Test-Path $conflictingPathProject) -or (Test-Path $conflictingPathEngine)) {
    Write-Host "ERROR: Conflicting plugin '$conflictingPlugin' found! Please remove it before installing '$pluginName'."
    exit 1
}

if ((Test-Path $pluginPathProject) -or (Test-Path $pluginPathEngine)) {
    Write-Host "Plugin '$pluginName' is already installed. Skipping download."
    exit 0
}

Write-Host "Downloading plugin from $pluginURL..."
$zipPath = "$projectPluginsDir\$pluginName.zip"
Invoke-WebRequest -Uri $pluginURL -OutFile $zipPath

Write-Host "Extracting plugin..."
Expand-Archive -Path $zipPath -DestinationPath $projectPluginsDir -Force

Write-Host "Renaming extracted folder..."
Rename-Item -Path "$projectPluginsDir\ArticyXImporterForUnreal-master" -NewName $pluginName -Force

Write-Host "Cleaning up..."
Remove-Item -Path $zipPath -Force

Write-Host "Plugin installed successfully."
exit 0
