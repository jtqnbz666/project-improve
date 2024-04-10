package model

type LbConf struct {
	Version     string `gorm:"index:i_conf,unique"`
	Platform    string `gorm:"index:i_conf,unique"`
	Channel     string `gorm:"index:i_conf,unique"`
	PackageName string `gorm:"index:i_conf,unique"`
	Host        string
	Port        int32
}
