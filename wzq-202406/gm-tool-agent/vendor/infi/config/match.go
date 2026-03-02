package config

const (
	MatchSectionName = "Match"
	FrontendServiceName = "FrontendService"
)

func ParseMatchFrontend() (uri string) {
	file := GlobalServerConfig.ConfigFile
	section := file.Section(MatchSectionName)
	uri = section.Key(FrontendServiceName).MustString("127.0.0.1:50504")
	return
}