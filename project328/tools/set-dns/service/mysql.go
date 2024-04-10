package service

import (
	"fmt"
	"gorm.io/driver/mysql"
	"gorm.io/gorm"
	"gorm.io/plugin/dbresolver"
	"set-dns/global"
	"time"
)

const MYSQL_DSN = "root:hoxi0328JING@tcp(rm-bp1c4m1w22oq77q82.rwlb.rds.aliyuncs.com:3306)/gm?charset=utf8mb4&parseTime=True&loc=Local"

func ConnectToMysql() {
	global.MysqlIns = connectMysql(MYSQL_DSN)
	//global.MysqlIns.Config.Logger = logger.Default.LogMode(logger.Info)
}

func connectMysql(dsn string) *gorm.DB {
	db, err := gorm.Open(mysql.Open(dsn), &gorm.Config{})
	if err != nil {
		global.Logger.Fatal(fmt.Sprintf("open-db-err: [%v]", err))
		return nil
	}

	var splittingDB []gorm.Dialector

	err = db.Use(dbresolver.Register(dbresolver.Config{Replicas: splittingDB}).
		SetConnMaxIdleTime(time.Hour).
		SetConnMaxLifetime(24 * time.Hour).
		SetMaxIdleConns(100).
		SetMaxOpenConns(200))
	if err != nil {
		global.Logger.Fatal(fmt.Sprintf("connect-db-err: [%v]", err))
		return nil
	}

	return db
}
