package config

// 读取http配置
func ParseKafka() (addrs []string) {
	file := GlobalServerConfig.ConfigFile

	section := file.Section("Kafka")
	addrs = section.Key("Addrs").Strings(",")

	return
}

// 读取Kafka Consumer配置
func ParseKafkaConsumer() (groupID string, topics []string) {
	file := GlobalServerConfig.ConfigFile

	section := file.Section("Kafka")
	groupID = section.Key("GroupID").MustString("group")
	topics = section.Key("Topics").Strings(",")

	return
}
