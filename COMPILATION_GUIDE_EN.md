# Build Guide — Il2CppDumper Extended (English)

This is the English version of `COMPILATION_GUIDE.md`.

## Requirements
- Android NDK r21+
- Gradle 7+
- JDK 8 or 11
- Git

Optional: Android Studio, CMake 3.10+

## Environment setup
Windows (PowerShell / Chocolatey):
```powershell
choco install androidstudio gradle temurin11
```
Linux (Ubuntu/Debian):
```bash
sudo apt update && sudo apt install -y openjdk-11-jdk gradle git
```
macOS (Homebrew):
```bash
brew install openjdk@11 gradle
brew install --cask android-studio
```

## Build (Gradle)
```bash
git clone https://github.com/VladKalinov/Internal-Il2cppDumper-Android.git
cd Internal-Il2cppDumper-Android
./gradlew assembleRelease
```
Artifacts:
```
app/build/intermediates/cmake/release/obj/<abi>/libil2cppdumper.so
```

## Build (ndk-build)
```bash
cd app/src/main/jni
$ANDROID_NDK_HOME/ndk-build APP_ABI=arm64-v8a
```

## Configuration (`app/src/main/jni/Includes/config.h`)
```cpp
#define Sleep 2
#define UseFakeLib
#define EnableExtendedDump
```

## Common issues
- NDK not found: install via sdkmanager or set `ndk.dir` in `local.properties`.
- `undefined reference to 'il2cpp_base'`: ensure it is non-static in `il2cpp_dump.cpp`.
- Missing `il2cpp_dump_extended.cpp` in build: add it to `Android.mk` `LOCAL_SRC_FILES`.

See original `COMPILATION_GUIDE.md` for more details.
