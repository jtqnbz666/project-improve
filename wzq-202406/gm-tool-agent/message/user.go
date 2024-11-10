package message

import "gm-tool-agent/model"

type UserListBackMsg struct {
	BaseBackMsg
	List  []*model.User `json:"list"`
	Total uint64        `json:"total"`
}

type UserListReqMsg struct {
	Order     string            `json:"order"`
	Limit     int32             `json:"limit"`
	Offset    uint64            `json:"offset"`
	Condition map[string]string `json:"condition"`
}
