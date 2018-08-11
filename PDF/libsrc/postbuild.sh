#!/bin/bash

INC_OUT_DIR=$1
shift

mkdir -p $INC_OUT_DIR

echo Copying header files ...
for SRC in "$@"; do
    TGT="$INC_OUT_DIR/$SRC"
    echo "Copying $SRC => $TGT"
    mkdir -p "$(dirname $TGT)"
    cp "$SRC" "$TGT"
done
