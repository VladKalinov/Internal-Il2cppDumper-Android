# Il2CppDumper Extended — IDA Pro Ready (English)

## Overview
This project extends the original Internal-Il2cppDumper-Android to generate extra artifacts tailored for reverse engineering in IDA Pro.

Generated outputs (on device under `/storage/emulated/0/Android/data/<package>/`):
- dump.cs — C# signatures of classes and methods (original behavior)
- script.json — Method names, RVA/VA addresses, and signatures for IDA scripting
- il2cpp.h — C/C++ header with class/field layouts (basic mapping)
- StringLiteral.json — String literals (baseline; full metadata parsing not implemented yet)

Primary goal: streamline IDA Pro analysis by auto-naming functions and enriching context.

## Features
- Runtime dump (works with encrypted/obfuscated metadata at rest)
- Optional Fake Lib mode (root/non-root flows)
- IDA Python script `ida_load_script.py` to auto-apply names/comments
- Simple type mapping for il2cpp.h (int/float/bool/etc.)
- Configurable via `app/src/main/jni/Includes/config.h`

## How It Works
1) A native library is injected/loaded into the game process.
2) After IL2CPP is fully initialized, dumper inspects assemblies and classes via IL2CPP API.
3) It produces `dump.cs` and, if `EnableExtendedDump` is defined, `script.json`, `il2cpp.h`, `StringLiteral.json`.

## Build
- Android NDK r21+
- Gradle 7+
- JDK 8/11

Build via Gradle:
```bash
./gradlew assembleRelease
```
Artifacts: `app/build/intermediates/cmake/release/obj/<abi>/libil2cppdumper.so`

## Install & Run (Root Fake Lib example)
- Replace game `libunity.so` (or `libmain.so`) with our loader and rename the original to `librealunity.so`/`librealmain.so`.
- Launch the game and wait a few seconds (adjust `Sleep` in `config.h`).
- Check logs via `adb logcat | grep Il2Cpp`.
- Pull outputs from `/storage/emulated/0/Android/data/<package>/`.

Non-root Lib Call method is supported (modify APK; see original README and IDA guide).

## IDA Pro Workflow
- Open `libil2cpp.so` in IDA, let analysis finish.
- Run `ida_load_script.py` (File → Script file), point to `script.json`.
- Functions get named like `UnityEngine_GameObject_SetActive` and annotated with signatures.
- Optionally import `il2cpp.h` via “Parse C header file” to use structures in local types.

## Configuration (`Includes/config.h`)
```cpp
#define Sleep 2           // Wait for IL2CPP to fully initialize
#define UseFakeLib        // Fake lib mode (comment out for Lib Call mode)
#define EnableExtendedDump // Enable script.json/il2cpp.h/StringLiteral.json
```

## File Format: script.json (excerpt)
```json
{
  "ScriptMethod": [
    {
      "Address": 1879048192,
      "RVA": 123456,
      "Name": "SetActive",
      "Signature": "void SetActive(Boolean value)",
      "TypeSignature": "UnityEngine.GameObject"
    }
  ]
}
```
Address = VA, RVA = VA - image_base. In IDA, real address = `image_base + RVA`.

## Limitations
- StringLiteral.json is a minimal stub (full parsing requires metadata layout parsing).
- Obfuscated names remain obfuscated (runtime has no original symbols).
- Very large games (20k+ methods) take longer to apply in IDA.

## Docs
- QUICKSTART_EN.md — 5-minute setup
- IDA_PRO_GUIDE_EN.md — Detailed IDA workflow
- COMPILATION_GUIDE.md — Build instructions (generic)
- EXAMPLES.md — Sample analyses and automation

## Credits
- Based on works by Perfare, BryanGIG, and VladKalinov.

## License
MIT (see LICENSE).
