# IDA Pro Guide — Il2CppDumper Extended (English)

## Goals
- Open `libil2cpp.so` in IDA
- Auto-name functions using `script.json`
- Import `il2cpp.h` to use structures in Local Types

## Prerequisites
- IDA Pro 7.0+
- `libil2cpp.so` extracted from the APK
- Files generated on device:
  - `script.json`
  - `il2cpp.h`
  - (optional) `dump.cs`, `StringLiteral.json`

## Step 1 — Load libil2cpp.so
1. File → Open → select `libil2cpp.so`
2. Choose processor:
   - ARM: `ARM Little-endian`
   - ARM64: `AArch64`
3. Let auto-analysis finish

## Step 2 — Apply names via script.json
1. File → Script file → choose `ida_load_script.py`
2. Select `script.json`
3. The script will:
   - Create functions (if missing)
   - Assign names like `UnityEngine_GameObject_SetActive`
   - Add method signatures as function comments

Tip: Progress is printed in the Output window every 100 methods.

## Step 3 — Import il2cpp.h (optional but recommended)
1. File → Load file → Parse C header file
2. Select `il2cpp.h`
3. Open Local Types (Edit → Structs or View → Open subviews → Local Types)
4. Apply types with `Y` on operands/stack variables

## Addressing and Base Address
- `script.json` supplies `RVA` and `Address (VA)`
- If your IDA image base differs from `Address - RVA`, use:
```python
# IDA Python console
import idaapi
base = idaapi.get_imagebase()
actual = base + rva  # Use this to resolve the final address
```

## Finding Things Quickly
- Functions window: Ctrl+F to search by name
- Cross references: `X`
- Jump to address: `G`
- Decompile: `F5`

## Troubleshooting
- Names not applied:
  - Ensure correct architecture (ARM vs AArch64)
  - Verify that RVA + image_base points into a mapped segment
- Empty/partial script.json:
  - Increase `Sleep` in `config.h` and re-dump
  - Wait until main menu or game is fully loaded
- Colliding names:
  - Script appends address suffix automatically

## Batch Automation Example
```python
# IDA Python console snippet
import json, idaapi, idc
base = idaapi.get_imagebase()
with open('script.json','r') as f:
    data = json.load(f)
for m in data['ScriptMethod']:
    ea = base + m['RVA']
    idc.add_func(ea)
    cname = (m['TypeSignature'] + '_' + m['Name']).replace('.', '_')
    idc.set_name(ea, cname, idc.SN_NOWARN | idc.SN_NOCHECK)
    idc.set_func_cmt(ea, m.get('Signature',''), 0)
print('[+] Done')
```

## Notes
- Obfuscated names cannot be deobfuscated without extra context
- `il2cpp.h` contains basic field types; refine manually when needed
- Large titles may take minutes to rename all functions

## See Also
- `README_EN.md` — overview
- `QUICKSTART_EN.md` — 5-minute setup
- `COMPILATION_GUIDE.md` — building the project
- `EXAMPLES.md` — analysis samples
