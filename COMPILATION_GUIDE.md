# Руководство по компиляции Il2CppDumper Extended

## Требования

### Обязательные
- **Android NDK** r21 или новее
- **Gradle** 7.0+
- **Java JDK** 8 или 11
- **Git**

### Опциональные
- **Android Studio** (для удобной разработки)
- **CMake** 3.10+ (если используется CMake вместо ndk-build)

## Установка зависимостей

### Windows

```powershell
# Установка через Chocolatey
choco install androidstudio
choco install gradle
choco install openjdk11

# Или скачайте вручную:
# Android Studio: https://developer.android.com/studio
# Gradle: https://gradle.org/install/
# JDK: https://adoptium.net/
```

### Linux

```bash
# Ubuntu/Debian
sudo apt update
sudo apt install -y openjdk-11-jdk gradle git

# Установка Android SDK/NDK через sdkmanager
wget https://dl.google.com/android/repository/commandlinetools-linux-9477386_latest.zip
unzip commandlinetools-linux-9477386_latest.zip -d ~/android-sdk
~/android-sdk/cmdline-tools/bin/sdkmanager --sdk_root=~/android-sdk "ndk;21.4.7075529"
```

### macOS

```bash
# Через Homebrew
brew install openjdk@11
brew install gradle
brew install --cask android-studio
```

## Компиляция

### Метод 1: Через Gradle (рекомендуется)

```bash
# Клонирование репозитория
git clone https://github.com/VladKalinov/Internal-Il2cppDumper-Android.git
cd Internal-Il2cppDumper-Android

# Настройка переменных окружения (если нужно)
export ANDROID_HOME=~/Android/Sdk
export ANDROID_NDK_HOME=$ANDROID_HOME/ndk/21.4.7075529

# Компиляция для всех архитектур
./gradlew assembleRelease

# Или только для конкретной архитектуры
./gradlew assembleRelease -Pandroid.injected.build.abi=arm64-v8a
```

### Метод 2: Через Android Studio

1. Откройте Android Studio
2. File → Open → Выберите папку проекта
3. Дождитесь синхронизации Gradle
4. Build → Make Project (Ctrl+F9)
5. Библиотеки будут в `app/build/intermediates/cmake/release/obj/`

### Метод 3: Через ndk-build (прямая компиляция)

```bash
cd app/src/main/jni

# Компиляция для ARM64
$ANDROID_NDK_HOME/ndk-build APP_ABI=arm64-v8a

# Компиляция для ARM32
$ANDROID_NDK_HOME/ndk-build APP_ABI=armeabi-v7a

# Компиляция для всех архитектур
$ANDROID_NDK_HOME/ndk-build APP_ABI=all
```

## Выходные файлы

После успешной компиляции библиотеки будут находиться в:

```
app/build/intermediates/cmake/release/obj/
├── arm64-v8a/
│   └── libil2cppdumper.so
├── armeabi-v7a/
│   └── libil2cppdumper.so
├── x86/
│   └── libil2cppdumper.so
└── x86_64/
    └── libil2cppdumper.so
```

Или при использовании ndk-build:

```
app/src/main/jni/libs/
├── arm64-v8a/
│   └── libil2cppdumper.so
└── armeabi-v7a/
    └── libil2cppdumper.so
```

## Конфигурация перед компиляцией

### Файл: `app/src/main/jni/Includes/config.h`

```cpp
// Задержка перед дампом (секунды)
#define Sleep 2

// Режим fake lib (закомментируйте для lib call метода)
#define UseFakeLib

// Расширенные функции для IDA Pro
#define EnableExtendedDump
```

### Выбор режима компиляции

**Для Fake Lib (требует root):**
```cpp
#define UseFakeLib
```

**Для Lib Call (non-root):**
```cpp
// #define UseFakeLib  // Закомментировано
```

**Только dump.cs (без IDA файлов):**
```cpp
// #define EnableExtendedDump  // Закомментировано
```

## Устранение проблем компиляции

### Ошибка: NDK not found

```bash
# Установите NDK через sdkmanager
$ANDROID_HOME/cmdline-tools/latest/bin/sdkmanager "ndk;21.4.7075529"

# Или укажите путь в local.properties
echo "ndk.dir=/path/to/ndk" >> local.properties
```

### Ошибка: Gradle sync failed

```bash
# Очистка кеша Gradle
./gradlew clean
rm -rf .gradle
./gradlew assembleRelease
```

### Ошибка: C++ compilation failed

```bash
# Проверьте версию NDK
$ANDROID_NDK_HOME/ndk-build --version

# Убедитесь, что используется NDK r21+
# Старые версии могут не поддерживать C++17
```

### Ошибка: undefined reference to 'il2cpp_base'

Убедитесь, что в `il2cpp_dump.cpp` переменная объявлена без `static`:

```cpp
// Правильно:
uint64_t il2cpp_base = 0;

// Неправильно:
static uint64_t il2cpp_base = 0;
```

### Ошибка: Cannot find il2cpp_dump_extended.cpp

Проверьте `Android.mk`:

```makefile
LOCAL_SRC_FILES += native-lib.cpp \
                   Il2Cpp/il2cpp_dump.cpp \
                   Il2Cpp/il2cpp_dump_extended.cpp
```

## Оптимизация размера библиотеки

### Включение оптимизации

В `Android.mk`:

```makefile
LOCAL_CFLAGS += -Os -ffunction-sections -fdata-sections
LOCAL_LDFLAGS += -Wl,--gc-sections,--strip-all
```

### Отключение отладочной информации

```makefile
LOCAL_CPPFLAGS += -s -DNDEBUG
```

### Результат:
- **Без оптимизации:** ~500KB
- **С оптимизацией:** ~200KB
- **С UPX сжатием:** ~100KB (может вызвать проблемы с некоторыми играми)

## Тестирование после компиляции

### 1. Проверка библиотеки

```bash
# Проверка архитектуры
file libil2cppdumper.so

# Проверка экспортируемых символов
nm -D libil2cppdumper.so | grep JNI_OnLoad

# Проверка зависимостей
readelf -d libil2cppdumper.so
```

### 2. Тестовая установка

```bash
# Создайте тестовое приложение
adb install test_app.apk

# Замените библиотеку
adb push libil2cppdumper.so /data/local/tmp/
adb shell su -c "cp /data/local/tmp/libil2cppdumper.so /data/data/com.test.app/lib/"

# Запустите и проверьте logcat
adb logcat | grep Il2Cpp
```

### 3. Проверка генерации файлов

```bash
# После запуска игры проверьте файлы
adb shell ls -la /storage/emulated/0/Android/data/com.game.package/

# Должны быть:
# - com.game.package-dump.cs
# - script.json
# - il2cpp.h
# - StringLiteral.json
```

## Сборка для разных целей

### Debug сборка (с логами)

```bash
./gradlew assembleDebug
```

Логи будут более подробными, размер библиотеки больше.

### Release сборка (оптимизированная)

```bash
./gradlew assembleRelease
```

Оптимизирована по размеру и скорости, минимум логов.

### Профилирование сборка

```bash
./gradlew assembleRelease -Pandroid.enableProfileJson=true
```

Для анализа производительности компиляции.

## Кросс-компиляция

### Для эмуляторов (x86/x86_64)

```bash
./gradlew assembleRelease -Pandroid.injected.build.abi=x86_64
```

### Для реальных устройств (ARM)

```bash
# ARM64 (современные устройства)
./gradlew assembleRelease -Pandroid.injected.build.abi=arm64-v8a

# ARM32 (старые устройства)
./gradlew assembleRelease -Pandroid.injected.build.abi=armeabi-v7a
```

## Автоматизация сборки

### Скрипт для автоматической сборки

```bash
#!/bin/bash
# build_all.sh

echo "Building Il2CppDumper Extended..."

# Очистка
./gradlew clean

# Сборка для всех архитектур
for ABI in armeabi-v7a arm64-v8a x86 x86_64; do
    echo "Building for $ABI..."
    ./gradlew assembleRelease -Pandroid.injected.build.abi=$ABI
    
    # Копирование в output
    mkdir -p output/$ABI
    cp app/build/intermediates/cmake/release/obj/$ABI/libil2cppdumper.so output/$ABI/
done

echo "Build completed! Files in output/"
```

### GitHub Actions CI/CD

```yaml
# .github/workflows/build.yml
name: Build Il2CppDumper

on: [push, pull_request]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v2
      
      - name: Set up JDK 11
        uses: actions/setup-java@v2
        with:
          java-version: '11'
          
      - name: Setup Android SDK
        uses: android-actions/setup-android@v2
        
      - name: Build with Gradle
        run: ./gradlew assembleRelease
        
      - name: Upload artifacts
        uses: actions/upload-artifact@v2
        with:
          name: libil2cppdumper
          path: app/build/intermediates/cmake/release/obj/
```

## FAQ

**Q: Какую версию NDK использовать?**  
A: Рекомендуется r21 или новее. Проект использует C++17, который требует современный NDK.

**Q: Можно ли компилировать на Windows?**  
A: Да, используйте Android Studio или WSL (Windows Subsystem for Linux).

**Q: Сколько времени занимает компиляция?**  
A: 1-5 минут в зависимости от мощности компьютера и количества архитектур.

**Q: Нужен ли root для компиляции?**  
A: Нет, root нужен только для установки на устройство (метод Fake Lib).

**Q: Можно ли использовать на iOS?**  
A: Нет, этот проект только для Android. Для iOS используйте другие инструменты.

---

**Успешной компиляции!** 🚀
