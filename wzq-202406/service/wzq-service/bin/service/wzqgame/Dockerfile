# 使用 CentOS 作为基础镜像
FROM centos:8 

# 创建目录
RUN mkdir -p /home/GameServer

# 将文件复制到容器中
COPY ./GameServer /home/GameServer

# 添加 /home/GameServer 到 ld.so.conf 并更新缓存
RUN echo "/home/GameServer" >> /etc/ld.so.conf && ldconfig

# 设置工作目录
WORKDIR /home/GameServer

# 确保可执行文件具有执行权限
RUN chmod +x wzqgame

# 设置容器的默认命令
CMD sh -c "./wzqgame && tail -f /dev/null"
