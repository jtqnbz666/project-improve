# 更新最新可执行文件
cp -r ./wzqconn/wzqconn /home/duolepublic/lzt/wzqconn/ConnServer
cp -r ./wzqgame/wzqgame /home/duolepublic/lzt/wzqgame/GameServer
cp -r ./wzqauth/wzqauth /home/duolepublic/lzt/wzqauth/AuthServer
cp -r ./wzqroom/wzqroom /home/duolepublic/lzt/wzqroom/RoomServer


cd /home/duolepublic/lzt/
bash ./rebuild.sh

