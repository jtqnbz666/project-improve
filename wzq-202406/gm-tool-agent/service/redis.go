package service

import (
	"gm-tool-agent/global"
	"infi/config"
	"infi/db"
)

func ParseRedis(sectionName string) (uri string, name string, pwd string, db int) {
	file := config.GlobalServerConfig.ConfigFile

	section := file.Section(sectionName)
	uri = section.Key("Uri").MustString("127.0.0.1")
	name = section.Key("Name").MustString("")
	pwd = section.Key("Pwd").MustString("")
	db = section.Key("DB").MustInt(0)

	return
}

func ConnectToRedis() {
	uri, name, pwd, dbNum := ParseRedis("Redis")
	global.RedisIns = db.ConnectRedis(uri, name, pwd, dbNum)
}
