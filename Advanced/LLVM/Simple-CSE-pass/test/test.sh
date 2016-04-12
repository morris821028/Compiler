#!/bin/sh

make -C ../build -j12

for i in case1 case2 case3 case4 case5;
do
	echo "\033[35mTest ${i} \033[0m"
	clang ${i}.c -DDEBUG -S -emit-llvm -o ${i}.bc
	opt -mem2reg -S ${i}.bc -o ${i}-mem2reg.bc
	opt -load ../build/MyCSE/libMyCSE.so -myMagicCSE -S ${i}-mem2reg.bc -o ${i}-cse.bc
	clang ${i}-mem2reg.bc -o ${i}-mem2reg
	clang ${i}-cse.bc -o ${i}-cse
done
