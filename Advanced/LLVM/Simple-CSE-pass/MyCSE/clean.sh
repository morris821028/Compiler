
for i in case1 case2 case3 case4;
do
	rm -f ${i} ${i}.opt.ll ${i}.ll ${i}.opt.adce.ll 
	rm -f ${i}-mem2reg.bc ${i}-constprop.bc ${i}-simplifycfg.bc ${i}.bc
done;
