# 🚀 Quick Start — Il2CppDumper Extended (English)

Language / Язык / 语言:

- English: [QUICKSTART_EN.md](QUICKSTART_EN.md)
- Русский: [QUICKSTART.md](QUICKSTART.md)
- 中文: [QUICKSTART_ZH.md](QUICKSTART_ZH.md)

## Dump in 5 Minutes

### 1) Get the library
- Build with Gradle: `./gradlew assembleRelease`
- Or use a prebuilt release (if provided).

### 2) Install on device (Root Fake Lib method)
```bash
adb push libunity.so /sdcard/
adb shell
su
cd /data/data/com.example.game/lib
mv libunity.so librealunity.so
cp /sdcard/libunity.so ./
chmod 755 libunity.so
exit
exit
```

### 3) Launch the game
```bash
adb shell am start -n com.example.game/.MainActivity
adb logcat | grep Il2Cpp
```
Expected logs:
```
Il2CppDumper: Lib loaded
Il2CppDumper: Start dumping
Il2CppDumper: Extended dump enabled - generating IDA Pro files...
Il2CppDumper: dump done!
```

### 4) Pull outputs
```bash
adb pull /storage/emulated/0/Android/data/com.example.game/ ./dump_output/
```
You should see:
- com.example.game-dump.cs
- script.json
- il2cpp.h
- StringLiteral.json

### 5) IDA Pro
1. Open `libil2cpp.so` and let auto-analysis finish
2. File → Script file → choose `ida_load_script.py`
3. Select `script.json`
4. Functions are auto-named and annotated

---

## Non-Root (Lib Call) Method
1) Decompile APK with apktool
2) Copy `libil2cppdumper.so` into `lib/<abi>/`
3) In `MainActivity.smali` add:
```smali
const-string v0, "il2cppdumper"
invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```
4) Rebuild, sign, install

---

## Tips
- Increase `Sleep` in `app/src/main/jni/Includes/config.h` if dump is incomplete
- Wait until the main menu loads for better coverage
- Obfuscated method/class names cannot be restored to originals

## Useful Commands
```bash
adb logcat | grep -i il2cpp
adb shell cat /proc/$(pidof com.example.game)/maps | grep libil2cpp
```

## Docs
- README_EN.md — Overview and features
- IDA_PRO_GUIDE_EN.md — Detailed IDA workflow
- COMPILATION_GUIDE.md — Build instructions
- EXAMPLES.md — Practical examples
