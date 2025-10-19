# Il2CppDumper 扩展版 — 支持 IDA Pro（中文）

语言 / Language / Язык:

- 中文: [README_ZH.md](README_ZH.md)
- English: [README_EN.md](README_EN.md)
- Русский: [README_EXTENDED.md](README_EXTENDED.md)

## 概述
本项目在原始 Internal-Il2cppDumper-Android 基础上扩展，新增适用于 IDA Pro 逆向分析的输出文件与脚本。

设备输出目录：`/storage/emulated/0/Android/data/<包名>/`
- dump.cs — C# 类与方法签名（原始功能）
- script.json — 方法名称、RVA/VA 地址与签名（供 IDA 脚本使用）
- il2cpp.h — C/C++ 结构体定义（基础类型映射）
- StringLiteral.json — 字符串字面量（基础版，未做完整元数据解析）

目标：大幅提升在 IDA Pro 中的自动化标注与分析效率。

## 功能特点
- 运行时导出（可绕过静态加密/混淆的 metadata）
- 支持 Fake Lib 模式（Root/非 Root）
- 提供 IDA Python 脚本 `ida_load_script.py` 自动命名与注释
- 基础 il2cpp.h 类型映射（int/float/bool 等）
- 通过 `app/src/main/jni/Includes/config.h` 配置

## 工作流程
1) 原生 SO 库被注入/加载到游戏进程。
2) 等待 IL2CPP 完全初始化后，通过 IL2CPP API 枚举程序集、类与方法。
3) 生成 `dump.cs`；若定义了 `EnableExtendedDump`，同时生成 `script.json`、`il2cpp.h`、`StringLiteral.json`。

## 构建
- Android NDK r21+
- Gradle 7+
- JDK 8/11

使用 Gradle 构建：
```bash
./gradlew assembleRelease
```
产物位置：`app/build/intermediates/cmake/release/obj/<abi>/libil2cppdumper.so`

## 安装与运行（Root Fake Lib 示例）
- 用我们的 loader 替换游戏的 `libunity.so`（或 `libmain.so`），并将原库重命名为 `librealunity.so`/`librealmain.so`。
- 启动游戏，等待数秒（`config.h` 中通过 `Sleep` 可调整）。
- 日志：`adb logcat | grep Il2Cpp`。
- 输出文件位于 `/storage/emulated/0/Android/data/<包名>/`。

非 Root 的 Lib Call 方法也支持（需修改 APK，详见原 README 与 IDA 指南）。

## IDA Pro 流程
- 在 IDA 中打开 `libil2cpp.so`，等待自动分析完成。
- 运行 `ida_load_script.py`（File → Script file），选择 `script.json`。
- 函数会被自动命名为 `UnityEngine_GameObject_SetActive` 等，并附加签名注释。
- 可通过 “Parse C header file” 导入 `il2cpp.h`，以在 Local Types 中使用结构体。

## 配置（`Includes/config.h`）
```cpp
#define Sleep 2             // 等待 IL2CPP 完全初始化
#define UseFakeLib          // Fake lib 模式（Lib Call 模式请注释）
#define EnableExtendedDump  // 生成 script.json/il2cpp.h/StringLiteral.json
```

## 文件格式：script.json（示例）
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
Address 为 VA（虚拟地址），RVA 为 `VA - image_base`。在 IDA 中实际地址为 `image_base + RVA`。

## 限制
- StringLiteral.json 为简化实现（完整功能需解析 metadata 布局）。
- 若游戏已进行名称混淆，运行时无法恢复原始名称。
- 特别大的项目（>20k 方法）在 IDA 中应用命名耗时更长。

## 文档
- QUICKSTART_ZH.md — 5 分钟快速上手
- IDA_PRO_GUIDE_ZH.md — IDA 详细使用流程
- COMPILATION_GUIDE.md — 编译说明（通用）
- EXAMPLES.md — 分析与自动化示例

## 致谢
- 致敬 Perfare、BryanGIG 与 VladKalinov 的相关工作。

## 许可
MIT（见 LICENSE）。
