using UnrealBuildTool;
using System;
using System.IO;
using System.Diagnostics;
using System.Runtime.InteropServices;
#if WINDOWS
using System.Windows.Forms;
#endif

public class ManiacManfredEditorTarget : TargetRules
{
    public ManiacManfredEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        ExtraModuleNames.Add("ManiacManfred");
        IncludeOrderVersion = EngineIncludeOrderVersion.Latest;

        // Run pre-build script before Unreal starts compiling
        RunPreBuildScript(Target);
    }

    private static void RunPreBuildScript(TargetInfo Target)
    {
        string PluginName = "ArticyXImporter";
        string ConflictingPlugin = "Articy3Importer";
        string ProjectPluginsDir = Path.Combine(Target.ProjectFile.Directory.ToString(), "Plugins");
        string PluginPath = Path.Combine(ProjectPluginsDir, PluginName);
        string ScriptPathWin = Path.Combine(Target.ProjectFile.Directory.ToString(), "DownloadPlugin.ps1");
        string ScriptPathUnix = Path.Combine(Target.ProjectFile.Directory.ToString(), "DownloadPlugin.sh");

        // Check if the conflicting plugin exists
        if (Directory.Exists(Path.Combine(ProjectPluginsDir, ConflictingPlugin)))
        {
            throw new BuildException($"ERROR: Conflicting plugin '{ConflictingPlugin}' found! Remove it before installing '{PluginName}'.");
        }

        // Only install plugin if it's missing
        if (!Directory.Exists(PluginPath))
        {
            Console.WriteLine($"Plugin {PluginName} not found! Running pre-build script...");

            try
            {
                if (Environment.OSVersion.Platform == PlatformID.Win32NT && File.Exists(ScriptPathWin))
                {
                    Process.Start("powershell.exe", $"-ExecutionPolicy Bypass -File \"{ScriptPathWin}\"").WaitForExit();
                }
                else if (File.Exists(ScriptPathUnix))
                {
                    Process.Start("/bin/sh", $"\"{ScriptPathUnix}\"").WaitForExit();
                }
                else
                {
                    Console.WriteLine("Pre-Build script not found! Plugin may not install correctly.");
                }
                Console.WriteLine($"Plugin installed. Restart the build.");
                Environment.Exit(-1);
            }
            catch (Exception ex)
            {
                throw new BuildException($"Pre-Build script failed: {ex.Message}");
            }
        }
        else
        {
            Console.WriteLine($"Plugin '{PluginName}' is already installed.");
        }
    }

}
