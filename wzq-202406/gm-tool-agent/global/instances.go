package global

import (
	dysmsapi20170525 "github.com/alibabacloud-go/dysmsapi-20170525/v3/client"
	"github.com/go-redis/redis/v8"
	"github.com/quipo/statsd"
	"gorm.io/gorm"
)

var (
	MysqlIns *gorm.DB
	// 连接信息redis
	RedisIns     redis.UniversalClient
	Sms          *dysmsapi20170525.Client
	StatsdClient *statsd.StatsdClient
)
