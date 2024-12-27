# LLVM IR Optimization Playground

## Project Overview
This project demonstrates how to analyze and optimize LLVM IR (Intermediate Representation) using custom LLVM passes. Users can write C code, convert it to LLVM IR using Clang, and then apply a sequence of custom and built-in optimizations to explore the effects. This project is educational, focusing on learning compiler optimization techniques and data-flow analysis, such as dominator trees and SSA construction.

---

## Features
- Convert C code to LLVM IR using Clang.
- Apply custom LLVM passes written with the LLVM PassManager.
- Chain built-in passes with custom passes in a user-defined order.
- Visualize intermediate LLVM IR to understand transformations.

---

## Prerequisites
To use this project, ensure the following tools are installed:

1. **LLVM/Clang** (with the `opt` tool):
   - Download from [LLVM Releases](https://github.com/llvm/llvm-project/releases).
   - Add LLVM binaries (e.g., `clang`, `opt`) to your PATH.

2. **CMake**: Used for building the project.
   - Download from [CMake.org](https://cmake.org/).

3. **C++ Compiler**: Visual Studio (Windows), GCC/Clang (Linux/Mac).

4. (Optional) **MSYS2**: For an alternative LLVM installation on Windows.

---

## Installation
### Clone the Repository
```bash
git clone https://github.com/yourusername/llvm-ir-optimization-playground.git
cd llvm-ir-optimization-playground
```

### Build the Project
1. Create a build directory:
   ```bash
   mkdir build && cd build
   ```

2. Run CMake:
   ```bash
   cmake ..
   ```

3. Build the shared library:
   ```bash
   make
   ```

The shared library (e.g., `libMyPasses.so` or `MyPasses.dll`) will be created in the `build` directory.

---

## Usage
### Step 1: Generate LLVM IR from C Code
Use Clang to compile your C code to LLVM IR:
```bash
clang -O0 -S -emit-llvm test.c -o test.ll
```
This generates an unoptimized LLVM IR file `test.ll` from the input `test.c`.

### Step 2: Apply Optimization Passes
Run `opt` with your custom pass:
```bash
opt -load-pass-plugin=./libMyPasses.so \
    -passes="function(mem2reg),function(my-pass),module(verify)" \
    < test.ll > optimized.ll
```
- Replace `my-pass` with the name of your custom pass.
- The resulting optimized LLVM IR will be written to `optimized.ll`.

### Step 3: Inspect the Output
View the optimized LLVM IR:
```bash
cat optimized.ll
```

---

## Folder Structure
```
llvm-ir-optimization-playground/
├── CMakeLists.txt         # Build configuration
├── README.md              # Project documentation
├── scripts/               # Utility scripts
│   ├── build.sh           # Build helper script
│   └── run.sh             # Example usage script
├── src/                   # Source files for custom passes
│   ├── main.cpp           # Optional: Entry point for standalone tools
│   └── passes/            # Custom LLVM passes
│       ├── MyPass1.cpp    # Example custom pass
│       ├── MyPass2.cpp    # Another custom pass
│       └── ...
├── include/               # Headers for custom passes
│   ├── MyPass1.h
│   ├── MyPass2.h
│   └── ...
└── test/                  # Test input files
    ├── test1.c            # Example C code for testing
    ├── test2.c
    └── ...
```

---

## License
This project is licensed under the MIT License. See `LICENSE` for details.

