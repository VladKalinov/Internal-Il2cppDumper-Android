# 🚀 Быстрый старт - Il2CppDumper Extended

## За 5 минут до первого дампа

### 1️⃣ Скачайте готовую библиотеку

Если не хотите компилировать сами, скачайте готовую библиотеку из [Releases](https://github.com/VladKalinov/Internal-Il2cppDumper-Android/releases).

### 2️⃣ Установите на устройство (Root метод)

```bash
# Загрузите библиотеку на устройство
adb push libunity.so /sdcard/

# Подключитесь к устройству
adb shell
su

# Перейдите в папку игры (замените com.example.game на пакет вашей игры)
cd /data/data/com.example.game/lib

# Переименуйте оригинальную библиотеку
mv libunity.so librealunity.so

# Скопируйте нашу библиотеку
cp /sdcard/libunity.so ./
chmod 755 libunity.so

# Выход
exit
exit
```

### 3️⃣ Запустите игру

```bash
# Запустите игру
adb shell am start -n com.example.game/.MainActivity

# Следите за логами
adb logcat | grep Il2Cpp
```

Вы увидите:
```
Il2CppDumper: Lib loaded
Il2CppDumper: Start dumping
Il2CppDumper: Extended dump enabled - generating IDA Pro files...
Il2CppDumper: dump done!
```

### 4️⃣ Извлеките файлы

```bash
# Извлеките все файлы
adb pull /storage/emulated/0/Android/data/com.example.game/ ./dump_output/

# Проверьте что получили
ls -lh ./dump_output/
```

Вы должны увидеть:
- ✅ `com.example.game-dump.cs` - C# код
- ✅ `script.json` - Данные для IDA Pro
- ✅ `il2cpp.h` - C++ заголовки
- ✅ `StringLiteral.json` - Строки

### 5️⃣ Откройте в IDA Pro

1. **Откройте libil2cpp.so:**
   - File → Open
   - Выберите `libil2cpp.so` из APK игры
   - Processor: ARM или ARM64
   - Дождитесь автоанализа

2. **Загрузите script.json:**
   - File → Script file
   - Выберите `ida_load_script.py`
   - Выберите `script.json`
   - Подождите 2-10 минут

3. **Готово!** Функции переименованы, можно анализировать.

---

## Non-Root метод (требует модификацию APK)

### 1️⃣ Декомпилируйте APK

```bash
# Установите apktool
# Windows: choco install apktool
# Linux: sudo apt install apktool

# Декомпилируйте
apktool d game.apk -o game_decompiled
```

### 2️⃣ Добавьте библиотеку

```bash
# Скопируйте libil2cppdumper.so в lib папку
cp libil2cppdumper.so game_decompiled/lib/arm64-v8a/
cp libil2cppdumper.so game_decompiled/lib/armeabi-v7a/
```

### 3️⃣ Модифицируйте MainActivity

Откройте `game_decompiled/smali/com/example/game/MainActivity.smali`

Найдите метод `onCreate` и добавьте после `.locals X`:

```smali
const-string v0, "il2cppdumper"
invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```

### 4️⃣ Перекомпилируйте и установите

```bash
# Перекомпилируйте
apktool b game_decompiled -o game_modded.apk

# Подпишите (используйте uber-apk-signer или zipalign + apksigner)
java -jar uber-apk-signer.jar -a game_modded.apk

# Установите
adb install game_modded-aligned-signed.apk
```

---

## Что делать дальше?

### 📖 Изучите dump.cs
```bash
# Откройте в VS Code или любом редакторе
code com.example.game-dump.cs

# Поищите интересные классы
grep -n "class Player" com.example.game-dump.cs
grep -n "Currency" com.example.game-dump.cs
```

### 🔍 Анализируйте в IDA Pro
- Используйте **Ctrl+F** для поиска функций
- **X** для просмотра cross-references
- **F5** для декомпиляции
- **Tab** для переключения между ассемблером и псевдокодом

### 🛠️ Создайте Frida скрипт
```javascript
// hook.js
Java.perform(function() {
    // Адрес из script.json
    var func_addr = ptr("0x70123456");
    
    Interceptor.attach(func_addr, {
        onEnter: function(args) {
            console.log("Function called!");
        }
    });
});

// Запуск: frida -U -f com.example.game -l hook.js
```

---

## Частые проблемы

### ❌ Дамп не создаётся
**Решение:** Увеличьте `Sleep` в `config.h` до 5-10 секунд и перекомпилируйте.

### ❌ script.json пустой
**Решение:** Подождите пока игра полностью загрузится (дойдите до главного меню).

### ❌ IDA не находит функции
**Решение:** Проверьте базовый адрес в IDA (должен совпадать с Address - RVA из script.json).

### ❌ Игра крашится
**Решение:** 
1. Проверьте архитектуру (ARM32 vs ARM64)
2. Убедитесь что библиотека правильно переименована
3. Проверьте logcat для ошибок

---

## Полезные ссылки

- 📚 [Полная документация](README_EXTENDED.md)
- 🎓 [Руководство по IDA Pro](IDA_PRO_GUIDE.md)
- 🔧 [Руководство по компиляции](COMPILATION_GUIDE.md)
- 💡 [Примеры использования](EXAMPLES.md)

---

## Поддержка

Если что-то не работает:
1. Проверьте logcat: `adb logcat | grep -i il2cpp`
2. Убедитесь что игра использует IL2CPP (не Mono)
3. Попробуйте другую версию игры
4. Создайте Issue в репозитории

**Удачного анализа!** 🎯
