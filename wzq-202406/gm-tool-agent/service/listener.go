package service

import (
	"context"
	"encoding/json"
	"fmt"
	"gm-tool-agent/global"
)

type UserRawData struct {
	PhoneNumber string `json:"phoneNumber"`
}

func StartListener() {
	// 初始化短信服务
	SMSInit()

	for {
		fmt.Println("Start Listening")
		result, err := global.RedisIns.BLPop(context.Background(), 0, "WZQ_GM_TOOL").Result()
		if err != nil {
			fmt.Println("Error:", err)
			return
		} else {
			fmt.Printf("receive one msg:%s", result[1])
		}

		if len(result) > 1 {
			data := result[1]
			var userRawData UserRawData
			err := json.Unmarshal([]byte(data), &userRawData)
			if err != nil {
				fmt.Println("Error unmarshalling JSON:", err)
				return
			}
			fmt.Println("Extracted phoneNumber:", userRawData.PhoneNumber)
			err = SendSms(userRawData.PhoneNumber)
			if err != nil {
				return
			}
		}
	}
}
