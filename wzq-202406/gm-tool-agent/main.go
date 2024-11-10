package main

import (
	"fmt"
	"github.com/quipo/statsd"
	"gm-tool-agent/api/setting"
	"gm-tool-agent/api/user"
	"gm-tool-agent/global"
	"gm-tool-agent/service"
	"gm-tool-agent/service/server"
	"infi/config"
)

func main() {
	global.StatsdClient = statsd.NewStatsdClient(config.GlobalServerConfig.ConfigFile.Section("Statsd").Key("Address").MustString("127.0.0.1:8125"), "dev"+".")
	err := global.StatsdClient.CreateSocket()
	if err != nil {
		fmt.Println(err)
	}
	defer global.StatsdClient.Close()

	service.ConnectToMysql()
	service.ConnectToRedis()
	go service.StartListener()
	address, _ := config.ParseHTTP()

	http := server.NewHttp(address)

	http.AddRouter("/api/user/list", true, server.RouteFunc(user.List))
	http.AddRouter("/api/setting/authCode", true, server.RouteFunc(setting.AuthCodeSet))

	http.Serve()
}
