param (
    [ValidateSet("vs2022", "vs2019", "clean")]
    [string]$action = "vs2022",

    [ValidateSet("Debug", "Release")]
    [string]$config = "Debug"
)

$premakeExe = Join-Path $PSScriptRoot "..\vendor\bin\premake5.exe"

if (-not (Test-Path $premakeExe)) {
    Write-Host "Error: premake5.exe not found in vendor/bin"
    exit 1
}

switch ($action) {
    "clean" {
        Write-Host "Cleaning build folders..."
        Remove-Item -Recurse -Force ..\bin, ..\bin-int -ErrorAction SilentlyContinue
        Remove-Item -Recurse -Force ..\.vs, ..\*.sln, ..\Engine\*.vcxproj* -ErrorAction SilentlyContinue
    }
    default {
        Write-Host "Running premake with action: $action"
        & $premakeExe $action

        Write-Host "Building solution..."
        msbuild.exe ..\MyGameEngine.sln /p:Configuration=$config /m
    }
}
