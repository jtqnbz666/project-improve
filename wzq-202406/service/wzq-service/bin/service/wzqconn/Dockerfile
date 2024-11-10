# 使用 CentOS 作为基础镜像
FROM centos:8 

# 创建目录
RUN mkdir -p /home/ConnServer

# 将文件复制到容器中
COPY ./ConnServer /home/ConnServer

# 添加 /home/ConnServer 到 ld.so.conf 并更新缓存
RUN echo "/home/ConnServer" >> /etc/ld.so.conf && ldconfig

# 设置工作目录
WORKDIR /home/ConnServer

# 确保可执行文件具有执行权限
RUN chmod +x wzqconn

# 设置容器的默认命令
CMD ["./wzqconn"]