package config

type ConsulRegisterConfig struct {
	Address string

	RegistrationID      string
	RegistrationName    string
	RegistrationPort    int
	RegistrationTags    []string
	RegistrationAddress string

	CheckMethod							string
	CheckTimeout                        string
	CheckInterval                       string
	CheckDeregisterCriticalServiceAfter string
}

// 读取consul配置
func ParseConsul() (consulConfig *ConsulRegisterConfig) {
	file := GlobalServerConfig.ConfigFile

	consulConfig = &ConsulRegisterConfig{}

	section := file.Section("Consul")
	consulConfig.Address = section.Key("Address").MustString("127.0.0.1")

	consulConfig.RegistrationID = section.Key("RegistrationID").MustString("1")
	consulConfig.RegistrationName = section.Key("RegistrationName").MustString("Server1")
	consulConfig.RegistrationPort = section.Key("RegistrationPort").MustInt(8081)
	consulConfig.RegistrationAddress = section.Key("RegistrationAddress").MustString("127.0.0.1")
	// Tags配置，以逗号分割
	tags := section.Key("RegistrationTags").Strings(",")
	consulConfig.RegistrationTags = tags

	consulConfig.CheckMethod = section.Key("CheckMethod").MustString("HTTP")
	consulConfig.CheckTimeout = section.Key("CheckTimeout").MustString("5s")
	consulConfig.CheckInterval = section.Key("CheckInterval").MustString("5s")
	consulConfig.CheckDeregisterCriticalServiceAfter = section.Key("CheckDeregisterCriticalServiceAfter").MustString("30s")

	return consulConfig
}
