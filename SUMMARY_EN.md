# Summary — Il2CppDumper Extended (English)

- Added extended IDA outputs and scripts
- Created multi-language docs (EN/ZH)
- Modified build and sources to integrate extended dump

Key files:
- `Il2Cpp/il2cpp_dump_extended.{h,cpp}`
- `ida_load_script.py`
- Docs: `README_EN.md`, `README_EXTENDED_EN.md`, `QUICKSTART_EN.md`, `IDA_PRO_GUIDE_EN.md`, `COMPILATION_GUIDE_EN.md`, `EXAMPLES_EN.md`, `CHANGELOG_EN.md`

How to start:
1) Build: `./gradlew assembleRelease`
2) Run on device and pull outputs
3) Load `script.json` in IDA with `ida_load_script.py`
