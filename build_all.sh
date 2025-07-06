#!/bin/bash

zig fetch git+https://github.com/emscripten-core/emsdk#4.0.9

declare -a targets=(
    # "x86_64-linux-musl"     # Linux (64-bit) using musl libc
    # "aarch64-linux-musl"    # ARM64 Linux using musl libc
    # "x86_64-windows-gnu"    # Windows (64-bit, using MinGW/GNU toolchain)
    # "aarch64-macos"         # macOS (Apple Silicon)
    # "x86_64-macos"          # macOS (Intel)
    "wasm32-emscripten"     # WebAssembly via Emscripten (for web browsers)
    # "arm-linux-gnueabihf"   # ARMv7 Linux (e.g., Raspberry Pi 3/4)
    # "i386-linux-musl"       # Linux (32-bit) using musl libc
)

# Define optimization level
OPTIMIZE_FLAG="-Doptimize=ReleaseFast"

echo "Starting multi-target Zig build..."
echo "Optimization level: ${OPTIMIZE_FLAG}"
echo "-------------------------------------"

# Loop through each target and build
for target in "${targets[@]}"; do
    echo "Building for target: ${target}..."
    # Execute the zig build command
    zig build ${OPTIMIZE_FLAG} -Dtarget="${target}"

    # Check the exit status of the last command
    if [ $? -eq 0 ]; then
        echo "Successfully built for ${target}."
    else
        echo "Error: Failed to build for ${target}."
        echo "Please check the error messages above for details."
        # Optionally, you can exit the script on the first failure:
        exit 1
    fi
    echo "-------------------------------------"
done

echo "Multi-target Zig build process completed."
