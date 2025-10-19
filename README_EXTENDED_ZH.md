# Il2CppDumper 扩展版 — 详细说明（中文）

本文件为 `README_EXTENDED.md` 的中文版本。

## 内容
- 面向 IDA 的扩展输出：`script.json`、`il2cpp.h`、`StringLiteral.json`，以及原始的 `dump.cs`
- 提供 `ida_load_script.py` 自动命名与注释
- 完整文档：快速上手、IDA 指南、编译指南、示例

## 编译
```bash
./gradlew assembleRelease
```

## 配置
`app/src/main/jni/Includes/config.h`：
```cpp
#define Sleep 2
#define UseFakeLib
#define EnableExtendedDump
```

## 在 IDA 中使用
- 打开 `libil2cpp.so`，运行 `ida_load_script.py`，选择 `script.json`
- 可通过 "Parse C header file" 导入 `il2cpp.h`

## 备注
- 混淆名称无法还原原始命名
- `StringLiteral.json` 为基础实现

更多细节参见 `README_ZH.md`、`IDA_PRO_GUIDE_ZH.md`、`QUICKSTART_ZH.md`、`COMPILATION_GUIDE_ZH.md` 与 `EXAMPLES_ZH.md`。
