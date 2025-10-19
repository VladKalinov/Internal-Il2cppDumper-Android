# 更新日志 — Il2CppDumper 扩展版（中文）

## [Extended 1.0] — 2025-10-19

### 新增
- 面向 IDA 的扩展输出：`script.json`、`il2cpp.h`、`StringLiteral.json`
- 新增 IDA Python 加载脚本：`ida_load_script.py`
- 在 `config.h` 中加入 `EnableExtendedDump` 开关
- 文档集（中/英）：快速上手、IDA 指南、编译、示例、概览

### 变更
- 导出 `il2cpp_base`（去除 `static`），并在头文件中 `extern` 声明
- `native-lib.cpp` 在 `EnableExtendedDump` 条件下调用 `il2cpp_dump_extended()`
- 在 `Android.mk` 中加入 `il2cpp_dump_extended.cpp`

### 已知限制
- `StringLiteral.json` 为基础实现，未做完整元数据解析
- 混淆游戏仍保留混淆名称
- 大型项目（>20k 方法）在 IDA 中处理时间较长

### 规划
- 完整字符串提取
- 生成 DummyDll（.NET 假程序集）
- Ghidra 导出器
- 改进头文件中的泛型支持
- 集成 Frida 的辅助工具
