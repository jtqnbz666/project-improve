#!/bin/bash
 protoc --cpp_out=./cggProto message.proto 
 protoc --csharp_out=./csharpProto message.proto 
