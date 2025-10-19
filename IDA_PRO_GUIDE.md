# Руководство по использованию Il2CppDumper Extended для IDA Pro

Язык / Language / 语言:

- Русский: [IDA_PRO_GUIDE.md](IDA_PRO_GUIDE.md)
- English: [IDA_PRO_GUIDE_EN.md](IDA_PRO_GUIDE_EN.md)
- 中文: [IDA_PRO_GUIDE_ZH.md](IDA_PRO_GUIDE_ZH.md)

## Обзор

Расширенная версия Il2CppDumper теперь генерирует дополнительные файлы, специально предназначенные для анализа в IDA Pro:

### Генерируемые файлы

1. **dump.cs** - Декомпилированныйерал C# код (как в оригинале)
2. **script.json** - JSON с адресами методов, сигнатурами и метаданными
3. **il2cpp.h** - C++ заголовочный файл с определениями структур
4. **StringLiteral.json** - JSON со строковыми литами

## Установка и компиляция

### Требования
- Android NDK (r21 или новее)
- Gradle
- Java JDK 8+

### Компиляция

1. Клонируйте репозиторий:
```bash
git clone https://github.com/VladKalinov/Internal-Il2cppDumper-Android.git
cd Internal-Il2cppDumper-Android
```

2. Откройте проект в Android Studio или скомпилируйте через командную строку:
```bash
./gradlew assembleRelease
```

3. Скомпилированные библиотеки будут в:
```
app/build/intermediates/cmake/release/obj/
```

## Использование на Android

### Метод 1: Fake Lib (Root)

1. Скопируйте `libunity.so` или `libmain.so` в `/data/data/(package)/lib/`
2. Переименуйте оригинальную библиотеку в `librealunity.so` или `librealmain.so`
3. Запустите игру
4. Подождите 2-5 секунд после загрузки
5. Файлы будут сохранены в `/storage/emulated/0/Android/data/(package)/`

### Метод 2: Lib Call (Non-root)

1. Декомпилируйте APK через Apktool
2. Добавьте `libil2cppdumper.so` в папку lib
3. Вставьте вызов библиотеки в MainActivity (см. README.md)
4. Перекомпилируйте, подпишите и установите APK

## Работа с IDA Pro

### Шаг 1: Загрузка libil2cpp.so в IDA Pro

1. Откройте IDA Pro (версия 7.0+)
2. File -> Open -> Выберите `libil2cpp.so` из APK
3. Выберите процессор: ARM или ARM64 (в зависимости от архитектуры)
4. Дождитесь завершения автоанализа

### Шаг 2: Загрузка script.json

1. Скопируйте `script.json` с устройства на компьютер
2. В IDA Pro: File -> Script file -> Выберите `ida_load_script.py`
3. Выберите файл `script.json` когда появится диалог
4. Дождитесь завершения обработки (может занять несколько минут)

### Шаг 3: Использование il2cpp.h

1. Скопируйте `il2cpp.h` в рабочую директорию
2. В IDA Pro: File -> Load file -> Parse C header file
3. Выберите `il2cpp.h`
4. Теперь структуры доступны в Local Types (View -> Open subviews -> Local Types)

## Что делает ida_load_script.py

Скрипт автоматически:
- Читает `script.json` с адресами методов
- Вычисляет корректные адреса с учётом базового адреса модуля
- Создаёт функции в IDA, если их нет
- Переименовывает функции в читаемые имена (например: `UnityEngine_GameObject_SetActive`)
- Добавляет комментарии с сигнатурами методов
- Обрабатывает коллизии имён

## Пример использования

### До применения скрипта:
```
sub_12A4B0:
    PUSH {R4-R7,LR}
    ...
```

### После применения скрипта:
```
UnityEngine_GameObject_SetActive:
    ; Signature: void SetActive(Boolean)
    PUSH {R4-R7,LR}
    ...
```

## Структура script.json

```json
{
  "ScriptMethod": [
    {
      "Address": 1234567890,
      "RVA": 123456,
      "Name": "SetActive",
      "Signature": "void SetActive(Boolean value)",
      "TypeSignature": "UnityEngine.GameObject"
    }
  ]
}
```

### Поля:
- **Address** - Виртуальный адрес метода в памяти
- **RVA** - Относительный виртуальный адрес (смещение от базы)
- **Name** - Имя метода
- **Signature** - Полная сигнатура метода
- **TypeSignature** - Полное имя класса (namespace.class)

## Советы по анализу

### 1. Поиск конкретных методов
После загрузки script.json используйте:
- **Ctrl+F** в Functions window для поиска по имени
- **Alt+T** для поиска текста в комментариях

### 2. Навигация по коду
- Функции теперь имеют читаемые имена
- Используйте **X** (cross-references) для поиска вызовов
- **G** для перехода к адресу

### 3. Работа со структурами
- Импортируйте `il2cpp.h` для типизации данных
- Применяйте структуры к переменным через **Y** (Set type)

### 4. Декомпиляция
- **F5** для декомпиляции функции в псевдокод
- С правильными именами код становится читаемым

## Устранение проблем

### Проблема: Файлы не генерируются
**Решение:** 
- Увеличьте `Sleep` в `config.h` (по умолчанию 2 секунды)
- Проверьте logcat: `adb logcat | grep Il2Cpp`

### Проблема: script.json пустой
**Решение:**
- Убедитесь, что игра полностью загрузилась
- Проверьте, что IL2CPP инициализирован (в логах должно быть "il2cpp_base")

### Проблема: IDA не находит функции
**Решение:**
- Проверьте базовый адрес модуля (должен совпадать с Address - RVA)
- Убедитесь, что загружен правильный libil2cpp.so (архитектура ARM/ARM64)

### Проблема: Коллизии имён
**Решение:**
- Скрипт автоматически добавляет суффикс с адресом
- Можно вручную переименовать через **N** (Rename)

## Дополнительные возможности

### Экспорт данных из IDA
После анализа можно экспортировать:
```python
# В IDA Python console
import json
functions = {}
for func in idautils.Functions():
    name = idc.get_func_name(func)
    functions[hex(func)] = name

with open('exported_functions.json', 'w') as f:
    json.dump(functions, f, indent=2)
```

### Автоматизация анализа
Создайте IDC скрипт для пакетной обработки:
```c
#include <idc.idc>

static main() {
    auto script_path = "C:\\path\\to\\ida_load_script.py";
    RunPythonStatement("execfile('" + script_path + "')");
}
```

## Известные ограничения

1. **StringLiteral.json** - Базовая реализация, требует доступа к global-metadata.dat для полной функциональности
2. **Обфусцированные имена** - Если игра использует обфускацию, имена будут вида `Method_0x123456`
3. **Производительность** - Обработка больших игр (>10000 методов) может занять 5-10 минут

## Сравнение с оригинальным Il2CppDumper

| Функция             | Оригинал  | Extended |
|---------------------|-----------|
| dump.cs             | ✅        | ✅      |
| script.json         | ❌        | ✅      |
| il2cpp.h            | ❌        | ✅      |
| StringLiteral.json  | ❌        | ✅      |
| IDA скрипт          | ❌        | ✅      |
| Runtime дамп        | ✅        | ✅      |

## Лицензия

Основано на проекте [Auto-Il2cppDumper](https://github.com/Avatarchik/Auto-Il2cppDumper)

## Поддержка

Для вопросов и багов создавайте Issue в репозитории.
