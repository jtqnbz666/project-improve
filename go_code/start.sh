#!/bin/bash

set -xe

cd "$(dirname "$(readlink -f "$0")")" || exit 1

docker run -d --restart always --name test -p 59999:3000 -w /app -v "$(pwd)":/app -v "$(readlink -f ~/go)":/go -e 'GOPROXY=https://goproxy.cn,direct' golang:1.21 go run main.go

