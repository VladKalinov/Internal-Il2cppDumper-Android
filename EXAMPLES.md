# Примеры использования Il2CppDumper Extended

## 📖 Содержание

1. [Базовое использование](#базовое-использование)
2. [Анализ в IDA Pro](#анализ-в-ida-pro)
3. [Поиск игровой логики](#поиск-игровой-логики)
4. [Модификация игры](#модификация-игры)
5. [Автоматизация анализа](#автоматизация-анализа)

## Базовое использование

### Пример 1: Дамп простой игры

```bash
# 1. Установка библиотеки (root)
adb push libunity.so /sdcard/
adb shell su -c "cp /sdcard/libunity.so /data/data/com.example.game/lib/"
adb shell su -c "mv /data/data/com.example.game/lib/libunity.so /data/data/com.example.game/lib/librealunity.so"
adb shell su -c "cp /sdcard/libunity.so /data/data/com.example.game/lib/"

# 2. Запуск игры
adb shell am start -n com.example.game/.MainActivity

# 3. Мониторинг логов
adb logcat | grep Il2Cpp

# Вывод:
# Il2CppDumper: Lib loaded
# Il2CppDumper: Waiting in 2...
# Il2CppDumper: Start dumping
# Il2CppDumper: il2cpp_base: 0x70000000
# Il2CppDumper: dumping...
# Il2CppDumper: Extended dump enabled - generating IDA Pro files...
# Il2CppDumper: Generating script.json...
# Il2CppDumper: script.json saved to /storage/emulated/0/Android/data/com.example.game/script.json
# Il2CppDumper: dump done!

# 4. Извлечение файлов
adb pull /storage/emulated/0/Android/data/com.example.game/
```

### Пример 2: Анализ dump.cs

```bash
# Открыть dump.cs в текстовом редакторе
code com.example.game-dump.cs

# Поиск конкретного класса
grep -n "class Player" com.example.game-dump.cs

# Поиск метода с адресом
grep -A 5 "RVA: 0x12A4B0" com.example.game-dump.cs
```

**Результат в dump.cs:**
```csharp
// Namespace: GameLogic
public class Player : MonoBehaviour
{
    // Fields
    public int health; // 0x18
    public float speed; // 0x1C
    
    // RVA: 0x12A4B0 VA: 0x7012A4B0
    public void TakeDamage(int damage) { }
    
    // RVA: 0x12A500 VA: 0x70125500
    public void Heal(int amount) { }
}
```

## Анализ в IDA Pro

### Пример 3: Загрузка в IDA Pro

```python
# 1. Откройте libil2cpp.so в IDA Pro
# 2. Запустите скрипт через File -> Script file -> ida_load_script.py

# Скрипт выведет:
"""
============================================================
IL2CPP Script Loader for IDA Pro
============================================================
[*] Loading script.json from: C:\Users\...\script.json
[+] Successfully loaded script.json
[*] Found 15432 methods to process
[*] Starting to apply method names...
[*] Progress: 100 methods processed (98 success, 2 failed)
[*] Progress: 200 methods processed (195 success, 5 failed)
...
[+] Completed! 15320 methods renamed successfully, 112 failed
[+] Script execution completed!
============================================================
"""
```

### Пример 4: Навигация по коду в IDA

```python
# В IDA Python console

# Поиск функции по имени
addr = idc.get_name_ea_simple("GameLogic_Player_TakeDamage")
print(f"TakeDamage at: 0x{addr:X}")

# Переход к функции
idc.jumpto(addr)

# Получение декомпиляции
import ida_hexrays
cfunc = ida_hexrays.decompile(addr)
print(cfunc)

# Вывод:
"""
void __fastcall GameLogic_Player_TakeDamage(Player *this, int damage)
{
  this->health -= damage;
  if ( this->health <= 0 )
    GameLogic_Player_Die(this);
}
"""
```

### Пример 5: Поиск cross-references

```python
# Найти все места, где вызывается TakeDamage
import idautils

addr = idc.get_name_ea_simple("GameLogic_Player_TakeDamage")

print("Cross-references to TakeDamage:")
for xref in idautils.XrefsTo(addr):
    caller = idc.get_func_name(xref.frm)
    print(f"  Called from: {caller} at 0x{xref.frm:X}")

# Вывод:
"""
Cross-references to TakeDamage:
  Called from: GameLogic_Enemy_Attack at 0x70130000
  Called from: GameLogic_Trap_OnTrigger at 0x70135000
  Called from: GameLogic_Boss_SpecialAttack at 0x70140000
"""
```

## Поиск игровой логики

### Пример 6: Поиск функций валюты

```python
# Анализ script.json для поиска функций с деньгами
import json

with open('script.json', 'r') as f:
    data = json.load(f)

# Поиск методов связанных с валютой
currency_methods = []
keywords = ['coin', 'gold', 'money', 'currency', 'gem', 'diamond']

for method in data['ScriptMethod']:
    name_lower = method['Name'].lower()
    type_lower = method['TypeSignature'].lower()
    
    if any(kw in name_lower or kw in type_lower for kw in keywords):
        currency_methods.append({
            'name': method['Name'],
            'class': method['TypeSignature'],
            'rva': hex(method['RVA']),
            'signature': method['Signature']
        })

# Вывод результатов
print(f"Found {len(currency_methods)} currency-related methods:\n")
for m in currency_methods[:10]:  # Первые 10
    print(f"{m['class']}.{m['name']}")
    print(f"  RVA: {m['rva']}")
    print(f"  Signature: {m['signature']}\n")

# Результат:
"""
Found 47 currency-related methods:

GameLogic.CurrencyManager.AddCoins
  RVA: 0x150000
  Signature: void AddCoins(Int32 amount)

GameLogic.CurrencyManager.SpendCoins
  RVA: 0x150100
  Signature: Boolean SpendCoins(Int32 amount)

GameLogic.Shop.BuyWithGems
  RVA: 0x160000
  Signature: Boolean BuyWithGems(Int32 itemId, Int32 price)
"""
```

### Пример 7: Анализ античита

```python
# Поиск функций проверки целостности
import json

with open('script.json', 'r') as f:
    data = json.load(f)

anticheat_keywords = ['check', 'verify', 'validate', 'detect', 'cheat', 'hack']

for method in data['ScriptMethod']:
    name_lower = method['Name'].lower()
    
    if any(kw in name_lower for kw in anticheat_keywords):
        print(f"{method['TypeSignature']}.{method['Name']}")
        print(f"  RVA: 0x{method['RVA']:X}\n")

# Результат показывает потенциальные функции античита:
"""
Security.IntegrityChecker.VerifyAPK
  RVA: 0x200000

Security.MemoryProtection.DetectModification
  RVA: 0x201000

Security.AntiCheat.ValidateGameState
  RVA: 0x202000
"""
```

## Модификация игры

### Пример 8: Патчинг функции в IDA

```python
# В IDA Pro - патчинг функции SpendCoins для бесплатных покупок

# 1. Найти функцию
addr = idc.get_name_ea_simple("GameLogic_CurrencyManager_SpendCoins")

# 2. Посмотреть оригинальный код
"""
GameLogic_CurrencyManager_SpendCoins:
    CMP     R1, [R0,#coins_offset]  ; Сравнить amount с текущими монетами
    BGT     not_enough              ; Если больше - недостаточно
    SUB     [R0,#coins_offset], R1  ; Вычесть монеты
    MOV     R0, #1                  ; Вернуть true
    BX      LR
not_enough:
    MOV     R0, #0                  ; Вернуть false
    BX      LR
"""

# 3. Патч: всегда возвращать true без проверки
# Заменить CMP и BGT на NOP
idc.patch_byte(addr, 0x00)  # NOP
idc.patch_byte(addr+1, 0xBF)  # NOP
idc.patch_byte(addr+2, 0x00)  # NOP
idc.patch_byte(addr+3, 0xBF)  # NOP

# 4. Сохранить патч
# File -> Produce file -> Create DIF file
```

### Пример 9: Создание Frida скрипта из dump

```javascript
// frida_currency_hack.js
// Создан на основе данных из script.json

Java.perform(function() {
    // Адреса из script.json
    var AddCoins_addr = ptr("0x70150000");  // RVA + base
    var SpendCoins_addr = ptr("0x70150100");
    
    // Hook AddCoins
    Interceptor.attach(AddCoins_addr, {
        onEnter: function(args) {
            console.log("[AddCoins] Original amount: " + args[1]);
            args[1] = ptr(999999);  // Изменить на 999999
            console.log("[AddCoins] Modified amount: " + args[1]);
        }
    });
    
    // Hook SpendCoins - всегда возвращать true
    Interceptor.attach(SpendCoins_addr, {
        onLeave: function(retval) {
            console.log("[SpendCoins] Original return: " + retval);
            retval.replace(ptr(1));  // Всегда true
            console.log("[SpendCoins] Modified return: " + retval);
        }
    });
    
    console.log("[*] Currency hooks installed!");
});

// Запуск:
// frida -U -f com.example.game -l frida_currency_hack.js
```

## Автоматизация анализа

### Пример 10: Массовый анализ методов

```python
# analyze_all_methods.py
import json
import re

def analyze_script_json(json_path):
    with open(json_path, 'r') as f:
        data = json.load(f)
    
    stats = {
        'total_methods': len(data['ScriptMethod']),
        'namespaces': {},
        'method_types': {},
        'suspicious_methods': []
    }
    
    for method in data['ScriptMethod']:
        # Статистика по namespace
        ns = method['TypeSignature'].split('.')[0]
        stats['namespaces'][ns] = stats['namespaces'].get(ns, 0) + 1
        
        # Анализ сигнатуры
        sig = method['Signature']
        if 'Boolean' in sig:
            stats['method_types']['Boolean'] = stats['method_types'].get('Boolean', 0) + 1
        
        # Поиск подозрительных методов
        suspicious_keywords = ['decrypt', 'encode', 'obfuscate', 'check', 'verify']
        if any(kw in method['Name'].lower() for kw in suspicious_keywords):
            stats['suspicious_methods'].append({
                'name': f"{method['TypeSignature']}.{method['Name']}",
                'rva': hex(method['RVA'])
            })
    
    return stats

# Использование
stats = analyze_script_json('script.json')

print(f"Total methods: {stats['total_methods']}\n")
print("Top 5 namespaces:")
for ns, count in sorted(stats['namespaces'].items(), key=lambda x: x[1], reverse=True)[:5]:
    print(f"  {ns}: {count} methods")

print(f"\nSuspicious methods: {len(stats['suspicious_methods'])}")
for m in stats['suspicious_methods'][:10]:
    print(f"  {m['name']} at {m['rva']}")
```

### Пример 11: Генерация IDA скрипта для комментариев

```python
# generate_ida_comments.py
import json

def generate_ida_script(json_path, output_path):
    with open(json_path, 'r') as f:
        data = json.load(f)
    
    with open(output_path, 'w') as out:
        out.write("import idc\n\n")
        out.write("# Auto-generated IDA script for adding comments\n\n")
        out.write("base = idaapi.get_imagebase()\n\n")
        
        for method in data['ScriptMethod']:
            rva = method['RVA']
            comment = f"{method['TypeSignature']}.{method['Name']}\\n{method['Signature']}"
            
            out.write(f"# {method['Name']}\n")
            out.write(f"addr = base + 0x{rva:X}\n")
            out.write(f'idc.set_cmt(addr, "{comment}", 1)\n\n')
        
        out.write('print("[+] Comments added successfully!")\n')

# Использование
generate_ida_script('script.json', 'add_comments.py')
print("Generated add_comments.py - run it in IDA Pro")
```

### Пример 12: Сравнение двух версий игры

```python
# compare_versions.py
import json

def compare_dumps(old_json, new_json):
    with open(old_json, 'r') as f:
        old_data = json.load(f)
    with open(new_json, 'r') as f:
        new_data = json.load(f)
    
    old_methods = {m['Name']: m for m in old_data['ScriptMethod']}
    new_methods = {m['Name']: m for m in new_data['ScriptMethod']}
    
    # Новые методы
    added = set(new_methods.keys()) - set(old_methods.keys())
    # Удалённые методы
    removed = set(old_methods.keys()) - set(new_methods.keys())
    # Изменённые методы (разные RVA)
    changed = []
    for name in set(old_methods.keys()) & set(new_methods.keys()):
        if old_methods[name]['RVA'] != new_methods[name]['RVA']:
            changed.append({
                'name': name,
                'old_rva': hex(old_methods[name]['RVA']),
                'new_rva': hex(new_methods[name]['RVA'])
            })
    
    print(f"Added methods: {len(added)}")
    for name in list(added)[:10]:
        print(f"  + {name}")
    
    print(f"\nRemoved methods: {len(removed)}")
    for name in list(removed)[:10]:
        print(f"  - {name}")
    
    print(f"\nChanged methods: {len(changed)}")
    for m in changed[:10]:
        print(f"  ~ {m['name']}: {m['old_rva']} -> {m['new_rva']}")

# Использование
compare_dumps('v1.0_script.json', 'v1.1_script.json')
```

## Заключение

Эти примеры показывают основные сценарии использования Il2CppDumper Extended:

1. **Базовый дамп** - извлечение информации из игры
2. **Анализ в IDA Pro** - реверс-инжиниринг с автоматической разметкой
3. **Поиск логики** - нахождение интересных функций
4. **Модификация** - создание патчей и хуков
5. **Автоматизация** - массовый анализ и сравнение версий

Для более подробной информации см.:
- [README_EXTENDED.md](README_EXTENDED.md) - Полная документация
- [IDA_PRO_GUIDE.md](IDA_PRO_GUIDE.md) - Руководство по IDA Pro
- [COMPILATION_GUIDE.md](COMPILATION_GUIDE.md) - Компиляция проекта
