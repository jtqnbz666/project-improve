// 提供worker启动、处理消息业务调用等接口
package interfaces

import "time"

/*
	消息管理抽象层
*/
type IMsgHandle interface {
	DoMsgHandler(request IRequest)                 // 马上以非阻塞方式处理消息
	AddRouter(msgID uint32, router IRouter)        // 为消息添加具体的处理逻辑
	AddInstantRouter(msgID uint32, router IRouter) // 为非工作协程处理的消息添加具体的处理逻辑
	StartWorkerPool()                              // 启动worker工作池
	SendMsgToTaskQueue(request IRequest)           // 将消息交给TaskQueue,由worker进行处理
	SetProfiling(func(startTime time.Time, request IRequest))
}
