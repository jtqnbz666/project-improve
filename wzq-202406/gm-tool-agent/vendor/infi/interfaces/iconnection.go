package interfaces

import (
	"context"
	"google.golang.org/protobuf/proto"
	"net"
)

// IConnection 定义连接接口
type IConnection interface {
	Start()                   // 启动连接，让当前连接开始工作
	Stop()                    // 停止连接，结束当前连接状态
	Context() context.Context // 返回ctx，用于用户自定义的go程获取连接退出状态

	GetTCPConnection() *net.TCPConn // 从当前连接获取原始的socket TCPConn
	GetConnID() uint64              // 获取当前连接ID
	SetConnID(value uint64)         // 设置连接ID
	GetWorkerID() uint              // 获取 WorkerID
	RemoteAddr() net.Addr           // 获取远程客户端地址信息

	SendBack(req IRequest, msg proto.Message) error             // 发送客户端请求的返回消息
	SendMsg(msgID uint32, identifier uint64, data []byte) error // 将Message数据发送数据给远程的TCP客户端

	SetProperty(key string, value interface{})   // 设置链接属性
	GetProperty(key string) (interface{}, error) // 获取链接属性
	RemoveProperty(key string)                   // 移除链接属性

	GetLastSendData() []byte
	Info() map[string]interface{}
}
