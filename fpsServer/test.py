# 用于测试协议头， !IHH  表示大端、32字节，16字节，16字节
import socket
import struct
import time

# 定义协议结构
class PduHeader:
    def __init__(self, length, command_id, seq_num):
        self.length = length
        self.command_id = command_id
        self.seq_num = seq_num

    def pack(self):
        return struct.pack('!IHH', self.length, self.command_id, self.seq_num)

# 创建 PDU 对象
pdu = PduHeader(length=20, command_id=1, seq_num=42)

# 数据包
data_packet = pdu.pack() + b"BodyDataHere"  # 将头部和身体拼接在一起

# 目标服务器地址
server_address = ('127.0.0.1', 1234)

# 创建 UDP 套接字
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

try:
    # 发送数据包
    sock.sendto(data_packet, server_address)

    time.sleep(1)  # 等待1秒后发送第二个数据包，如果需要的话

    sock.sendto(data_packet, server_address)

    print("Data sent!")
finally:
    # 关闭套接字
    sock.close()

