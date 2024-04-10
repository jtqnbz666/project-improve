#!/bin/bash

set -e

./fmt.sh
go build ./main.go
./main "999.888.777" "Switch" "TapTap" "com.hoxigames.jdslzd" "47.95.6.108" "8888"
