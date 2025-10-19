# 概览 — Il2CppDumper 扩展版（中文）

- 新增面向 IDA 的扩展输出与脚本
- 提供多语言文档（中/英）
- 修改构建与源码以集成扩展导出

关键文件：
- `Il2Cpp/il2cpp_dump_extended.{h,cpp}`
- `ida_load_script.py`
- 文档：`README_ZH.md`、`README_EXTENDED_ZH.md`、`QUICKSTART_ZH.md`、`IDA_PRO_GUIDE_ZH.md`、`COMPILATION_GUIDE_ZH.md`、`EXAMPLES_ZH.md`、`CHANGELOG_ZH.md`

快速开始：
1）编译：`./gradlew assembleRelease`
2）在设备上运行并拉取输出
3）在 IDA 中用 `ida_load_script.py` 加载 `script.json`
