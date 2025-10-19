# Changelog - Il2CppDumper Extended

## [Extended 1.0] - 2025-10-19

### ✨ Новые возможности

#### Генерация файлов для IDA Pro
- **script.json** - JSON с адресами методов, сигнатурами и метаданными для автоматической разметки в IDA Pro
- **il2cpp.h** - C++ заголовочный файл с определениями структур классов
- **StringLiteral.json** - JSON со строковыми литералами из игры

#### Python-скрипт для IDA Pro
- **ida_load_script.py** - Автоматическая загрузка script.json в IDA Pro
  - Переименование функций в читаемые имена
  - Добавление комментариев с сигнатурами методов
  - Обработка коллизий имён
  - Прогресс-бар для отслеживания процесса

#### Конфигурация
- Новый флаг `EnableExtendedDump` в config.h для включения/отключения расширенных функций
- Возможность компилировать только базовую версию (dump.cs) или полную (все файлы)

### 📝 Новые файлы

#### Исходный код (C++)
- `app/src/main/jni/Il2Cpp/il2cpp_dump_extended.h` - Заголовочный файл расширенного дампера
- `app/src/main/jni/Il2Cpp/il2cpp_dump_extended.cpp` - Реализация генерации дополнительных файлов

#### Скрипты
- `ida_load_script.py` - Python-скрипт для IDA Pro

#### Документация
- `README_EXTENDED.md` - Полная документация расширенной версии
- `IDA_PRO_GUIDE.md` - Подробное руководство по использованию с IDA Pro
- `COMPILATION_GUIDE.md` - Руководство по компиляции проекта
- `EXAMPLES.md` - Примеры использования и анализа
- `CHANGELOG.md` - История изменений

### 🔧 Изменения в существующих файлах

#### app/src/main/jni/Il2Cpp/il2cpp_dump.cpp
- Изменено `static uint64_t il2cpp_base` на `uint64_t il2cpp_base` для экспорта переменной

#### app/src/main/jni/Il2Cpp/il2cpp_dump.h
- Добавлено `extern uint64_t il2cpp_base` для доступа из других модулей

#### app/src/main/jni/native-lib.cpp
- Добавлен include для `il2cpp_dump_extended.h`
- Добавлен вызов `il2cpp_dump_extended()` после основного дампа (если включен флаг)

#### app/src/main/jni/Includes/config.h
- Добавлен флаг `#define EnableExtendedDump` для управления расширенными функциями

#### app/src/main/jni/Android.mk
- Добавлен `Il2Cpp/il2cpp_dump_extended.cpp` в список компилируемых файлов

### 📊 Формат script.json

```json
{
  "ScriptMethod": [
    {
      "Address": 1879048192,
      "RVA": 123456,
      "Name": "MethodName",
      "Signature": "ReturnType MethodName(ParamType param)",
      "TypeSignature": "Namespace.ClassName"
    }
  ],
  "ScriptMetadata": [],
  "ScriptMetadataMethod": [],
  "Addresses": []
}
```

### 🎯 Основные улучшения

1. **Автоматизация анализа** - Больше не нужно вручную искать функции в IDA Pro
2. **Читаемые имена** - Функции автоматически получают имена вида `UnityEngine_GameObject_SetActive`
3. **Метаданные** - Комментарии с полными сигнатурами методов
4. **Структуры** - il2cpp.h позволяет типизировать данные в IDA Pro
5. **Совместимость** - Работает с оригинальным функционалом, можно отключить расширения

### 🔄 Обратная совместимость

- Все оригинальные функции сохранены
- dump.cs генерируется как и раньше
- Расширенные функции можно отключить через config.h
- Размер библиотеки увеличился незначительно (~50KB)

### 📈 Производительность

- Генерация script.json: ~1-3 секунды для средней игры (10K методов)
- Генерация il2cpp.h: ~2-5 секунд
- Общее увеличение времени дампа: ~3-8 секунд
- Загрузка в IDA Pro: ~2-10 минут (зависит от количества методов)

### 🐛 Известные ограничения

1. **StringLiteral.json** - Базовая реализация, требует доступа к global-metadata.dat для полной функциональности
2. **Обфусцированные имена** - Если игра использует обфускацию, имена будут нечитаемыми
3. **Большие игры** - Обработка >20K методов может занять 10-15 минут в IDA Pro
4. **Коллизии имён** - Перегруженные методы получают суффикс с адресом

### 🔮 Планы на будущее

- [ ] Полная реализация StringLiteral.json с парсингом global-metadata.dat
- [ ] Поддержка генерации DummyDll (фиктивных .NET сборок)
- [ ] Экспорт в формат Ghidra
- [ ] Автоматическое определение типов параметров
- [ ] Поддержка generic-типов в il2cpp.h
- [ ] Интеграция с Frida для динамического анализа

### 📦 Размер файлов

| Файл | Размер (типичный) |
|------|-------------------|
| dump.cs | 5-50 MB |
| script.json | 1-10 MB |
| il2cpp.h | 500KB-5MB |
| StringLiteral.json | 100KB-2MB |

### 🙏 Благодарности

Основано на работе:
- **Perfare** - Zygisk-Il2CppDumper
- **BrianGIG** - Auto-Il2cppDumper
- **VladKalinov** - Internal-Il2cppDumper-Android

### 📄 Лицензия

MIT License - см. [LICENSE](LICENSE)

---

## Как использовать эту версию

### Быстрый старт

1. **Компиляция:**
   ```bash
   ./gradlew assembleRelease
   ```

2. **Установка на устройство:**
   ```bash
   adb push libil2cppdumper.so /sdcard/
   # Далее следуйте инструкциям в README_EXTENDED.md
   ```

3. **Запуск игры и дамп:**
   - Запустите игру
   - Подождите 2-5 секунд
   - Файлы сохранятся в `/storage/emulated/0/Android/data/(package)/`

4. **Анализ в IDA Pro:**
   ```bash
   # Извлеките файлы
   adb pull /storage/emulated/0/Android/data/(package)/script.json
   
   # Откройте libil2cpp.so в IDA Pro
   # Запустите ida_load_script.py
   # Выберите script.json
   ```

### Отключение расширенных функций

Если нужен только dump.cs (как в оригинале):

```cpp
// В app/src/main/jni/Includes/config.h
// Закомментируйте эту строку:
// #define EnableExtendedDump
```

Перекомпилируйте проект.

---

**Версия:** Extended 1.0  
**Дата релиза:** 2025-10-19  
**Статус:** Stable
