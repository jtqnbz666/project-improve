// 提供Client抽象层全部接口声明
package interfaces

import (
	"context"
	"time"
)

// 定义tcp客户端接口
type IClient interface {
	Dial(string, bool, time.Duration) error                                                  // 启动客户端
	Close()                                                                                  // 端开连接
	GetID() uint64                                                                           // 获取客户端ID
	SetID(uint64)                                                                            // 设置客户端ID
	AddRouter(msgID uint32, route func(IClient, IMessage))                                   // 路由功能：注册一个路由业务方法，当收到数据时根据类型交给对应的handler处理
	SetOnConnStart(func(IClient))                                                            // 设置该Client连接时Hook函数
	SetOnConnClose(func(IClient))                                                            // 设置该Client的连接断开时的Hook函数
	SetOnDataReceive(func(IMessage))                                                         // 设置该Client的收到数据时的Hook函数
	SendMsg(msgID uint32, identifier uint64, data []byte) error                              // client应答服务端
	SendRequest(ctx context.Context, msgID uint32, data []byte, callback func([]byte)) error // client请求
	AsyncSendRequest(ctx context.Context, msgID uint32, data []byte) ([]byte, error)         // client同步请求
}
