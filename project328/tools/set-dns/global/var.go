package global

import (
	"gorm.io/gorm"
	"log"
)

var (
	MysqlIns *gorm.DB
	Logger   *log.Logger
)
