package user

import (
	"context"
	_ "encoding/json"
	"github.com/sirupsen/logrus"
	"gm-tool-agent/global"
	"gm-tool-agent/message"
	"gm-tool-agent/model"
	"strconv"
)

func searchByDisplayID(displayID string) uint64 {
	data := &model.User{}
	global.MysqlIns.Where("display_id = ?", displayID).First(data)
	return data.ID
}

func searchByPhoneNumber(phoneNumber string) uint64 {
	data := &model.User{}
	global.MysqlIns.Where("phone_number = ?", phoneNumber).First(data)
	return data.ID
}

func searchByAccount(account string) []uint64 {
	var uidList []uint64
	var data []model.User
	account = "%" + account + "%"
	result := global.MysqlIns.Where("name like ?", account).Find(&data)
	if result.Error != nil {
		logrus.Errorf("查询出错: %v", result.Error)
	}

	logrus.Infof("test:%v, len:%v", account, len(uidList))
	for _, person := range data {
		uidList = append(uidList, person.ID)
		logrus.Infof("uid:%v", person.ID)
	}

	return uidList
}

func buildSearchResult(uidList []uint64, total uint64) *message.UserListBackMsg {
	ret := &message.UserListBackMsg{
		BaseBackMsg: message.BaseBackMsg{
			Success: true,
			Err:     "",
		},
		Total: total,
	}
	var list []*model.User
	for _, uid := range uidList {
		userData := &model.User{}
		global.MysqlIns.Where("ID = ?", uid).First(userData)
		if userData.Name == "" && userData.PhoneNumber != "" {
			userData.Name = "新手玩家"
		}
		logrus.Infof("name:%v， displayID:%v, phoneNumber:%v", userData.Name, userData.DisplayID, userData.PhoneNumber)
		list = append(list, userData)
	}
	ret.List = list
	return ret
}

func List(ctx context.Context, msg *message.UserListReqMsg) (*message.UserListBackMsg, error) {
	// 返回消息
	backMsg := &message.UserListBackMsg{}
	backMsg.Success = true

	if search, has := msg.Condition["DisplayID"]; has && search != "" {
		uid := searchByDisplayID(search)
		return buildSearchResult([]uint64{uid}, uint64(1)), nil
	}
	if search, has := msg.Condition["UserName"]; has && search != "" {
		uidList := searchByAccount(search)
		return buildSearchResult(uidList, uint64(len(uidList))), nil
	}
	if search, has := msg.Condition["PhoneNumber"]; has && search != "" {
		uid := searchByPhoneNumber(search)
		return buildSearchResult([]uint64{uid}, uint64(1)), nil
	}
	if search, has := msg.Condition["UserID"]; has && search != "" {
		userID, err := strconv.ParseUint(search, 10, 64)
		if err == nil {
			return buildSearchResult([]uint64{userID}, 1), nil
		}
	} else {
		// 查询指定范围
		var list []*model.User
		var total int64
		global.MysqlIns.Model(model.User{}).Count(&total)
		global.MysqlIns.Limit(int(msg.Limit)).Offset(int(msg.Offset)).Order(msg.Order).Find(&list)
		backMsg.List = list
		backMsg.Total = uint64(total)
	}

	return backMsg, nil
}
