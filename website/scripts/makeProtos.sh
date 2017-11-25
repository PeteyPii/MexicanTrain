#!/bin/bash
# Run this from parent directory!
mkdir -p gen
PROTO_DIR=../common/src
PROTOS=$(find $PROTO_DIR -name '*.proto')
protoc -I$PROTO_DIR --js_out=import_style=commonjs,binary:gen $PROTOS
