param (
    [ValidateSet("vs2022", "vs2019", "clean")]
    [string]$action = "vs2022",

    [ValidateSet("Debug", "Release")]
    [string]$config = "Debug",

    [switch]$runSandbox
)

# Helper: colored output
function Write-Info($msg)    { Write-Host "[INFO]  $msg" -ForegroundColor Cyan }
function Write-Warn($msg)    { Write-Host "[WARN]  $msg" -ForegroundColor Yellow }
function Write-Success($msg) { Write-Host "[OK]    $msg" -ForegroundColor Green }
function Write-ErrorMsg($msg){ Write-Host "[ERROR] $msg" -ForegroundColor Red }
function Write-Section($title) { Write-Host "`n=== $title ===`n" -ForegroundColor Magenta }

# Project root
$rootDir = Resolve-Path "$PSScriptRoot\.."

# MSBuild executable (assumes in PATH)
$msbuild = "MSBuild.exe"

# Output paths
$outputDir = "$rootDir\bin\$config-windows-x86_64"
$engineLib  = Join-Path $outputDir "Engine\Engine.lib"
$sandboxExe = Join-Path $outputDir "Sandbox\Sandbox.exe"

# Show file size
function Show-FileSize($path) {
    if (Test-Path $path) {
        $sizeMB = [math]::Round((Get-Item $path).Length / 1MB, 2)
        Write-Host "`t$path ($sizeMB MB)"
    } else {
        Write-Host "`t$path (not found)"
    }
}

# Build a single project by name
function Build-Project($projectName) {
    Write-Section "Building project: $projectName"
    $projPath = Join-Path $rootDir "$projectName\$projectName.vcxproj"
    if (-not (Test-Path $projPath)) { Write-Warn "Project file not found: $projPath"; return }

    $projTimer = [System.Diagnostics.Stopwatch]::StartNew()
    & $msbuild $projPath /p:Configuration=$config /p:Platform="x64" /m | ForEach-Object {
        if ($_ -match ":\s+error\s+[A-Z0-9]+:") { Write-ErrorMsg $_ }
        elseif ($_ -match ":\s+warning\s+[A-Z0-9]+:") { Write-Warn $_ }
        else { Write-Host $_ }
    }
    $projTimer.Stop()
    Write-Success "Finished $projectName in $([math]::Round($projTimer.Elapsed.TotalSeconds,2)) seconds."
}

# Timer for whole solution
$solutionTimer = [System.Diagnostics.Stopwatch]::StartNew()

switch ($action) {
    "clean" {
        Write-Info "Cleaning build folders..."
        Remove-Item -Recurse -Force (Join-Path $rootDir "bin"), (Join-Path $rootDir "bin-int") -ErrorAction SilentlyContinue
        Remove-Item -Recurse -Force (Join-Path $rootDir ".vs"), $solution.FullName, (Join-Path $rootDir "Engine\*.vcxproj*"), (Join-Path $rootDir "Sandbox\*.vcxproj*") -ErrorAction SilentlyContinue

        Write-Info "Regenerating solution with Premake..."
        premake5 vs2022
    }

    default {
        Write-Info "Regenerating solution with Premake..."
        premake5 $action

        # Build each project individually
        Build-Project "Engine"
        Build-Project "Sandbox"
    }
}

$solutionTimer.Stop()
Write-Success "`nTotal build time: $([math]::Round($solutionTimer.Elapsed.TotalSeconds,2)) seconds.`n"

Write-Host "Generated binaries:"
Show-FileSize $engineLib
Show-FileSize $sandboxExe

if ($runSandbox.IsPresent -and (Test-Path $sandboxExe)) {
    Write-Info "Running Sandbox..."
    & $sandboxExe
}
