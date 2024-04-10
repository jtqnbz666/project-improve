package main

import (
	"context"
	"fmt"
	"log"
	"time"

	"go.mongodb.org/mongo-driver/mongo"
	"go.mongodb.org/mongo-driver/mongo/options"
)

type Person struct {
	Name  string
	Age   int
	Email string
}

func main() {
	// 设置连接URI
	uri := "mongodb://127.0.0.1:27017"
	client, err := mongo.NewClient(options.Client().ApplyURI(uri))
	if err != nil {
		log.Fatal(err)
	}
	ctx, cancel := context.WithTimeout(context.Background(), 10*time.Second)
	defer cancel()

	// 连接数据库
	err = client.Connect(ctx)
	if err != nil {
		log.Fatal(err)
	}
	defer client.Disconnect(ctx)

	// 获取数据库实例
	db := client.Database("dev")

	// 在这里执行其他数据库操作，如插入文档、查询数据等
	fmt.Println("成功创建数据库！")
	// 获取集合实例
	collection := db.Collection("UserGameHistoryRecord")

	// 创建要插入的文档
	person := Person{
		Name:  "John Doe",
		Age:   30,
		Email: "johndoe@example.com",
	}

	// 插入文档
	_, err = collection.InsertOne(ctx, person)
	if err != nil {
		log.Fatal(err)
	}

	fmt.Println("插入数据成功！")
	// 初始化索引
	indexList, err := collection.Indexes().List(context.Background())
	if err != nil {
		panic(err)
		fmt.Println("hhha")
	}

	fmt.Println(indexList)
}
