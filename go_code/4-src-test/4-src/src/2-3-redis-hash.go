package main 


import (
	"fmt"
	"github.com/gomodule/redigo/redis"
)

func main() {
	c, err := redis.Dial("tcp", "127.0.0.1:6379")
	if err != nil {
		fmt.Println("conn redis failed,", err)
		return
	}

	defer c.Close()
	_, err = c.Do("HSet", "books", "count", 100)
	if err != nil {
		fmt.Println(err)
		return 
	}
	r, err := redis.Int(c.Do("HGet", "books", "count"))
	if err != nil {
		fmt.Println("get count failed,", err)
		return
	}

	fmt.Println(r)
}
