# Obfuscate++

**Advanced methods to obfuscate strings and data in binaries built from C++14 source code**


## Build

### Linux, macOS

```bash
git clone https://github.com/607011/obfuscateplusplus.git obfuscate++
cd obfuscate++
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .
cmake --build .
```

### Windows

In Visual Studio Command Prompt:

```
git clone https://github.com/607011/obfuscateplusplus.git obfuscate++
cd obfuscate++
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release .
cmake --build .
```

#### Prerequisites

```
winget install Kitware.CMake
winget install Git.Git
winget install Microsoft.WindowsSDK
winget install Python.Python.3.10
```
