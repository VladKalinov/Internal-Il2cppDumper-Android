# Il2CppDumper Extended - Версия с поддержкой IDA Pro

Язык / Language / 语言:

- Русский: [README_EXTENDED.md](README_EXTENDED.md)
- English: [README_EXTENDED_EN.md](README_EXTENDED_EN.md)
- 中文: [README_EXTENDED_ZH.md](README_EXTENDED_ZH.md)

## 🎯 Что нового?

Эта расширенная версия Il2CppDumper генерирует дополнительные файлы для анализа в IDA Pro:

### 📦 Выходные файлы

| Файл | Описание | Назначение |
|------|----------|------------|
| **dump.cs** | C# декомпиляция классов и методов | Просмотр структуры кода, поиск методов |
| **script.json** | JSON с адресами методов и метаданными | Автоматическая разметка в IDA Pro |
| **il2cpp.h** | C++ заголовки со структурами | Типизация данных в IDA Pro |
| **StringLiteral.json** | Строковые литералы из игры | Поиск текста, локализация |

## 🚀 Быстрый старт

### 1. Компиляция

```bash
git clone https://github.com/VladKalinov/Internal-Il2cppDumper-Android.git
cd Internal-Il2cppDumper-Android
./gradlew assembleRelease
```

Библиотеки будут в: `app/build/intermediates/cmake/release/obj/`

### 2. Установка на Android

**Root метод:**
```bash
adb push libunity.so /data/data/com.game.package/lib/
adb shell
su
cd /data/data/com.game.package/lib/
mv libunity.so librealunity.so
mv /sdcard/libunity.so ./
chmod 755 libunity.so
```

**Non-root метод:** См. [IDA_PRO_GUIDE.md](IDA_PRO_GUIDE.md)

### 3. Запуск дампа

1. Запустите игру
2. Подождите 2-5 секунд после загрузки главного экрана
3. Проверьте logcat: `adb logcat | grep Il2Cpp`
4. Файлы сохранятся в `/storage/emulated/0/Android/data/com.game.package/`

### 4. Извлечение файлов

```bash
adb pull /storage/emulated/0/Android/data/com.game.package/com.game.package-dump.cs
adb pull /storage/emulated/0/Android/data/com.game.package/script.json
adb pull /storage/emulated/0/Android/data/com.game.package/il2cpp.h
adb pull /storage/emulated/0/Android/data/com.game.package/StringLiteral.json
```

### 5. Анализ в IDA Pro

1. Откройте `libil2cpp.so` в IDA Pro
2. Дождитесь автоанализа
3. File → Script file → Выберите `ida_load_script.py`
4. Выберите `script.json`
5. Подождите завершения (2-10 минут)

## 📋 Конфигурация

Файл: `app/src/main/jni/Includes/config.h`

```cpp
// Задержка перед дампом (увеличьте если дамп не работает)
#define Sleep 2

// Режим fake lib (требует root или модификацию APK)
#define UseFakeLib

// Включить расширенные функции для IDA Pro
#define EnableExtendedDump
```

### Настройки:

- **Sleep**: Задержка в секундах перед началом дампа
  - `2` - для большинства игр
  - `5-10` - для игр с защитой или медленной загрузкой
  
- **UseFakeLib**: Режим работы
  - Закомментировать для метода "Lib call"
  - Раскомментировать для метода "Fake lib"
  
- **EnableExtendedDump**: Генерация файлов для IDA Pro
  - Закомментировать для генерации только dump.cs
  - Раскомментировать для генерации всех файлов

## 🔧 Использование script.json в IDA Pro

### Автоматический метод (рекомендуется)

```python
# Запустите ida_load_script.py через File -> Script file
# Скрипт автоматически:
# - Переименует функции
# - Добавит комментарии с сигнатурами
# - Создаст функции если их нет
```

### Ручной метод

```python
import json
import idc

# Загрузка script.json
with open('script.json', 'r') as f:
    data = json.load(f)

# Применение имён
base = idaapi.get_imagebase()
for method in data['ScriptMethod']:
    addr = base + method['RVA']
    name = method['TypeSignature'].replace('.', '_') + '_' + method['Name']
    idc.set_name(addr, name, idc.SN_NOWARN)
    idc.set_func_cmt(addr, method['Signature'], 0)
```

## 📊 Структура script.json

```json
{
  "ScriptMethod": [
    {
      "Address": 1879048192,        // VA (виртуальный адрес)
      "RVA": 123456,                // Смещение от базы
      "Name": "Update",             // Имя метода
      "Signature": "void Update()", // Полная сигнатура
      "TypeSignature": "UnityEngine.MonoBehaviour" // Класс
    }
  ]
}
```

### Вычисление адреса:
```
Реальный адрес = Base Address + RVA
или
Реальный адрес = Address (если база не изменилась)
```

## 🎓 Примеры использования

### Пример 1: Поиск функции SetActive

**В dump.cs:**
```csharp
// Namespace: UnityEngine
public class GameObject {
    // RVA: 0x12A4B0 VA: 0x7012A4B0
    public void SetActive(bool value) { }
}
```

**В script.json:**
```json
{
  "Address": 1879048192,
  "RVA": 1221808,
  "Name": "SetActive",
  "Signature": "void SetActive(Boolean value)",
  "TypeSignature": "UnityEngine.GameObject"
}
```

**В IDA Pro после загрузки:**
```
UnityEngine_GameObject_SetActive:
    ; Signature: void SetActive(Boolean value)
    PUSH {R4-R7,LR}
    MOV R4, R0
    MOV R5, R1
    ...
```

### Пример 2: Анализ игровой логики

```python
# Поиск всех методов класса Player
import json

with open('script.json', 'r') as f:
    data = json.load(f)

player_methods = [m for m in data['ScriptMethod'] 
                  if 'Player' in m['TypeSignature']]

for method in player_methods:
    print(f"{method['Name']} at RVA: 0x{method['RVA']:X}")
```

### Пример 3: Экспорт функций с адресами

```python
# В IDA Python console
import json

functions = {}
for func in idautils.Functions():
    name = idc.get_func_name(func)
    if name.startswith('UnityEngine_') or name.startswith('System_'):
        functions[name] = {
            'address': hex(func),
            'size': idc.get_func_attr(func, idc.FUNCATTR_END) - func
        }

with open('exported_unity_functions.json', 'w') as f:
    json.dump(functions, f, indent=2)
```

## 🐛 Устранение проблем

### Проблема: Дамп не создаётся

**Проверка:**
```bash
adb logcat | grep -i il2cpp
```

**Решения:**
1. Увеличьте `Sleep` в config.h до 5-10
2. Проверьте, что libil2cpp.so загружен: `adb shell cat /proc/$(pidof com.game.package)/maps | grep libil2cpp`
3. Убедитесь, что игра использует IL2CPP (не Mono)

### Проблема: script.json пустой или неполный

**Причины:**
- Игра не полностью инициализировала IL2CPP
- Слишком короткая задержка Sleep
- Игра использует ленивую загрузку классов

**Решение:**
```cpp
// В config.h увеличьте Sleep
#define Sleep 10

// Или дождитесь загрузки уровня перед дампом
```

### Проблема: IDA не находит функции по адресам

**Проверка базового адреса:**
```python
# В IDA Python console
import idaapi
print(f"Base: 0x{idaapi.get_imagebase():X}")

# Сравните с первым методом в script.json
# Base должен быть равен Address - RVA
```

**Решение:**
```python
# Если база отличается, пересчитайте адреса
correct_base = 0x70000000  # Ваш базовый адрес
for method in data['ScriptMethod']:
    correct_addr = correct_base + method['RVA']
    # Используйте correct_addr вместо method['Address']
```

### Проблема: Коллизии имён функций

**Автоматическое решение:**
Скрипт `ida_load_script.py` автоматически добавляет суффикс с адресом при коллизии.

**Ручное решение:**
```python
# Добавьте уникальный префикс
name = f"{method['TypeSignature']}_{method['Name']}_{hex(method['RVA'])}"
idc.set_name(addr, name, idc.SN_NOWARN | idc.SN_NOCHECK)
```

## 📈 Производительность

| Размер игры | Методов | Время дампа | Время IDA |
|-------------|---------|-------------|-----------|
| Маленькая   | < 5K    | 1-2 сек     | 1-2 мин   |
| Средняя     | 5-20K   | 2-5 сек     | 2-5 мин   |
| Большая     | > 20K   | 5-10 сек    | 5-15 мин  |

## 🔒 Обход защиты

### Обфускация имён
Если имена обфусцированы (например, `Method_0x123456`), дамп всё равно работает, но имена будут нечитаемыми.

**Решение:**
- Найдите старую версию игры без обфускации
- Используйте динамический анализ (Frida, GDB)
- Анализируйте логику по адресам и cross-references

### Шифрование метаданных
Если global-metadata.dat зашифрован, дамп может быть неполным.

**Решение:**
- Runtime дамп обходит шифрование (данные расшифрованы в памяти)
- Увеличьте Sleep, чтобы дать время на расшифровку

### Античит
Некоторые игры детектируют дамп по:
- Загрузке подозрительных библиотек
- Проверке целостности APK
- Мониторингу файловой системы

**Решение:**
- Уменьшите Sleep для быстрого дампа
- Используйте виртуальное окружение (VMOS, VirtualXposed)
- Патчите античит через Frida

## 📚 Дополнительные ресурсы

### Документация
- [IDA_PRO_GUIDE.md](IDA_PRO_GUIDE.md) - Подробное руководство по IDA Pro
- [Оригинальный README](README.md) - Базовая документация

### Инструменты
- [Il2CppInspector](https://github.com/djkaty/Il2CppInspector) - Альтернативный дампер
- [Frida-Il2Cpp-Bridge](https://github.com/vfsfitvnm/frida-il2cpp-bridge) - Динамический анализ
- [IDA Pro](https://hex-rays.com/ida-pro/) - Дизассемблер

### Обучение
- [IL2CPP Internals](https://blog.unity.com/technology/il2cpp-internals) - Как работает IL2CPP
- [IDA Python](https://hex-rays.com/products/ida/support/idapython_docs/) - Автоматизация IDA
- [ARM Assembly](https://azeria-labs.com/writing-arm-assembly-part-1/) - Основы ARM

## 🤝 Вклад в проект

Основано на:
- [Internal-Il2cppDumper-Android](https://github.com/VladKalinov/Internal-Il2cppDumper-Android) by VladKalinov
- [Auto-Il2cppDumper](https://github.com/BryanGIG/Auto-Il2cppDumper) by BrianGIG
- [Zygisk-Il2CppDumper](https://github.com/Perfare/Zygisk-Il2CppDumper) by Perfare

## 📄 Лицензия

MIT License - см. [LICENSE](LICENSE)

## ⚠️ Дисклеймер

Этот инструмент предназначен только для образовательных целей и исследования безопасности. Использование для взлома, читерства или нарушения условий использования игр запрещено. Автор не несёт ответственности за неправомерное использование.

---

**Версия:** Extended 1.0  
**Дата:** 2025-10-19  
**Автор расширений:** Il2CppDumper Extended Team
