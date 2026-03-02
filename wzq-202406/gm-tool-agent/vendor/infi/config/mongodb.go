package config

func ParseMongo() (uri string, db string) {
	file := GlobalServerConfig.ConfigFile

	section := file.Section("Mongo")
	uri = section.Key("Uri").MustString("mongodb://localhost:27017")
	db = section.Key("DB").MustString("project328")
	return
}
