# Il2CppDumper Extended — Full Documentation (English)

Language / Язык / 语言:

- English: [README_EXTENDED_EN.md](README_EXTENDED_EN.md)
- Русский: [README_EXTENDED.md](README_EXTENDED.md)
- 中文: [README_EXTENDED_ZH.md](README_EXTENDED_ZH.md)

This is the English version of `README_EXTENDED.md`.

## What’s included
- Extended outputs for IDA Pro: `script.json`, `il2cpp.h`, `StringLiteral.json`, plus original `dump.cs`
- IDA loader script `ida_load_script.py` for auto-naming and comments
- Docs: Quickstart, IDA guide, Compilation, Examples

## Build
```bash
./gradlew assembleRelease
```

## Configure
`app/src/main/jni/Includes/config.h`:
```cpp
#define Sleep 2
#define UseFakeLib
#define EnableExtendedDump
```

## Use in IDA
- Open `libil2cpp.so`, run `ida_load_script.py`, select `script.json`
- Optionally import `il2cpp.h` via “Parse C header file”

## Notes
- Obfuscated names remain obfuscated
- `StringLiteral.json` is a baseline implementation

For details see `README_EN.md`, `IDA_PRO_GUIDE_EN.md`, `QUICKSTART_EN.md`, `COMPILATION_GUIDE_EN.md`, and `EXAMPLES_EN.md`.
