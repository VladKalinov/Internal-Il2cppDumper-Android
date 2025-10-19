# Changelog — Il2CppDumper Extended (English)

## [Extended 1.0] — 2025-10-19

### New
- Added extended outputs for IDA Pro: `script.json`, `il2cpp.h`, `StringLiteral.json`
- Added IDA Python loader script: `ida_load_script.py`
- Config flag `EnableExtendedDump` in `config.h`
- Documentation set (EN/ZH): Quickstart, IDA Guide, Compilation, Examples, Summary

### Changes
- Exported `il2cpp_base` (removed `static`) and declared `extern` in header
- Updated `native-lib.cpp` to call `il2cpp_dump_extended()` behind `EnableExtendedDump`
- Included `il2cpp_dump_extended.cpp` in `Android.mk`

### Known limitations
- `StringLiteral.json` is minimal; full metadata parsing not implemented
- Obfuscated games keep obfuscated names
- Large titles (20k+ methods) take longer in IDA

### Roadmap
- Full StringLiteral extraction
- DummyDll generation
- Ghidra exporter
- Better generic types in headers
- Frida integration helpers
