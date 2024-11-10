/usr/local/bin/protoc2.4 --cpp_out=./wzqClient --proto_path=. *.proto
/usr/local/bin/protoc2.5 --cpp_out=./wzqServer --proto_path=. *.proto
cd ./messageConfig
python3 generateConstants.py
cd ..

# 拷贝到客户端
cp ./wzqClient/* ./messageConfig/*.h ../client/wzq-client/code/Classes/Net/Socket/Protocol/

# 拷贝到客户端
cp ./wzqServer/* ./messageConfig/*.h ../service/wzq-service/service/wzqauth/wzqauth/proto/
cp ./wzqServer/* ./messageConfig/*.h ../service/wzq-service/service/wzqconn/wzqconn/proto/
cp ./wzqServer/* ./messageConfig/*.h ../service/wzq-service/service/wzqgame/wzqgame/proto/
cp ./wzqServer/* ./messageConfig/*.h ../service/wzq-service/service/wzqroom/wzqroom/proto/

# 删除生成文件
rm ./wzqClient/* ./wzqServer/* ./messageConfig/*.h