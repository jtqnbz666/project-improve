// 提供消息的基本方法
package interfaces

/*
	将请求的一个消息封装到message中，定义抽象层接口
*/
type IMessage interface {
	GetDataLen() int32    // 获取消息数据段长度
	GetMsgID() uint32      // 获取消息ID
	GetIdentifier() uint64 // 获取序列号
	GetData() []byte       // 获取消息内容

	SetMsgID(uint32)                 // 设计消息ID
	SetData([]byte)                  // 设计消息内容
	SetIdentifier(identifier uint64) // 设置序列号
	SetDataLen(int32)               // 设置消息数据段长度
}
