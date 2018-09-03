#!/bin/bash
OPTIM_FLAGS=(
    "-O0"
    # "-Oz"
    # "--llvm-lto 3"
    "-s DEMANGLE_SUPPORT=1"
    "-s FORCE_FILESYSTEM=1"
    # "-s ELIMINATE_DUPLICATE_FUNCTIONS=1"
    "-s MODULARIZE=1"
    # "--closure 1"
)
EMCC="emcc ${OPTIM_FLAGS[@]}"
EMPP="em++ ${OPTIM_FLAGS[@]}"
qmake \
    config+=DEBUG \
    QMAKE_CC="$EMCC" \
    QMAKE_COMPILER="$EMCC" \
    QMAKE_LINK_C="$EMCC" \
    QMAKE_LINK_C_SHLIB="$EMCC" \
    QMAKE_CXX="$EMPP" \
    QMAKE_LINK="$EMPP" \
    QMAKE_LINK_SHLIB="$EMPP" \
    QMAKE_AR="emar cqs"
