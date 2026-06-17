# Tetoris

## Требования
- CMake 3.8+
- SFML 3.0+
- Компилятор с поддержкой C++17

## Сборка

### Windows
```bash
# Клонирование
git clone https://github.com/SleepYuze/Tetoris.git
cd Tetoris

# Сборка
В файле CMakeLists.txt добавьте путь к SFML
mkdir build
cd build
cmake ..
cmake --build . --config Release
