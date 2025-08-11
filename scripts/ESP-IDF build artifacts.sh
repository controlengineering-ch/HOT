echo "# ESP-IDF build artifacts
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
/ccache/

# SquareLine exports
*.slp~
*.tmp

# Logs and temp files
*.log
*.bak" > .gitignore

git add .gitignore
git commit -m "Add ESP-IDF .gitignore"
git push

