package config

// 读取Redis配置
func ParseMysql() (dsn string, replicas []string) {
	file := GlobalServerConfig.ConfigFile

	section := file.Section("Mysql")
	dsn = section.Key("Dsn").MustString("user:pass@tcp(127.0.0.1:3306)/dbname?charset=utf8mb4&parseTime=True&loc=Local")
	replicas = section.Key("Replicas").Strings(",")

	return
}