package main

import (
	"fmt"

	_ "github.com/go-sql-driver/mysql"
	"github.com/jmoiron/sqlx"
)

// DB数据库模型
var DB *sqlx.DB

const dsn = "root:nswdsm58@tcp(127.0.0.1:3306)/go_test?charset=utf8mb4"

type user struct {
	ID   int    `json:"id" db:"id"`
	Name string `json:"name" db:"name"`
	Age  int    `json:"age" db"age"`
}

// 连接数据库
func connect() (db *sqlx.DB, err error) {
	db, err = sqlx.Connect("mysql", dsn)
	db.SetMaxOpenConns(100) //设置连接池中最大连接数
	db.SetMaxIdleConns(20)  //设置连接池最大空闲连接数
	DB = db
	if err != nil {
		fmt.Println("数据库连接失败-->", err)
	}
	fmt.Println("数据库连接成功！")
	return
}

// 添加数据 Exec、MustExec
// MustExec遇到错误时直接抛出一个panic错误， 程序就退出了
// Exec 是将错误和执行结果一起返回， 由我们自己处理错误。 推荐使用
func createUser() {
	//创建表
	sql := `
        CREATE TABLE user  (
            id bigint(20) NOT NULL AUTO_INCREMENT,
            name varchar(20) CHARACTER SET utf8mb4 COLLATE utf8mb4_general_ci NULL DEFAULT '',
            age int(11) NULL DEFAULT 0,
            PRIMARY KEY (id) USING BTREE
        ) ENGINE = InnoDB AUTO_INCREMENT = 20 CHARACTER SET = utf8mb4 COLLATE = utf8mb4_general_ci ROW_FORMAT = Compact
    `
	_, err := DB.Exec(sql)
	fmt.Println(err)
}

// 添加数据
func insertUser() {
	sql := `insert into user (name, age) values ("jtt", 19)`
	res := DB.MustExec(sql)
	fmt.Println(res.LastInsertId)
	fmt.Println(res.RowsAffected)
}

// 更新数据
func updateUser() {
	sql := `update user set name = ?, age = ? where id = ?`
	res, err := DB.Exec(sql, "JJT", 99, 66)
	fmt.Println(err, res)
}

// Get、QueryRowx:查询一条数据
// QueryRowx可以指定到不同的数据类型中
func getNum() {
	var num int
	_ = DB.Get(&num, "select count(*) from user")
	fmt.Printf("数据库一共有:%d 个用户\n", num)
	var u user
	_ = DB.Get(&u, "select name, id, age from user where id = ?", 3)
	fmt.Printf("查找用户id == 3 的用户:%v \n", u)
}

// Select 、 Queryx：查询多条数据
// Queryx可以指定到不同的数据类型中
func getAll() {
	sql := `select id, name, age from user where id > 1`
	var us []user
	err := DB.Select(&us, sql)
	fmt.Println(err, us)
}

// 删除
func deleteUser() {
	sql := `delete from user where id = 20`
	_, _ = DB.Exec(sql)
}

// 事务处理
func events() {
	tx, _ := DB.Beginx()
	_, err1 := tx.Exec("update user set age = 10 where id = 20")
	_, err2 := tx.Exec("update user set age = 10 where id = 21")
	fmt.Println(err1, err2)
	if err1 != nil || err2 != nil {
		tx.Rollback()
	}
	tx.Commit()
}

func main() {
	db, _ := connect()
	defer db.Close()
	//建表
	//createUser()
	//添加数据
	insertUser()
	//修改数据
	updateUser()
	//查数据-Get
	getNum()
	//查数据-Select
	getAll()
	//事务
	events()
}
