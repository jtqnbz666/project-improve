package config

// 读取http配置
func ParseConsulFinder() (address, serviceID string) {
	file := GlobalServerConfig.ConfigFile

	section := file.Section("ConsulFinder")
	address = section.Key("Address").MustString("127.0.0.1")
	serviceID = section.Key("Service").MustString("1")

	return
}