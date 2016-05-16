#!/bin/bash

export LLVM_DIR=/home/morris1028/LLVM/llvm-3.8.0.src/build/bin
export PATH=$LLVM_DIR:$PATH
export LD_LIBRARY_PATH=/home/morris1028/LLVM/llvm-3.8.0.src/build/lib:$LD_LIBRARY_PATH
CLANG_ENGINE=clang-3.8

echo $LD_LIBRARY_PATH;

$CLANG_ENGINE -m32 -fno-inline a.c -S -emit-llvm -o a.ll
opt a.ll -O2 -disable-inlining -o a.1.ll
llc a.1.ll -march=x86 -O3 -o a.1.s
$CLANG_ENGINE -m32 a.1.s -o a.o
