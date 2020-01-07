#!/usr/bin/env bash

PROTOC=protoc
WORK_DIR=`pwd`
GRPC_CPP_PLUGIN_PATH=`which grpc_cpp_plugin`
PROTOS_PATH="${WORK_DIR}/../protos"
INCLUDE_PATH="${WORK_DIR}/include"
SRC_PATH="${WORK_DIR}/src"
PROTO_FILE="ftext.proto"

${PROTOC} -I ${PROTOS_PATH} --grpc_out=${SRC_PATH} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN_PATH} ${PROTO_FILE}
${PROTOC} -I ${PROTOS_PATH} --cpp_out=${SRC_PATH} ${PROTO_FILE}

mv ${SRC_PATH}/*.pb.h ${INCLUDE_PATH}

ls -l ${SRC_PATH}
ls -l ${INCLUDE_PATH}