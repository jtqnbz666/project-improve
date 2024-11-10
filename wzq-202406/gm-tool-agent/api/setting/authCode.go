package setting

import (
	"context"
	"fmt"
	"github.com/sirupsen/logrus"
	"gm-tool-agent/global"
	"gm-tool-agent/message"
	"time"
)

type AuthCodeSetReq struct {
	Phone string `json:"phone"`
}

func AuthCodeSet(ctx context.Context, req *AuthCodeSetReq) (any, error) {
	logrus.Infof("收到设置手机登录验证码消息:%v", req)
	backMsg := message.BaseBackMsg{}
	keyName := fmt.Sprintf("sms_%v", req.Phone)
	duration := 3600 * time.Second
	expireAt := time.Now().Add(duration).Unix()
	res := global.RedisIns.HSet(context.Background(), keyName, "123456", expireAt)
	if res.Err() == nil {
		backMsg.Success = true
	}

	return backMsg, nil
}
