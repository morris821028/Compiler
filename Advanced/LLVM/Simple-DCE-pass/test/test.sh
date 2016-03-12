#!/bin/sh

make -C ../build -j12

for i in case1 case2 case3 case4;
do
	echo "\033[35mTest ${i} \033[0m"
	clang ${i}.c -S -emit-llvm -o ${i}.ll
	opt -load ../build/MyDCE/libMyDCE.so -myMagicDCE -S ${i}.ll -o ${i}.opt.ll
	opt -adce -S ${i}.ll -o ${i}.opt.adce.ll
	clang ${i}.opt.ll -o ${i}
done


