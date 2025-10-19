# 编译指南 — Il2CppDumper 扩展版（中文）

本文件为 `COMPILATION_GUIDE.md` 的中文版本。

## 依赖
- Android NDK r21+
- Gradle 7+
- JDK 8/11
- Git
（可选）Android Studio、CMake 3.10+

## 环境安装
Windows（PowerShell / Chocolatey）：
```powershell
choco install androidstudio gradle temurin11
```
Linux（Ubuntu/Debian）：
```bash
sudo apt update && sudo apt install -y openjdk-11-jdk gradle git
```
macOS（Homebrew）：
```bash
brew install openjdk@11 gradle
brew install --cask android-studio
```

## 使用 Gradle 构建
```bash
git clone https://github.com/VladKalinov/Internal-Il2cppDumper-Android.git
cd Internal-Il2cppDumper-Android
./gradlew assembleRelease
```
产物：
```
app/build/intermediates/cmake/release/obj/<abi>/libil2cppdumper.so
```

## 使用 ndk-build 构建
```bash
cd app/src/main/jni
$ANDROID_NDK_HOME/ndk-build APP_ABI=arm64-v8a
```

## 配置（`app/src/main/jni/Includes/config.h`）
```cpp
#define Sleep 2
#define UseFakeLib
#define EnableExtendedDump
```

## 常见问题
- 找不到 NDK：用 sdkmanager 安装或在 `local.properties` 设置 `ndk.dir`。
- `undefined reference to 'il2cpp_base'`：确保在 `il2cpp_dump.cpp` 中不是 `static`。
- 缺少 `il2cpp_dump_extended.cpp`：在 `Android.mk` 的 `LOCAL_SRC_FILES` 中加入。

更多细节参见原始 `COMPILATION_GUIDE.md`。
