package db

import (
	"context"
	"github.com/go-redis/redis/v8"
	"github.com/sirupsen/logrus"
	"infi/utils"
	"io/ioutil"
	"strings"
	"time"
)

const timeoutPing = 3 * time.Second

func ConnectRedis(uri string, user string, pwd string, db int) redis.UniversalClient {
	options := &redis.UniversalOptions{
		Addrs:    strings.Split(uri, " "),
		DB:       db,
		Username: user,
		Password: pwd,
	}

	return ConnectRedisCore(options)
}

func ConnectRedisWithPoolSize(uri string, user string, pwd string, db, poolSize int) redis.UniversalClient {
	options := &redis.UniversalOptions{
		Addrs:    strings.Split(uri, " "),
		DB:       db,
		Username: user,
		Password: pwd,
		PoolSize: poolSize,
	}

	return ConnectRedisCore(options)
}

func ConnectRedisCore(options *redis.UniversalOptions) redis.UniversalClient {
	client := redis.NewUniversalClient(options)

	ctx, cancel := utils.MakeCtx(timeoutPing)
	defer cancel()

	pong, err := client.Ping(ctx).Result()

	if err != nil {
		logrus.WithField("role", "connect-redis").Error(pong, err)
		return nil
	}

	return client
}

func RunScript(client redis.UniversalClient, src string, keys []string, args ...interface{}) *redis.Cmd {
	return redis.NewScript(src).Run(client.Context(), client, keys, args...)
}

func RunScriptFromFile(client redis.UniversalClient, path string, keys []string, args ...interface{}) (*redis.Cmd, error) {
	content, err := ioutil.ReadFile(path)
	if err != nil {
		return nil, err
	}
	cmd := RunScript(client, string(content), keys, args...)
	return cmd, cmd.Err()
}

// 发布
func Publish(client redis.UniversalClient, channel, data string) (err error) {
	err = client.Publish(context.Background(), channel, data).Err()
	return err
}

// 订阅
func SubScribe(client redis.UniversalClient, channel string, doFunc func(string)) {
	sub := client.Subscribe(context.Background(), channel)
	_, err := sub.Receive(context.Background())
	if err != nil {
		return
	}
	ch := sub.Channel()
	for msg := range ch {
		doFunc(msg.Payload)
	}
}
