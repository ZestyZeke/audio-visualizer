# Instructions for reference when using conan
## Create temp workspace and search for package
```bash
mkdir .conan && cd .conan
conan search range* --remote=conan-center
```

## Create top-level conanfile
```bash
conanfile.txt
-------------
[requires]
<info from conan search>

[generators]
cmake
```

## Install Conan packages
```bash
conan install .. -s compiler=clang -s compiler.version=7.0 -s compiler.libcxx=libstdc++11 --build=missing
```

## Edit top level CMakeLists.txt
```bash
CmakeLists.txt
--------------
...
include(.conan/conanbuildinfo.cmake)
conan_basic_setup()
...
```
