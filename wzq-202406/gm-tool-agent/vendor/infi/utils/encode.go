package utils

import (
	"encoding/base64"
	"encoding/json"
	"github.com/sirupsen/logrus"
	"infi/errors"
	"google.golang.org/protobuf/proto"
)

func Base64UnmarshalProto[T any](data string) (*T, error) {
	var ret T
	if data == "" {
		return &ret, nil
	}
	byteData, err := base64.StdEncoding.DecodeString(data)
	if err != nil {
		return nil, errors.NewFormatError("base64解析失败[raw:%v][err:%v]", data, err)
	}

	pbPtr := interface{}(&ret).(proto.Message)
	if pbPtr == nil {
		return nil, errors.NewFormatError("待转的目标不是proto.Message")
	}

	err = proto.Unmarshal(byteData, pbPtr)
	if err != nil {
		return nil, errors.NewFormatError("base64 proto 解析失败 [raw:%v][proto:%v][err:%v]", data, pbPtr.ProtoReflect().Descriptor(), err)
	}
	return &ret, nil

}

// Base64Unmarshal
// Deprecated
func Base64Unmarshal[T proto.Message](dataType T, data string) T {
	if data == "" {
		return dataType
	}
	byteData, err := base64.StdEncoding.DecodeString(data)
	if err != nil {
		logrus.Warnf("base64 解析失败 %v", data)
		return dataType
	}
	err = proto.Unmarshal(byteData, dataType)
	if err != nil {
		logrus.Warnf("base64 proto 解析失败 %v %v", data, dataType.ProtoReflect().Descriptor())
		return dataType
	}
	return dataType
}

func Base64Marshal(data proto.Message) string {
	bytes, err := proto.Marshal(data)
	if err != nil {
		logrus.Warnf("proto 生成失败 %v %v", data, data.ProtoReflect().Descriptor())
	}
	return base64.StdEncoding.EncodeToString(bytes)
}

func JsonDecode[T any](data string) (*T, error) {
	if data == "" {
		return nil, nil
	}
	var ret T
	err := json.Unmarshal([]byte(data), &ret)
	if err != nil {
		return nil, errors.NewFormatError("json 解析失败 [raw:%v][err:%v]", data, err)
	}
	return &ret, nil
}

func JsonEncode(data interface{}) (string, error) {
	if data == nil {
		return "", nil
	}
	res, err := json.Marshal(data)
	if err != nil {
		return "", errors.NewFormatError("json 编码失败 [err:%v]", err)
	}
	return string(res), nil
}

func JsonMustEncode(data interface{}) string {
	ret, err := JsonEncode(data)
	if err != nil {
		panic(err)
	}
	return ret
}

// JSONUnmarshal
// Deprecated
func JSONUnmarshal(data string, dataType interface{}) interface{} {
	if data == "" {
		return nil
	}
	err := json.Unmarshal([]byte(data), dataType)
	if err != nil {
		logrus.Warnf("json 解析失败 %v", data)
		return nil
	}
	return dataType
}

// JSONUnmarshal
// Deprecated
func JSONMarshal(data interface{}) string {
	if data == nil {
		return ""
	}
	res, err := json.Marshal(data)
	if err != nil {
		logrus.Warnf("json 生成失败 %v", data)
		return ""
	}
	return string(res)
}
