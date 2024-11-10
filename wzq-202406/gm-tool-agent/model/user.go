package model

import "gorm.io/gorm"

type User struct {
	ID            uint64
	Account       string `gorm:"column:account"`
	Name          string `gorm:"column:name"`
	DisplayID     string `gorm:"column:display_id"`
	PhoneNumber   string `gorm:"column:phone_number"`
	LastLoginTime string `gorm:"column:regist_time"`
}

// TableName 方法返回自定义的表名
func (User) TableName() string {
	return "user" // 返回你想要的表名
}

type MobileNumberAccount struct {
	gorm.Model
	ID          uint64 `gorm:"primarykey"`
	AccountID   uint64
	PhoneNumber string `gorm:"type:varchar(100);uniqueIndex:i_phone_number_id"`
}
