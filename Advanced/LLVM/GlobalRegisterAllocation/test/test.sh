#!/bin/bash

TEXT_RED=`tput setaf 1`
TEXT_GREEN=`tput setaf 2`
TEXT_RESET=`tput sgr0`

export LLVM_DIR=/home/morris1028/LLVM/llvm-3.8.0.src/build/bin
export PATH=$LLVM_DIR:$PATH
export LD_LIBRARY_PATH=/home/morris1028/LLVM/llvm-3.8.0.src/build/lib:$LD_LIBRARY_PATH

CLANG_ENGINE=clang-3.8
#CFLAGS=-m32
#LLCFLAGS=-march=x86

for i in call-lib case1;
do 
	echo "[INFO] compile ${TEXT_GREEN}${i}${TEXT_RESET}" 
	$CLANG_ENGINE $CFLAGS -fno-inline ${i}.c -S -emit-llvm -o ${i}.ll
	opt ${i}.ll -O2 -disable-inlining -o ${i}.1.ll
	llc ${i}.1.ll $LLCFLAGS -O3 -o ${i}.1.s
	$CLANG_ENGINE $CFLAGS ${i}.1.s -o ${i}.out
done
