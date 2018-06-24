#!/bin/bash

BASE_PATH="pdfiumjs/"
INC_OUT_DIR=$1
shift

mkdir -p $INC_OUT_DIR

echo Copying header files ...

echo "BasePath => $BASE_PATH"
for SRC in "$@"; do
    TGT="$INC_OUT_DIR/${SRC#$BASE_PATH}"
    echo "Copying $SRC => $TGT"
    mkdir -p "$(dirname $TGT)"
    cp "$SRC" "$TGT"
done
