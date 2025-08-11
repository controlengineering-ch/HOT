#!/bin/bash
# Skripti RadonSafe-projektin Gitin kuntoon laittamiseksi

set -e

echo "📄 Luodaan .gitignore..."
cat << 'EOF' > .gitignore
# ESP-IDF build artifacts
/build/
/sdkconfig
/sdkconfig.old
/.vscode/
/.idea/
/CMakeFiles/
cmake_install.cmake
CMakeCache.txt

# Python cache
*.pyc
__pycache__/

# OS junk
.DS_Store
*.swp
*.swo

# ESP-IDF tools cache
/.espressif/
/dist/
/.ccache/

# SquareLine exports
*.slp~
*.tmp

# Logs and temp files
*.log
*.bak
EOF

echo "➕ Lisätään kaikki uudet tiedostot..."
git add -A

echo "💾 Commit..."
git commit -m "Add new components, scripts, main_gui.c, and ESP-IDF .gitignore" || echo "⚠️ Ei mitään commitoitavaa"

echo "🚀 Pushataan muutokset remoteen..."
git push

echo "✅ Git-repo siivottu ja päivitetty!"

