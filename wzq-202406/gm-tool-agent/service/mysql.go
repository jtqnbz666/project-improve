package service

import (
	"infi/config"
	"infi/db"

	"gm-tool-agent/global"
)

func ConnectToMysql() {
	dsn, _ := config.ParseMysql()
	global.MysqlIns = db.ConnectMysql(dsn)
}
