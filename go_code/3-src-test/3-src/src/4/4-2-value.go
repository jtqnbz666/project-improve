package main

import (
	"context"
	"fmt"
	"time"
)

var key string = "name"
var key2 string = "name1"

func main() {
	ctx, cancel := context.WithCancel(context.Background())
	//附加值
	valueCtx := context.WithValue(ctx, key, "key[监控1]") //是否可以有多个key
	valueCtx2 := context.WithValue(valueCtx, key2, "key [监控2]")

	go watch(valueCtx2)

	time.Sleep(5 * time.Second)
	fmt.Println("可以了， 通知监控停止")
	cancel() //把ctx 取消了之后， valueCtx 和 valueCtx2也会跟着结束
	//为了检测监控是否停止， 如果没有监控输出， 就表示停止了
	time.Sleep(5 * time.Second)
}

func watch(ctx context.Context) {
	for {
		select {
		case <-ctx.Done():
			//取出值

			//使用WithValue 方法， 这里就可以调用ctx.Value 方法
			fmt.Println(ctx.Value(key), "监控退出， 停止了。。。")
			fmt.Println(ctx.Value(key2), "监控退出， 停止了。。。")
			return
		default:
			//取出值
			fmt.Println(ctx.Value(key), "goroutine_key监控中。。。")
			fmt.Println(ctx.Value(key2), "goroutine_key2监控中。。。")
			time.Sleep(2 * time.Second)
		}
	}
}
