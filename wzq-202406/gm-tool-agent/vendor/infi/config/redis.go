package config

// 读取Redis配置
func ParseRedis() (uri, pwd string, db int) {
	file := GlobalServerConfig.ConfigFile

	section := file.Section("Redis")
	uri = section.Key("Uri").MustString("127.0.0.1:6379")
	pwd = section.Key("Pwd").MustString("")
	db = section.Key("DB").MustInt(0)

	return
}
