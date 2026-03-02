package errors

import (
	"fmt"
)

type FormatError string

func (p FormatError) Error() string {
	return string(p)
}

func NewFormatError(format string, a ...any) FormatError {
	return FormatError(fmt.Sprintf(format, a...))
}
