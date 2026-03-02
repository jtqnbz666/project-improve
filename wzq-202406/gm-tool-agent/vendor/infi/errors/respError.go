package errors

import (
	"errors"
	"fmt"
)

type RespError struct {
	Info string
	Code int32
}

func NewRespError(code int32, format string, a ...any) RespError {
	return RespError{
		Code: code,
		Info: fmt.Sprintf(format, a...),
	}
}

func (p RespError) Error() string {
	return fmt.Sprintf("%v[code:%v]", p.Info, p.Code)
}

func AsRespError(err error) *RespError {
	if err == nil {
		return nil
	}
	ret := &RespError{}
	if errors.As(err, ret) {
		return ret
	}
	return nil
}
