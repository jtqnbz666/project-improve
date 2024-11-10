package gm

type User struct {
	ID     uint64 `json:"id"`
	Name   string `json:"name"`
	Avatar string `json:"avatar"`
	OpenId string `json:"openId"`
}
