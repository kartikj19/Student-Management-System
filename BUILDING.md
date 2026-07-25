# Building the Student Management System

## Build Requirements

- **C++ Standard**: C++17 or later
- **Compiler**: GCC 7+, Clang 5+, MSVC 2017+
- **CMake**: Version 3.16 or later
- **SQLite3**: Bundled in-repo fallback if system development libraries are unavailable
- **Catch2**: For testing (optional)

## Linux/macOS

### Install Dependencies

**Ubuntu/Debian:**
```bash
sudo apt-get update
sudo apt-get install build-essential cmake sqlite3 libsqlite3-dev git
```

**macOS (with Homebrew):**
```bash
brew install cmake sqlite3
```

**Fedora/RHEL:**
```bash
sudo dnf install cmake sqlite-devel gcc-c++
```

### Build Steps

```bash
# Clone the repository
git clone https://github.com/kartikj19/Student-Management-System.git
cd Student-Management-System

# Create build directory
mkdir build && cd build

# Generate build files
cmake ..

# Build the project
make

# (Optional) Install to system
sudo make install
```

### Run

```bash
# From build directory
./student_management_system

# Or if installed
student_management_system
```

## Windows (Visual Studio)

### Install Dependencies

1. Install **Visual Studio 2019** or later (Community Edition is free)
2. Install **CMake** from https://cmake.org/download/
3. SQLite3 installation is optional (the build uses bundled SQLite source if not found)

### Build Steps

```bash
# Clone the repository
git clone https://github.com/kartikj19/Student-Management-System.git
cd Student-Management-System

# Create build directory
mkdir build && cd build

# Generate Visual Studio project files
cmake -G "Visual Studio 16 2019" -A x64 ..

# Build the project
cmake --build . --config Release
```

### Run

```bash
# From build directory
Release\student_management_system.exe
```

## Windows (MinGW)

### Install Dependencies

1. Install **MinGW** with GCC 7 or later
2. Install **CMake** from https://cmake.org/download/
3. SQLite3 installation is optional (the build uses bundled SQLite source if not found)

### Build Steps

```bash
# Clone the repository
git clone https://github.com/kartikj19/Student-Management-System.git
cd Student-Management-System

# Create build directory
mkdir build && cd build

# Generate build files
cmake -G "MinGW Makefiles" ..

# Build the project
mingw32-make
```

### Run

```bash
student_management_system.exe
```

## Build Options

You can customize the build with CMake options:

```bash
# Enable/disable testing
cmake -DENABLE_TESTING=ON ..

# Set installation prefix
cmake -DCMAKE_INSTALL_PREFIX=/usr/local ..

# Set build type
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake -DCMAKE_BUILD_TYPE=Release ..
```

## Running Tests

```bash
cd build

# Run all tests
ctest

# Run with verbose output
ctest --output-on-failure

# Run specific test
ctest -R test_student -V
```

## Troubleshooting

### CMake not found
- **Linux**: `sudo apt-get install cmake`
- **macOS**: `brew install cmake`
- **Windows**: Download from https://cmake.org/

### SQLite3 not found
- **Linux**: `sudo apt-get install libsqlite3-dev`
- **macOS**: `brew install sqlite3`
- **Windows**: No manual install required; CMake uses bundled SQLite source

### Compiler errors
- Ensure you're using C++17 or later
- Update your compiler to a newer version
- Check that all dependencies are installed

### Build errors on Windows
- Make sure Visual Studio is fully installed
- Run from Developer Command Prompt
- Ensure `third_party/sqlite/` exists (contains bundled SQLite source files)

## Clean Build

```bash
cd build
rm -rf *        # Linux/macOS
rmdir /s /q .   # Windows

cd ..
mkdir build
cd build
cmake ..
make  # or cmake --build . on Windows
```

## Building Documentation

To generate Doxygen documentation:

```bash
# Install Doxygen
sudo apt-get install doxygen  # Linux
brew install doxygen          # macOS

# Generate docs
doxygen Doxyfile
```

Documentation will be in `docs/html/index.html`

## Performance Build

For optimized performance:

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native" ..
make
```

## Debug Build

For debugging with GDB/LLDB:

```bash
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make

# Run with debugger
gdb ./student_management_system
```

---

**For more help**, check the main README.md or open an issue on GitHub.
