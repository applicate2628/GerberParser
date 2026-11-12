# G04 comment regression tests

Run from a Windows developer shell with the Windows SDK, CMake 3.20 or newer,
Ninja and an MSVC-compatible C++ compiler already available. Choose an output
path outside the checkout; these commands do not install dependencies.

```text
cmake -S tests -B <build-directory> -G Ninja -DCMAKE_CXX_COMPILER=clang-cl -DCMAKE_BUILD_TYPE=Debug
cmake --build <build-directory>
ctest --test-dir <build-directory> --output-on-failure
```

Use a separate build directory with `CMAKE_BUILD_TYPE=Release` for the optimized
configuration. The test executable calls the real `CGerberParser::Parse`.
Five cases preserve valid empty, ordinary, successive, multiline and long
comments. Four cases require the existing `const char*` error convention with
`Unterminated G04 comment` when the terminator is missing, including after a
valid comment. Each case runs in its own process with a ten-second timeout.
Assertion/error dialogs are disabled only in that test process.

The standalone target compiles the existing root library sources. A test-only
precompiled header supplies `<windows.h>` and `<string>` before legacy headers;
this avoids changing unrelated production include-order assumptions. It does
not repair or qualify the historical Visual Studio projects, which refer to
untracked `Composite.cpp` and `Composite.h` files.

This is a bounded regression for G04 truncation, not full Gerber grammar,
renderer, memory-ownership, sanitizer or platform qualification. No automated
workflow is added or required.
