# Obfuscate++

**Advanced methods to obfuscate strings and data in binaries built from C++14 source code**


## Für c’t- und heise+-Leser

![heise+](resources/heiseplus.svg)

Die Version mit dem Tag CT1123 ist die [bei heise+ veröffentlichte](https://heise.de/-8526330). Nach dem Klonen bitte auschecken mit

```bash
git checkout CT1123
```


## Build

### Linux, macOS

```bash
git clone https://github.com/607011/obfuscateplusplus.git obfuscate++
cd obfuscate++
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

`-G Ninja` can be omitted on platforms not providing the Ninja build tool.

### Windows

In Visual Studio Developer Console:

```
git clone https://github.com/607011/obfuscateplusplus.git obfuscate++
cd obfuscate++
mkdir build
cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Release ..
cmake --build .
```

`-G Ninja` can be omitted on platforms not providing the Ninja build tool.

#### Prerequisites

```
winget install Kitware.CMake
winget install Git.Git
winget install Microsoft.WindowsSDK
winget install Python.Python.3.10
```
