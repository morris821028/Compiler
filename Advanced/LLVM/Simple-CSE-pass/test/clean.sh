
for i in case1 case2 case3 case4 case5;
do
	rm -f ${i} ${i}.opt.ll ${i}.ll ${i}.opt.adce.ll 
	rm -f *.bc
	rm -f ${i}-mem2reg.bc ${i}-constprop.bc ${i}-simplifycfg.bc ${i}.bc
	rm -f ${i} ${i}-cse ${i}-mem2reg
done;
