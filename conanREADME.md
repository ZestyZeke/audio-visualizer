
steps for conan...
mkdir .conan && cd .conan

conan search range* --remote=conan-center

conanfile.txt
-------------
[requires]
<info from conan search>
[generators]
cmake

conan install .. -s compiler=clang -s compiler.version=7.0 -s compiler.libcxx=libstdc++11 --build=missing

edit top level CMakeLists.txt
-----------------------------
...
include(.conan/conanbuildinfo.cmake)
conan_basic_setup()
...
