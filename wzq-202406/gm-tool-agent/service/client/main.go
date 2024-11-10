package client

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"time"

	"infi/config"
	"infi/utils"
	"github.com/sirupsen/logrus"
)

var address string
var backUrl string

func InitConf() {
	section := config.GlobalServerConfig.ConfigFile.Section("Core")
	address = section.Key("Server").MustString("")
	backUrl = section.Key("BackUrl").MustString("")
	if address == "" {
		logrus.Panicf("配置Core.Server找不到")
	}
	if backUrl == "" {
		logrus.Panicf("配置Core.BackUrl找不到")
	}
}

func request[T interface{}](path string, data interface{}) T {
	c := &http.Client{}
	sendData := utils.JSONMarshal(data)
	for i := 0; i < 5; i++ {
		// 出错的话重试n次
		resp, err := c.Post(fmt.Sprintf("%v%v", address, path), "application/json", bytes.NewReader([]byte(sendData)))
		if err != nil {
			logrus.Warnf("gm tool 核心请求错误, 重试 %v %v %v", address, path, err)
			time.Sleep(time.Second)
		} else {
			respBody, _ := io.ReadAll(resp.Body)
			var t T
			_ = json.Unmarshal(respBody, &t)
			return t
		}
	}
	logrus.Errorf("gm tool 核心请求错误, 重试5次均失败 %v %v", address, path)
	var zero T
	return zero
}
