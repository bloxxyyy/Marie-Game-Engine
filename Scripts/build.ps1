param (
    [ValidateSet("vs2022", "vs2019", "clean")]
    [string]$action = "vs2022",

    [ValidateSet("Debug", "Release")]
    [string]$config = "Debug",

    [switch]$runSandbox
)

# Project root
$rootDir = Resolve-Path "$PSScriptRoot\.."

# Detect first solution file in root
$solution = Get-ChildItem -Path $rootDir -Filter *.sln | Select-Object -First 1

if (-not $solution) {
    Write-Error "No solution file found in $rootDir. Did premake5 run correctly?"
    exit 1
}

# MSBuild executable (assumes in PATH from VS Developer Command Prompt)
$msbuild = "MSBuild.exe"

# Output directories
$outputDir = "$rootDir\bin\$config-windows-x86_64"
$engineLib  = Join-Path $outputDir "Engine\Engine.lib"
$sandboxExe = Join-Path $outputDir "Sandbox\Sandbox.exe"

# Helper to show file size
function Show-FileSize($path) {
    if (Test-Path $path) {
        $sizeMB = [math]::Round((Get-Item $path).Length / 1MB, 2)
        Write-Host "`t$path ($sizeMB MB)"
    } else {
        Write-Host "`t$path (not found)"
    }
}

# Timer
$stopwatch = [System.Diagnostics.Stopwatch]::StartNew()

switch ($action) {
    "clean" {
        Write-Host "Cleaning build folders..."
        Remove-Item -Recurse -Force (Join-Path $rootDir "bin"), (Join-Path $rootDir "bin-int") -ErrorAction SilentlyContinue
        Remove-Item -Recurse -Force (Join-Path $rootDir ".vs"), $solution.FullName, (Join-Path $rootDir "Engine\*.vcxproj*"), (Join-Path $rootDir "Sandbox\*.vcxproj*") -ErrorAction SilentlyContinue

        Write-Host "Regenerating solution..."
        premake5 vs2022
    }

    default {
        Write-Host "Running premake with action: $action..."
        premake5 $action

        Write-Host "Building solution ($config) with MSBuild..."
        & $msbuild $solution.FullName /p:Configuration=$config /m
    }
}

$stopwatch.Stop()
Write-Host "`nBuild completed in $($stopwatch.Elapsed.TotalSeconds) seconds.`n"

# Show resulting binaries
Write-Host "Generated binaries:"
Show-FileSize $engineLib
Show-FileSize $sandboxExe

# Optionally run Sandbox
if ($runSandbox.IsPresent -and (Test-Path $sandboxExe)) {
    Write-Host "`nRunning Sandbox..."
    & $sandboxExe
}
