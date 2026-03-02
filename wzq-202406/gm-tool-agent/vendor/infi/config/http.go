package config

// 读取http配置
func ParseHTTP() (address string, ip string) {
	file := GlobalServerConfig.ConfigFile

	section := file.Section("Http")
	address = section.Key("Address").MustString("127.0.0.1")
	ip = section.Key("Ip").MustString("127.0.0.1")

	return
}
