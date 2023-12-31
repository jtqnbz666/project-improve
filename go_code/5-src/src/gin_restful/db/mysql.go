package db

import (
	"database/sql"
	"log"

	_ "github.com/go-sql-driver/mysql"
)

var SqlDB *sql.DB

func init() {
	var err error
	SqlDB, err = sql.Open("mysql", "root:nswdsm58@tcp(127.0.0.1:3306)/go_test?charset=utf8mb4")
	if err != nil {
		log.Fatal(err.Error())
	}
	err = SqlDB.Ping()
	if err != nil {
		log.Fatal(err.Error())
	}
	SqlDB.SetMaxIdleConns(20)
	SqlDB.SetMaxOpenConns(20)
}
