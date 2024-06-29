#!/bin/bash
 cd ./build || { echo "Failed to change directory to build"; exit 1; }
# 判断是否需要清理并重新生成
if [ "$1" = "reset" ]; then
    rm * -rf 
    cmake ..
    make || { echo "Build failed"; exit 1; }
else
    make || { echo "Build failed"; exit 1; }
fi

# 复制生成的可执行文件
cp MyProject .. || { echo "Failed to copy executable"; exit 1; }

echo "Build completed successfully."

