# EFAS Libraries - Developer Guidance

This document outlines the workflow for using and developing the EFAS C++ ecosystem (`Utility`, `Database`, `DataAccess`, `DataPackage`, `AI`, `HydraulicModel`, ...) using **vcpkg** and **Modern CMake**.

## 📋 Table of Contents
1.  [Prerequisites](#prerequisites)
2.  [Consumer Workflow: Creating a New Client App](#1-consumer-workflow-creating-a-new-client-app)
3.  [Maintainer Workflow: Modifying & Updating Libraries](#2-maintainer-workflow-modifying--updating-libraries)

---

## Prerequisites

Ensure your environment is set up:
*   **Visual Studio 2019/2022** (C++ Desktop Workload).
*   **CMake** (Version 3.20 or higher).
*   **Git** (optional with SSH or OAuth Configured for private repos).
*   **vcpkg** installed and integrated (`vcpkg integrate install`).
*   **Environment Variable**: `VCPKG_USE_HARDLINKS = 1` (Highly recommended to save disk space).

---

## 1. Consumer Workflow: Creating a New Client App

Follow these steps to create a new C++ Application (Console, Qt, or Tauri Backend) that uses EFAS libraries.

### Step 1.1: Configure `vcpkg.json`
Create a `vcpkg.json` in your project root to declare dependencies.

```json
{
  "name": "my-new-app",
  "version": "1.0.0",
  "dependencies": [
    "efas-datapackage", 
    "efas-ai",
    "cpr",
    "eigen3"
  ],
  "builtin-baseline": "YOUR_CURRENT_BASELINE_HASH"
}
```

### 1.2 Configure `vcpkg-configuration.json`

Create this file next to vcpkg.json to tell vcpkg where to find EFAS libraries (Custom Registry).

```json
{
  "default-registry": {
    "kind": "git",
    "repository": "https://github.com/microsoft/vcpkg",
    "baseline": "2e65c0be072239da927899a229a397984be5c853" 
  },
  "registries": [
    {
      "kind": "git",
      "repository": "git@github.com:EFAS-Technologies/efas-vcpkg-registry.git",
      "baseline": "LATEST_COMMIT_HASH_OF_YOUR_REGISTRY",
      "packages": [ 
        "efas-utility", "efas-database", "efas-dataaccess", 
        "efas-hydraulicmodel", "efas-datapackage", "efas-ai", 
        "epanet", "onnxruntime", "efas-gis" 
      ]
    }
  ]
}
```
### 1.3 Configure `CMakeLists.txt`

Use find_package to load the libraries. Note that the CMake Package Name might differ from the vcpkg Package Name.

Vcpkg Name	CMake Find Name	Target Name
efas-utility	Utility	EFAS::Utility
efas-database	Database	EFAS::Database
efas-ai	AI	EFAS::AI
epanet	epanet	epanet::epanet
onnxruntime	ONNXRuntime	onnxruntime::onnxruntime

Example CMakeLists.txt:

```cmake
cmake_minimum_required(VERSION 3.20)
project(MyClientApp LANGUAGES CXX)

# Global compiler settings
if(MSVC)
    add_compile_definitions(NOMINMAX) # Important for Windows
    add_compile_options(/bigobj)      # Important for Eigen/Boost
    add_compile_options(/utf-8)
endif()

# Find Packages
find_package(DataPackage CONFIG REQUIRED)
find_package(AI CONFIG REQUIRED)

add_executable(MyClientApp main.cpp)

# Link Libraries
target_link_libraries(MyClientApp PRIVATE
    EFAS::DataPackage
    EFAS::AI
)
```

### 1.4 Configure `CMakePresets.json`

Standardize build settings using presets.

```json
{
  "version": 3,
  "configurePresets": [
    {
      "name": "windows-base",
      "hidden": true,
      "generator": "Visual Studio 16 2019",
      "architecture": "x64",
      "binaryDir": "${sourceDir}/build",
      "cacheVariables": {
        "CMAKE_TOOLCHAIN_FILE": "C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "VCPKG_TARGET_TRIPLET": "x64-windows"
      }
    },
    {
      "name": "windows-release",
      "inherits": "windows-base",
      "cacheVariables": { "CMAKE_BUILD_TYPE": "Release" }
    }
  ],
  "buildPresets": [
    {
      "name": "release",
      "configurePreset": "windows-release",
      "configuration": "Release"
    }
  ]
}
```
### 1.5 Configure and Build

Once all files are set up, run the build process. vcpkg will automatically download and compile all dependencies (this may take a while for the first run).
Using Command Line (PowerShell) from terminal of your ide

```powershell
# 1. Configure (Generates build files and runs vcpkg install)
cmake --preset windows-release

# 2. Build (Compiles the code)
cmake --build --preset release
```

## 2. Maintainer Workflow: Modifying & Updating Libraries

When you need to fix a bug or dev new feature in a library (e.g., Utility) and update it for the Client App, follow this 3-Step Cycle.

### Phase A: Modify Source Code (The Library Repo)

1. Open the library repo (e.g., efas-utility).
2. Make changes to the code.
3. Local Test: You can try to build it locally using its own vcpkg.json to ensure it compiles.
4. Git Commit & Push:
   ```powershell
   git add .
   git commit -m "Fix bug in systemobserver"
   git push origin main
   ```
5. Get the Commit Hash:
   ```powershell
   git rev-parse HEAD
   # Example Output: a1b2c3d... (Copy this)

### Phase B: Update Registry (The Metadata)

1. Open the Registry repo (efas-vcpkg-registry).
2. Edit ports/efas-utility/portfile.cmake.
3. Update the REF field with the new Commit Hash from Phase A.
   ```cmake
   vcpkg_from_git(
       ...
       REF a1b2c3d... # Paste new hash here
       ...
   )
   ```
4. [IMPORTANT] Commit the Portfile changes first:
   Vcpkg needs this commit to calculate the correct tree hash.
   ```powershell
   git add ports/efas-utility/portfile.cmake
   git commit -m "Update efas-utility ref to a1b2c3d"
   ```
5. Update Version Database:
   ```powershell
   # Replace path to your vcpkg.exe
   vcpkg x-add-version efas-utility --overwrite-version --x-builtin-ports-root=./ports --x-builtin-registry-versions-dir=./versions
   ```
6. Commit the Version Database changes & Push:
   Now commit the changes that vcpkg just made to the versions/ folder.
   ```powershell
   git add versions/
   git commit -m "Update version database for efas-utility"
   git push origin main
   ```
7. Get Registry Commit Hash:
   ```powershell
   git rev-parse HEAD
   # Example Output: 998877... (Copy this for Phase C)
   ```

### Phase C: Update Client (The Application)

1. Open your Client Project.
2. Open vcpkg-configuration.json.
3. Update the baseline of your custom registry with the hash from Phase B.
   ```json
   "registries": [
     {
       "kind": "git",
       "repository": "...",
       "baseline": "998877..." // Paste Registry hash here
     }
   ]
   ```
4. Rebuild:
   Delete the build folder (Recommended to ensure clean update).
   Run CMake Configure. Vcpkg will detect the change, download the new source code, and rebuild the library.
