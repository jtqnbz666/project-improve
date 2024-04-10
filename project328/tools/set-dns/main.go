package main

import (
	"fmt"
	"log"
	"os"
	"set-dns/global"
	"set-dns/model"
	"set-dns/service"
	"strconv"
)

func main() {
	global.Logger = log.Default()

	args := os.Args
	needArg := 7

	if len(args) != needArg {
		global.Logger.Fatal(fmt.Sprintf("param len err: [raw:%v][need:%v]", len(args), needArg))
	}

	version := args[1]
	platform := args[2]
	channel := args[3]
	packageName := args[4]
	host := args[5]
	portI, _ := strconv.Atoi(args[6])
	port := int32(portI)

	global.Logger.Println(fmt.Sprintf("Args: [version:%v][platform:%v][channel:%v][packageName:%v][host:%v][port:%v]", version, platform, channel, packageName, host, port))

	service.ConnectToMysql()

	conf := &model.LbConf{}
	res := global.MysqlIns.Where("platform = ? and package_name = ? and channel = ? and version = ?",
		platform,
		packageName,
		channel,
		version).First(conf)
	if res.RowsAffected > 0 {
		global.MysqlIns.Where("platform = ? and package_name = ? and channel = ? and version = ?",
			platform,
			packageName,
			channel,
			version).Select("host", "port").Updates(&model.LbConf{
			Host: host,
			Port: port,
		})
	} else {
		global.MysqlIns.Create(&model.LbConf{
			Version:     version,
			Platform:    platform,
			Channel:     channel,
			PackageName: packageName,
			Host:        host,
			Port:        port,
		})
	}
	global.Logger.Println("Done Success")
}
