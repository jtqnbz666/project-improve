package main

import (
	"fmt"

	"gorm.io/driver/mysql"
	"gorm.io/gorm"
)

type People struct {
	Age  int
	Name string `gorm:"default:'jt'"`
	// Birthday time.Time
	Ge     string `gorm:"default:'wh'"`
	Season string
	New    int
}

type user struct {
	Age  int
	Name string
}

type test struct {
	Age int
	// 如果update的结构体没有对应字段， 就算select了，也不会清空为0
}

func main() {
	// 参考 https://github.com/go-sql-driver/mysql#dsn-data-source-name 获取详情
	dsn := "root:123456@tcp(127.0.0.1:3306)/test?charset=utf8mb4&parseTime=True&loc=Local"
	db, _ := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	db.AutoMigrate(&People{}) //自动生成表
	// Birthday: time.Now(),
	//user1 := People{Name: "Jinzhu", Age: 18, New: 123}
	//db.Create(&user1) //添加一条记录

	user3 := &People{}
	db.Model(&People{}).Select("name").Where("age = ?", 18).First(&user3)
	fmt.Printf("season:%v, name:%v, age:%v\n", user3.Season, user3.Name, user3.Age)

	//1. 使用select进行选择
	// if user3.Season == "" {
	// 	fmt.Println("使用select进行选择")
	// 	db.Model(&People{}).Select("name").Where("age = ?", 18).First(&user3).Updates(&People{
	// 		Season: "S2",
	// 		Name:   "1",
	// 	}).First(&user3)

	// }
	// fmt.Println(user3.Season)

	//1.1 使用select进行选择, 但update的结构体里面并没有这个字段
	if user3.Season == "" {
		fmt.Println("使用select进行选择")
		db.Model(&People{}).Select("name").Where("age = ?", 18).First(&user3).Updates(&test{
			Age: 10,
		}).First(&user3)

	}
	fmt.Println(user3.Season)

	//2. 不使用select进行选择
	// if user3.Season == "" {
	// 	fmt.Println("不使用select进行选择")
	// 	db.Model(&People{}).Where("age = ?", 18).First(&user3).Updates(&People{
	// 		//Season: "S3",
	// 		Name: "",
	// 	}).First(&user3)

	// }
	// fmt.Println(user3.Season)

	//3. 第一个函数Find得到的model为user，但并不存在， 所以报错
	// var userTest user
	// if user3.Season == "" {
	// 	fmt.Println("不使用model")
	// 	db.Where("age = ?", 18).Find(&userTest).Updates(&People{
	// 		Season: "S7",
	// 		Name:   "xx",
	// 	})
	// }

	//4，第一个函数Updates得到的model为people， 正常操作
	// var userTest user
	// if user3.Season == "" {
	// 	fmt.Println("不使用model")
	// 	db.Where("age = ?", 18).Updates(&People{
	// 		//Season: "S7",
	// 		Name: "gg",
	// 	}).Find(&userTest)
	// }
	// fmt.Println(user3.Season)
	// var str string //读单个字段season
	// db.Model(&People{}).Select("season").Where("age = ?", 18).First(&user3).Updates(&People{
	// 	Season: "S1",
	// }).First(&str)
	// fmt.Println(str)

	// var u1 user //只要字段一致(People和user都有name和age字段)，就可以读到它里面
	// db.Model(&People{}).Select("name", "age").Where("age = ?  and status = 1", 18).First(&user3).Updates(&People{
	// 	Season: "S1",
	// }).First(&u1)
	// fmt.Println(u1)
}
