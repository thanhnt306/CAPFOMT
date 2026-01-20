# EFAS Libraries System

This repository contains the core C++ libraries for the EFAS project (`Utility`, `Database`, `DataAccess`, `DataPackage`, `AI`, `HydraulicModel`).

The system is built using **Modern CMake**, manages standard dependencies via **vcpkg** (Manifest Mode), and requires manual setup for specific binaries (EPANET, ONNX Runtime). It supports a modular architecture with a unified SDK output.

## 📋 Prerequisites

Before proceeding, ensure your development environment meets the following requirements:

1.  **Visual Studio 2019** (or newer)
    *   Workload required: *Desktop development with C++*.
2.  **CMake** (Version 3.20 or higher).
3.  **Git**.
4.  **PowerShell** (Recommended for running build commands).

---

## 🛠️ Step 1: Setup Dependencies

### 1.1 Setup vcpkg (For Boost, JSON, CPR, Eigen, etc.)

1.  **Clone vcpkg** (If you haven't already):
    ```powershell
    cd C:/Dev  # Or your preferred tools directory
    git clone https://github.com/microsoft/vcpkg.git
    cd vcpkg
    .\bootstrap-vcpkg.bat
    ```

2.  **Enable User-wide Integration**:
    ```powershell
    .\vcpkg integrate install
    ```

3.  **Enable Hardlinks (Highly Recommended)**:
    To save disk space, set the following Environment Variable in Windows (or intergrate in CMakePresets.json at step 2):
    *   **Variable:** `VCPKG_USE_HARDLINKS`
    *   **Value:** `1`

4.  **Configure `vcpkg.json`**:
    Each library in this project (`Utility`, `AI`, etc.) uses a `vcpkg.json` file to manage dependencies.
    You must create or update this file in the **root folder of EACH library**.
    ```json
    {
        "name": "efas-dependencies",
        "version": "1.0.0",
        "dependencies": [
          "boost-filesystem",
          "boost-locale",
          "boost-system",
          "boost-graph",
          "boost-regex",
          "boost-container",
          "boost-geometry",
          "nlohmann-json",
          "spdlog",
          "eigen3",
          "spectra",
          "asio",
          "crow",
          "cpr",
          "nanoflann",
          "gdal",
          "sqlite3", 
          "libpq",   
          "libpqxx"
        ],
        "builtin-baseline": "128988b03fc9216012e4983a81da9e86911230f8"
    }
    ```

### 1.2 Setup Manual Libraries (For EPANET & ONNX Runtime)

Since `epanet` and `onnxruntime` are not used via vcpkg in this configuration, you must download their binaries manually.

1.  **EPANET 2.2**: Download and extract to a known location (e.g., `C:/Libs/epanet2_2`).
2.  **ONNX Runtime**: Download the Windows x64 zip from GitHub Releases and extract (e.g., `C:/Libs/onnxruntime-win-x64`).

---

## ⚙️ Step 2: Configure `CMakePresets.json`

Every library in this project (`Utility`, `AI`, etc.) uses a `CMakePresets.json` file to manage paths.

You must create or update this file in the **root folder of EACH library**. Ensure the paths match your local machine.

**Template for `CMakePresets.json`:**

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
      "environment": {
        "VCPKG_USE_HARDLINKS": "1"
      },
      "cacheVariables": {
        "CMAKE_BUILD_TYPE": "Release",

        // [VCPKG CONFIG]
        "CMAKE_TOOLCHAIN_FILE": "C:/Dev/vcpkg/scripts/buildsystems/vcpkg.cmake",
        "VCPKG_TARGET_TRIPLET": "x64-windows",
        "VCPKG_INSTALLED_DIR": "C:/Dev/EFAS_Libs/Common_Vcpkg",

        // [SDK CONFIG]
        // All libraries will install to this common folder:
        "CMAKE_INSTALL_PREFIX": "${sourceDir}/../EFAS_SDK",
        // Libraries will look for dependencies in this folder:
        "CMAKE_PREFIX_PATH": "${sourceDir}/../EFAS_SDK",

        // [MANUAL LIBS PATHS]
        // Update these to where you extracted the files in Step 1.2
        "EpanetLib_ROOT": "C:/Libs/epanet2_2",
        "ONNXRuntime_ROOT": "C:/Libs/onnxruntime-win-x64"
      }
    },
    {
      "name": "windows-release",
      "displayName": "Windows Release (x64)",
      "inherits": "windows-base"
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

---

## 🚀 Step 3: Build and Install

Due to dependency chains, you **MUST** build and install the libraries in the following **Bottom-Up Order**:

1.  **Utility** (Core)
2.  **Database** (Depends on Utility)
3.  **HydraulicModel** (Depends on Utility, Epanet)
4.  **DataAccess** (Depends on Utility, Database, HydraulicModel)
5.  **DataPackage** (Depends on all above)
6.  **AI** (Depends on all above + ONNX)

### Build Commands

For **each library** in the order above, open a terminal in its root folder and run:

1.  **Configure**:
    ```powershell
    cmake --preset windows-release
    ```

2.  **Build**:
    ```powershell
    cmake --build --preset release
    ```

3.  **Install** (Copies files to `EFAS_SDK`):
    ```powershell
    cmake --install build
    ```

> **Note:** If `cmake --install` fails, verify that the previous library in the chain was successfully installed into `EFAS_SDK`.

---

## 📦 Step 4: Verify Output

After building all libraries, you should have a unified **`EFAS_SDK`** folder (located parallel to your project folders) with this structure:

```text
EFAS_SDK/
├── bin/        # Runtime DLLs (Utility.dll, cpr.dll, onnxruntime.dll...)
├── lib/        # Import Libs (.lib)
│   └── cmake/  # CMake Config files (UtilityConfig.cmake, etc.)
└── include/    # Headers
    ├── Utility/
    ├── Database/
    ├── DataAccess/
    └── ...
