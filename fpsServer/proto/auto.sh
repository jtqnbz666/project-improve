#!/bin/bash
 protoc --cpp_out=./cggProto message.proto base.proto
 protoc --csharp_out=./csharpProto message.proto base.proto
