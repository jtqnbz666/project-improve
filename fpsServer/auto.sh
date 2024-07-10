#!/bin/bash

# 显式设置 PATH
export PATH="/opt/homebrew/bin:$PATH"

# 切换到 build 目录，使用绝对路径
cd "$(dirname "$0")/build" || { echo "Failed to change directory to build"; exit 1; }

# 判断是否需要清理并重新生成
if [ "$1" = "reset" ]; then
    rm -rf *
    /opt/homebrew/bin/cmake ..
    make || { echo "Build failed"; exit 1; }
else
    make || { echo "Build failed"; exit 1; }
fi

# 复制生成的可执行文件，使用绝对路径
cp MyProject .. || { echo "Failed to copy executable"; exit 1; }

echo "Build completed successfully."

