# 使用 CentOS 作为基础镜像
FROM centos:8 

# 创建目录
RUN mkdir -p /home/RoomServer

# 将文件复制到容器中
COPY ./RoomServer /home/RoomServer

# 添加 /home/RoomServer 到 ld.so.conf 并更新缓存
RUN echo "/home/RoomServer" >> /etc/ld.so.conf && ldconfig

# 设置工作目录
WORKDIR /home/RoomServer

# 确保可执行文件具有执行权限
RUN chmod +x wzqroom

# 设置容器的默认命令
CMD ["./wzqroom"]