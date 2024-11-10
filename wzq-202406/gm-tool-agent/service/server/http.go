package server

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"gm-tool-agent/global"
	"io/ioutil"
	"net/http"
	"reflect"
	"time"

	"github.com/sirupsen/logrus"

	"gm-tool-agent/gm"
	"gm-tool-agent/message"
)

type Http struct {
	server *http.Server
}

// NewHttp 创建服务器
func NewHttp(address string) *Http {
	return &Http{
		server: &http.Server{
			Addr:         address,
			Handler:      http.NewServeMux(),
			ReadTimeout:  30 * time.Second,
			WriteTimeout: 30 * time.Second,
		},
	}
}

func (s *Http) Serve() {
	err := s.server.ListenAndServe()
	if err != nil {
		logrus.Panicf("web-server-start: %v", err)
	}
}

func (s *Http) AddRouter(path string, needLogin bool, handler func(byteData []byte, user *gm.User) interface{}) {
	s.AddRawRouter(path, needLogin, func(byteData []byte, user *gm.User, _ http.ResponseWriter, _ *http.Request) interface{} {
		return handler(byteData, user)
	})
}

func (s *Http) AddRawRouter(path string, needLogin bool, handler func(byteData []byte, user *gm.User, writer http.ResponseWriter, request *http.Request) interface{}) {
	s.server.Handler.(*http.ServeMux).HandleFunc(path, func(writer http.ResponseWriter, request *http.Request) {
		global.StatsdClient.Incr("gm_tool.req.req_in.count", 1)
		t0 := time.Now()
		writer.Header().Set("Access-Control-Allow-Origin", "*")
		writer.Header().Set("Access-Control-Allow-Headers", "*")
		writer.Header().Set("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
		if request.Method == "OPTIONS" {
			writer.WriteHeader(200)
			return
		}
		logrus.Infof("收到请求 %v %v, 匹配 %v", request.Method, request.RequestURI, path)
		body, err := ioutil.ReadAll(request.Body)
		if err != nil {
			logrus.Warnf("读取请求失败 %v %v %v", request.Method, request.RequestURI, err)
			writer.WriteHeader(400)
			return
		}

		statusCode := 200
		var back interface{}
		user := &gm.User{}
		needLogin = false

		statusCode = 200
		back = handler(body, user, writer, request)

		writer.Header().Set("Content-Type", "application/json")
		writer.WriteHeader(statusCode)
		_, err = writer.Write(encodeBack(back))
		if err != nil {
			logrus.Warnf("写回数据失败 %v %v", path, err)
		}
		global.StatsdClient.Timing("gm_tool.req.timer", time.Since(t0).Milliseconds())
		logrus.Infof("time:%v", time.Since(t0).Milliseconds())
	})
}

func (s *Http) AddStaticRouter(path string, handler func(request *http.Request) ([]byte, string)) {
	s.server.Handler.(*http.ServeMux).HandleFunc(path, func(writer http.ResponseWriter, request *http.Request) {
		if request.Method == "OPTIONS" {
			writer.WriteHeader(200)
			return
		}
		data, fileType := handler(request)
		if data == nil {
			writer.WriteHeader(404)
			return
		}
		switch fileType {
		case ".js":
			writer.Header().Set("Content-Type", "application/javascript")
		case ".css":
			writer.Header().Set("Content-Type", "text/css")
		case ".html":
			writer.Header().Set("Content-Type", "text/html")
		case ".ico":
			writer.Header().Set("Content-Type", "image/x-icon")
		default:
			writer.Header().Set("Content-Type", "text/plain")
		}
		_, err := writer.Write(data)
		if err != nil {
			logrus.Warnf("%v 数据写回失败", request.RequestURI)
		}
	})
}

func encodeBack(msg interface{}) []byte {
	res, err := json.Marshal(msg)
	if err != nil {
		logrus.Warnf("json生成失败")
		return make([]byte, 0)
	}
	return res
}

// RouteFunc
// f 必须为以下形式
//
//	func(ctx context.Context, v *Req) error
//	func(ctx context.Context, v *Req, user *gm.User) (*Resp, error)
//
// 其中 *Req 和 *Resp 的类型可以自定义，这个参数会通过 reflect 来进行注入。
// 其中 *Req 可以使用 _ struct{} 表示不关心 Req 内容。
// 其中 v *Req, user *gm.User 和 *Resp, 可省略。
// 返回 err != nil 会返回 Success = false, Err = err.Error()。
func RouteFunc(f any) func(byteData []byte, user *gm.User) any {
	v := reflect.ValueOf(f)
	t := v.Type()
	if t.Kind() != reflect.Func {
		logrus.Error("handler must be a function", "funcType", t.String())
		panic(errors.New("handler must be a function"))
	}
	numIn := t.NumIn()
	numOut := t.NumOut()
	if numIn >= 1 {
		if !t.In(0).Implements(reflect.TypeOf((*context.Context)(nil)).Elem()) {
			logrus.Error("handler function 1st arguments must be ctx", "funcType", t.String())
			panic(errors.New("handler function 1st arguments must be ctx"))
		}
	}
	if numIn >= 2 {
		// 第二个参数可以是任意 json.Unmarshal 能解析的类型
	}
	if numIn >= 3 {
		if t.In(2) != reflect.TypeOf((*gm.User)(nil)) {
			logrus.Error("handler function 3rd arguments must be *gm.User", "funcType", t.String())
			panic(errors.New("handler function 3rd arguments must be *gm.User"))
		}
	}
	if numIn >= 4 {
		logrus.Error("handler function wrong input arguments count", "funcType", t.String())
		panic(errors.New("handler function wrong input arguments count"))
	}
	if numOut >= 3 {
		logrus.Error("handler function wrong output results count", "funcType", t.String())
		panic(errors.New("handler function wrong output results count"))
	}

	return func(byteData []byte, user *gm.User) any {
		ctxReflectValue := reflect.ValueOf(context.TODO())
		userValue := reflect.ValueOf(user)
		var in []reflect.Value
		if numIn >= 1 {
			in = append(in, ctxReflectValue)
		}
		if numIn >= 2 {
			v := reflect.New(t.In(1))
			err := json.Unmarshal(byteData, v.Interface())
			if err != nil {
				logrus.Error("json unmarshal error", "err", err, "funcType", t.String())
				return &message.BaseBackMsg{
					Success: false,
					Err:     "json 解析失败",
				}
			}
			in = append(in, v.Elem())
		}
		if numIn >= 3 {
			in = append(in, userValue)
		}
		resultsReflectValue := v.Call(in)
		if numOut == 0 {
			return &message.BaseBackMsg{
				Success: true,
				Err:     "",
			}
		} else if numOut == 1 {
			if !resultsReflectValue[0].IsNil() {
				err := resultsReflectValue[0].Interface().(error)
				if err != nil {
					logrus.Error("call function return error", "err", err, "funcType", t.String())
					return &message.BaseBackMsg{
						Success: false,
						Err:     err.Error(),
					}
				}
				return &message.BaseBackMsg{
					Success: true,
					Err:     "",
				}
			}
			return &message.BaseBackMsg{
				Success: true,
				Err:     "",
			}
		} else if numOut == 2 {
			if !resultsReflectValue[1].IsNil() {
				err := resultsReflectValue[1].Interface().(error)
				if err != nil {
					logrus.Error("call function return error", "err", err, "funcType", t.String())
					return &message.BaseBackMsg{
						Success: false,
						Err:     err.Error(),
					}
				}
				return resultsReflectValue[0].Interface()
			}
			return resultsReflectValue[0].Interface()
		} else {
			// unreachable
			return &message.BaseBackMsg{
				Success: false,
				Err:     fmt.Sprintf("unexpected function NumOut %d", numOut),
			}
		}
	}
}
