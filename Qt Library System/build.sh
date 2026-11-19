#!/bin/bash
# ============================================================
# build.sh — One-click build & run for macOS / Linux
# VS Code calls this script via tasks.json.
# You can also run it directly:  bash build.sh
# ============================================================
set -e   # stop immediately on any error

echo "================================================"
echo "  Library Management System — Build Script"
echo "================================================"
echo ""

# ── Step 1: Find qmake ──────────────────────────────────────
QMAKE=""
for candidate in \
    "/opt/anaconda3/bin/qmake"            \
    "/opt/homebrew/opt/qt@5/bin/qmake"   \
    "/opt/homebrew/opt/qt5/bin/qmake"    \
    "/opt/homebrew/opt/qt@6/bin/qmake"   \
    "/usr/local/opt/qt@5/bin/qmake"      \
    "/usr/local/opt/qt5/bin/qmake"       \
    "$(command -v qmake  2>/dev/null)"   \
    "$(command -v qmake6 2>/dev/null)"
do
    if [ -x "$candidate" ]; then
        QMAKE="$candidate"
        break
    fi
done

if [ -z "$QMAKE" ]; then
    echo "❌  ERROR: Qt not found on this machine."
    echo ""
    echo "   Install Qt with one of these commands:"
    echo "     brew install qt@5          (Mac — Homebrew)"
    echo "     sudo apt install qt5-default cmake g++  (Linux)"
    echo ""
    echo "   Then run:  bash build.sh"
    exit 1
fi

echo "✅  Found qmake : $QMAKE"
echo "    Qt version  : $($QMAKE --version | grep -o 'Qt version [0-9.]*')"
echo ""

# ── Step 2: Create build directory ──────────────────────────
mkdir -p build
cd build

# ── Step 3: Run qmake to generate the Makefile ──────────────
echo "⚙️   Running qmake..."
"$QMAKE" ../LibrarySystem.pro CONFIG+=sdk_no_version_check -spec macx-clang 2>&1 \
  || "$QMAKE" ../LibrarySystem.pro CONFIG+=sdk_no_version_check 2>&1

# ── Step 4 (macOS only): remove the missing AGL framework ───
#    Apple removed AGL in macOS 13+; Qt 5 still references it.
if [[ "$OSTYPE" == "darwin"* ]]; then
    sed -i '' 's/-framework AGL//g' Makefile
fi

# ── Step 5: Compile ─────────────────────────────────────────
CORES=$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)
echo "🔨  Compiling on $CORES cores..."
make -j"$CORES"

echo ""
echo "✅  Build successful!"
echo ""

# ── Step 6 (macOS only): bundle Qt libs and re-sign ─────────
cd ..
if [[ "$OSTYPE" == "darwin"* ]] && [ -d "build/LibrarySystem.app" ]; then
    # Find macdeployqt next to the qmake we used
    MACDEPLOYQT="$(dirname "$QMAKE")/macdeployqt"
    if [ -x "$MACDEPLOYQT" ]; then
        echo "📦  Bundling Qt libraries..."
        "$MACDEPLOYQT" build/LibrarySystem.app 2>&1
        echo "🔏  Signing app..."
        codesign --force --deep --sign - build/LibrarySystem.app
        echo ""
    fi
fi

# ── Step 7: Launch ──────────────────────────────────────────
if [ -d "build/LibrarySystem.app" ]; then
    echo "🚀  Launching LibrarySystem.app ..."
    open build/LibrarySystem.app
elif [ -f "build/LibrarySystem" ]; then
    echo "🚀  Launching LibrarySystem ..."
    ./build/LibrarySystem
else
    echo "⚠️   Could not find the compiled binary to launch."
fi
