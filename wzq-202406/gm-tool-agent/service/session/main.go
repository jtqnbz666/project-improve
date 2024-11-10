package session

import (
	"fmt"
	"github.com/golang-jwt/jwt/v4"
	"gm-tool-agent/gm"
	"infi/config"
	"sync"
	"time"
)

type Manager struct {
	key       []byte
	userCache map[string]*gm.User
	sync.RWMutex
}

var sessionMgr *Manager

func Init() {
	key := config.GlobalServerConfig.ConfigFile.Section("Jwt").Key("Key").MustString("")
	if key == "" {
		panic("jwt key不能为空")
	}
	sessionMgr = &Manager{
		key:       []byte(key),
		userCache: map[string]*gm.User{},
	}
}

func GetInstance() *Manager {
	return sessionMgr
}

func (m *Manager) Create(user *gm.User, ttl time.Duration) string {
	idStr := fmt.Sprintf("%v", user.ID)
	m.Lock()
	m.userCache[idStr] = user
	m.Unlock()

	c := &jwt.RegisteredClaims{
		ExpiresAt: jwt.NewNumericDate(time.Now().Add(ttl)),
		ID:        idStr,
	}
	token := jwt.NewWithClaims(jwt.SigningMethodHS256, c)
	res, _ := token.SignedString(m.key)
	return res
}
