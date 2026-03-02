// 连接管理相关,包括添加、删除、通过一个连接ID获得连接对象，当前连接数量、清空全部连接等方法
package interfaces

/*
连接管理抽象层
*/
type IConnManager interface {
	Add(conn IConnection)                   // 添加链接
	Remove(conn IConnection)                // 删除连接
	Get(connID uint64) (IConnection, error) // 利用ConnID获取链接
	Len() int                               // 获取当前连接
	ClearConn()                             // 删除并停止所有链接
	Info() map[uint64]map[string]interface{}
}
