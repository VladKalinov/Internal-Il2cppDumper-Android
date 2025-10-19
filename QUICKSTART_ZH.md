# 🚀 快速上手 — Il2CppDumper 扩展版（中文）

## 5 分钟完成首次导出

### 1）获取库文件
- 使用 Gradle 编译：`./gradlew assembleRelease`
- 或使用预编译产物（如有提供）。

### 2）安装到设备（Root Fake Lib 方法）
```bash
adb push libunity.so /sdcard/
adb shell
su
cd /data/data/com.example.game/lib
mv libunity.so librealunity.so
cp /sdcard/libunity.so ./
chmod 755 libunity.so
exit
exit
```

### 3）启动游戏
```bash
adb shell am start -n com.example.game/.MainActivity
adb logcat | grep Il2Cpp
```
预期日志：
```
Il2CppDumper: Lib loaded
Il2CppDumper: Start dumping
Il2CppDumper: Extended dump enabled - generating IDA Pro files...
Il2CppDumper: dump done!
```

### 4）拉取输出文件
```bash
adb pull /storage/emulated/0/Android/data/com.example.game/ ./dump_output/
```
应包含：
- com.example.game-dump.cs
- script.json
- il2cpp.h
- StringLiteral.json

### 5）在 IDA Pro 中使用
1. 打开 `libil2cpp.so` 并等待自动分析完成
2. File → Script file → 选择 `ida_load_script.py`
3. 选择 `script.json`
4. 函数将被自动命名并附加签名注释

---

## 非 Root（Lib Call）方法
1）使用 apktool 反编译 APK  
2）将 `libil2cppdumper.so` 放入 `lib/<abi>/`  
3）在 `MainActivity.smali` 的 `onCreate` 加入：
```smali
const-string v0, "il2cppdumper"
invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```
4）重打包、签名并安装

---

## 提示
- 如果导出不完整，请在 `app/src/main/jni/Includes/config.h` 中增大 `Sleep`
- 等待游戏进入主菜单再导出，覆盖率更好
- 混淆过的类/方法名无法恢复原始名称

## 常用命令
```bash
adb logcat | grep -i il2cpp
adb shell cat /proc/$(pidof com.example.game)/maps | grep libil2cpp
```

## 文档
- README_ZH.md — 概述与特性
- IDA_PRO_GUIDE_ZH.md — 详细 IDA 流程
- COMPILATION_GUIDE.md — 编译说明
- EXAMPLES.md — 实战示例
