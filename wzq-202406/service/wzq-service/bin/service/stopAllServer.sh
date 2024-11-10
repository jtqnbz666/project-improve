#!/bin/bash 
# 停容器
docker ps -a | grep -E 'wzqconn|wzqauth|wzqroom|wzqgame' | awk '{print $1}' | xargs docker stop 

ps aux | grep -E 'wzqconn|wzqauth|wzqroom|wzqgame' | awk '$11 ~ /^\.\/wzqconn$|^\.\/wzqauth$|^\.\/wzqroom$|^\.\/wzqgame$/' | awk '{print $2}' | xargs kill -9
