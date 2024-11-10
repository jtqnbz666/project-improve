#!/bin/bash -ilex 
# 编译；
# build 目录
if [ ! -d "build" ]
then
	mkdir -p build
fi

cd ./build
rm -rf *
cmake ..
make
cd ..
cd ./bin/service 
#./stopAllServer.sh
#./startAllServer.sh
echo "所有服务已全部重启!!!"
if [ "$1" == "docker" ]; then
	
	echo "docker模式"
    ./deploy.sh
fi



