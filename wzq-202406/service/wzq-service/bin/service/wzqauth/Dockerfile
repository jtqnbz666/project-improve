# 使用 CentOS 作为基础镜像
FROM centos:8 

# 创建目录
RUN mkdir -p /home/AuthServer

# 将文件复制到容器中
COPY ./AuthServer /home/AuthServer

# 添加 /home/AuthServer 到 ld.so.conf 并更新缓存
RUN echo "/home/AuthServer" >> /etc/ld.so.conf && ldconfig

# 设置工作目录
WORKDIR /home/AuthServer

# 确保可执行文件具有执行权限
RUN chmod +x wzqauth

# 设置容器的默认命令
CMD ["./wzqauth"]
