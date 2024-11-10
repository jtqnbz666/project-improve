package service

import (
	"context"
	"encoding/json"
	"fmt"
	openapi "github.com/alibabacloud-go/darabonba-openapi/v2/client"
	dysmsapi20170525 "github.com/alibabacloud-go/dysmsapi-20170525/v3/client"
	util "github.com/alibabacloud-go/tea-utils/v2/service"
	"github.com/alibabacloud-go/tea/tea"
	"github.com/sirupsen/logrus"
	"gm-tool-agent/global"
	"gm-tool-agent/message"
	"infi/errors"
	"math/rand"
	"strconv"
	"time"
)

// 短信服务

const (
	SMSSignature                = "李梓涛五子棋"
	SMSMessageTemplate          = "SMS_468960724"
	AlibabaCloudAccessKeyID     = "LTAI5tHv3xdza7Q9ZUtFhxVA"
	AlibabaCloudAccessKeySecret = "o77pJ7sSLWJJF0hivGA66CAphwveTU"
	SMSPrefix                   = "sms_"
	smsWhiteList                = "smsWhiteList_"
	SMSLimitPrefix              = "smsLimit_"
	SMSExpire                   = 5 * time.Minute
	limitVerificationDuration   = 15 * time.Second
	limitVerificationTimes      = 3
	smsWhiteListCode            = "123456"
)

func SMSInit() {
	global.Sms, _ = CreateClient(tea.String(AlibabaCloudAccessKeyID), tea.String(AlibabaCloudAccessKeySecret))
	if global.Sms == nil {
		logrus.Error("初始化短信服务失败")
	}
}

func generateVerificationCode() string {
	rand.Seed(time.Now().UnixNano())
	// 生成一个6位数的验证码
	code := fmt.Sprintf("%06d", rand.Intn(1000000))
	return code
}

func CreateClient(accessKeyId *string, accessKeySecret *string) (_result *dysmsapi20170525.Client, _err error) {
	config := &openapi.Config{
		AccessKeyId:     accessKeyId,
		AccessKeySecret: accessKeySecret,
	}

	config.Endpoint = tea.String("dysmsapi.aliyuncs.com")
	_result = &dysmsapi20170525.Client{}
	_result, _err = dysmsapi20170525.NewClient(config)
	return _result, _err
}

func SetVerificationCode(phoneNumber, code string) error {
	//校验一下该用户的当前记录的验证码个数，如果太多可能有问题
	const limitCodeCount = 100
	filedCount, err := global.RedisIns.HLen(context.Background(), SMSPrefix+phoneNumber).Result()
	if err != nil {
		return err
	} else {
		if filedCount > limitCodeCount {
			logrus.Error("可能存在恶意验证:%v", phoneNumber)
			return errors.NewRespError(message.RESULT_CODE_VERIFICATION_CODE_REQUEST_TOO_FREQUENT, "可能存在恶意验证%v", phoneNumber)
		}
	}
	// 用hash方便知道产生的验证码个数，做一些限制
	_, err = global.RedisIns.HSet(context.Background(), SMSPrefix+phoneNumber,
		code, time.Now().Unix()).Result()
	if err != nil {
		return err
	}

	// 重置过期时间
	_, err = global.RedisIns.Expire(context.Background(), SMSPrefix+phoneNumber, SMSExpire).Result()
	if err != nil {
		return err
	}

	return nil
}

// 验证码校验
func VerifyCode(phoneNumber, code string) (bool, error) {
	// 先检验是否白名单账号
	whiteCodes, err := global.RedisIns.Get(context.Background(), smsWhiteList+phoneNumber).Result()
	if whiteCodes != "" && whiteCodes == code {
		return true, nil
	}

	codes, err := global.RedisIns.HGetAll(context.Background(), SMSPrefix+phoneNumber).Result()
	if err != nil {
		logrus.Infof("redis没有该玩家验证码:phoneNumber:%v, code:%v", phoneNumber, code)
		return false, err
	}

	// 使用INCR增加验证码使用次数，设置15秒过期时间
	countKey := SMSLimitPrefix + phoneNumber
	count, err := global.RedisIns.Incr(context.Background(), countKey).Result()
	if err != nil {
		logrus.Infof("redis自增失败:phoneNumber:%v, code:%v", phoneNumber, code)
		return false, err
	}
	if count == 1 {
		global.RedisIns.Expire(context.Background(), countKey, limitVerificationDuration)
	}

	// 如果验证码使用次数超过5次，则告知操作太频繁
	if count > limitVerificationTimes {
		logrus.Warnf("玩家验证太频繁:phoneNumber:%v, code:%v", phoneNumber, code)
		return false, errors.NewRespError(message.RESULT_CODE_VERIFICATION_CODE_TRY_FREQUENT,
			"验证码尝试太频繁[code:%v][phone:%v]", code, phoneNumber)
	}

	for k, v := range codes {
		if k == code {
			timestamp, _ := strconv.ParseInt(v, 10, 64)
			// 判断验证码是否在5分钟内有效期
			if time.Now().Unix()-timestamp <= int64(SMSExpire) {
				logrus.Infof("验证码校验成功:phoneNumber:%v, code:%v", phoneNumber, code)
				// 次数限制验证通过，清除验证码使用次数记录
				global.RedisIns.Del(context.Background(), countKey)
				return true, nil
			} else {
				logrus.Infof("验证码校验失败:phoneNumber:%v, code:%v", phoneNumber, code)
				return false, errors.NewRespError(message.RESULT_CODE_VERIFICATION_CODE_EXPIRED, "验证码已过期[code:%v][phone:%v]", code, phoneNumber)
			}
		}
	}

	return false, errors.NewRespError(message.RESULT_CODE_VERIFICATION_CODE_ERROR, "不正确的验证码[code:%v][phone:%v]", code, phoneNumber)

}

func doSend(phoneNumber string) (_err error) {
	authCode := generateVerificationCode()
	templateParam := map[string]string{
		"code": authCode,
	}

	jsonStr, _ := json.Marshal(templateParam)
	templateParamStr := string(jsonStr)
	sendSmsRequest := &dysmsapi20170525.SendSmsRequest{
		SignName:      tea.String(SMSSignature),
		TemplateCode:  tea.String(SMSMessageTemplate),
		PhoneNumbers:  tea.String(phoneNumber),
		TemplateParam: &templateParamStr,
	}

	runtime := &util.RuntimeOptions{}
	sendResult, _err := global.Sms.SendSmsWithOptions(sendSmsRequest, runtime)
	logrus.Infof("sendResult:%v, err:%v", *sendResult, _err)

	if sendResult == nil {
		return errors.NewRespError(message.RESULT_CODE_VERIFICATION_CODE_REQUEST_ERROR,
			"验证码发生返回内容为空[code:%v][phone:%v][err:%v]", authCode, phoneNumber, _err)
	}

	if *sendResult.Body.Code == "isv.BUSINESS_LIMIT_CONTROL" {
		return errors.NewRespError(message.RESULT_CODE_VERIFICATION_CODE_REQUEST_TOO_FREQUENT,
			"验证码请求太频繁[code:%v][phone:%v]", authCode, phoneNumber)
	}

	// 设置验证码有效期
	err := SetVerificationCode(phoneNumber, authCode)
	if err != nil {
		return err
	}

	logrus.Infof("手机号: %v, 验证码: %v, 有效期至: %v\n",
		phoneNumber, authCode, time.Now().UTC().Add(SMSExpire))
	return nil
}

func SendSms(phoneNumber string) error {
	err := doSend(phoneNumber)
	if err != nil {
		logrus.Warnf("发送短信失败:phone:%v, error:%+v", phoneNumber, err)
		return err
	} else {
		logrus.Infof("发送短信成功:phone:%v", phoneNumber)
	}
	return nil
}
