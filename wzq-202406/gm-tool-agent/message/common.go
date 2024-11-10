package message

type BaseBackMsg struct {
	Success bool   `json:"success"`
	Err     string `json:"err"`
}
