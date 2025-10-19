# IDA Pro 使用指南 — Il2CppDumper 扩展版（中文）

## 目标
- 在 IDA 中打开 `libil2cpp.so`
- 通过 `script.json` 自动命名函数
- 导入 `il2cpp.h` 在 Local Types 中使用结构体

## 前置条件
- IDA Pro 7.0+
- 从 APK 中提取的 `libil2cpp.so`
- 设备生成的文件：
  - `script.json`
  - `il2cpp.h`
  - （可选）`dump.cs`、`StringLiteral.json`

## 步骤一：加载 libil2cpp.so
1. File → Open → 选择 `libil2cpp.so`
2. 处理器选择：
   - ARM：`ARM Little-endian`
   - ARM64：`AArch64`
3. 等待自动分析完成

## 步骤二：应用 `script.json` 命名
1. File → Script file → 选择 `ida_load_script.py`
2. 指定 `script.json`
3. 脚本会：
   - 在缺失处创建函数
   - 将函数命名为 `UnityEngine_GameObject_SetActive` 等
   - 将方法签名写入函数注释

提示：每处理 100 个方法会在 Output 窗口打印进度。

## 步骤三：导入 `il2cpp.h`（推荐）
1. File → Load file → Parse C header file
2. 选择 `il2cpp.h`
3. 打开 Local Types（Edit → Structs 或 View → Open subviews → Local Types）
4. 在反汇编处使用 `Y` 应用类型

## 地址与基址（Base Address）
- `script.json` 提供 `RVA` 与 `Address (VA)`
- 若 IDA 的 image base 不等于 `Address - RVA`，则需要计算：
```python
# 在 IDA Python console
import idaapi
base = idaapi.get_imagebase()
actual = base + rva  # 实际地址
```

## 快速定位
- 函数窗口搜索：Ctrl+F
- 交叉引用：`X`
- 跳转到地址：`G`
- 反编译：`F5`

## 常见问题
- 名称未应用：
  - 架构是否正确（ARM vs AArch64）
  - `RVA + image_base` 是否落在已映射段
- `script.json` 为空/不完整：
  - 在 `config.h` 增大 `Sleep` 并重新导出
  - 等待游戏完全载入（至少主菜单）
- 名称冲突：
  - 脚本自动附加地址后缀

## 批处理示例
```python
# IDA Python console 片段
import json, idaapi, idc
base = idaapi.get_imagebase()
with open('script.json','r', encoding='utf-8') as f:
    data = json.load(f)
for m in data['ScriptMethod']:
    ea = base + m['RVA']
    idc.add_func(ea)
    cname = (m['TypeSignature'] + '_' + m['Name']).replace('.', '_')
    idc.set_name(ea, cname, idc.SN_NOWARN | idc.SN_NOCHECK)
    idc.set_func_cmt(ea, m.get('Signature',''), 0)
print('[+] Done')
```

## 备注
- 被混淆的名称无法自动还原原始名称
- `il2cpp.h` 为基础字段类型，复杂场景请手动细化
- 大型项目批量命名需要较长时间

## 相关文档
- `README_ZH.md` — 概述
- `QUICKSTART_ZH.md` — 快速上手
- `COMPILATION_GUIDE.md` — 编译说明
- `EXAMPLES.md` — 示例
