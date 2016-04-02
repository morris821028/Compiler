#!/bin/sh

make -C ../build -j12

#for i in case1 case2;
#do
#	echo "\033[35mTest ${i} \033[0m"
#	clang ${i}.c -DDEBUG -S -emit-llvm -o ${i}.bc
#	opt -mem2reg -S ${i}.bc -o ${i}-mem2reg.bc
#	opt -constprop -S ${i}-mem2reg.bc -o ${i}-constprop.bc
#	opt -simplifycfg -S ${i}-constprop.bc -o ${i}-simplifycfg.bc
#	opt -load ../build/MyDCE/libMyDCE.so -myMagicDCE -S ${i}-simplifycfg.bc -o ${i}.opt.ll
#	opt -adce -S ${i}-simplifycfg.bc -o ${i}.opt.adce.ll
#	
#	opt -load ../build/MyDCE/libMyDCE.so -myMagicDCE -S ${i}.bc -o ${i}.test.ll
#	opt -adce -S ${i}.bc -o ${i}.test.adce.ll
#	clang ${i}.opt.ll -o ${i}
#done
