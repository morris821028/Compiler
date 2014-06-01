More
=====

[Blog 說明](http://morris821028.github.io/2014/05/12/uva-11291-with-yacc/)

Problem
=====

詳細題目請參考 [UVa 11291 - Smeech](http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=24&page=show_problem&problem=2266)

語法
```
<E[x]> -> <Integer> | (p <E[x]> <E[x]>)
<Integer> -> #number
```

目標計算出 `E[X] = p * (e1 + e2) + (1 - p) * (e1 - e2)`

Yacc
===== 

Yacc 主要是根據 CFG 進行解析。

其中，要對於 Yacc 語法的回傳值明白操作情況。

對於語法回傳值，我們來個最簡單的範例：
```
expression -> LEFT_P NUMBER expression expression RIGHT_P
    $$          $1     $2       $3         $4       $5
```
其中 `$$` 是回傳值，其中的 `$1`、`$2` ... 等，都是得到的回傳值。

對於 `yylval` 形態處理：
```
%union {
	float 	floatVal;
	int 	intVal;
}
```
通常使用 `union` 的方式來共用記憶體，在 lex 那邊就可以根據 `yylval.floatVal` 進行操作。

當然，我們也要對於每一個 `nonterminal` 定義回傳的型態

```
%type <floatVal> NUMBER
%type <floatVal> expression
%type <floatVal> startsymbol
```

基本上就大功告成了。

接著要考慮如何去完成多組測資組的問題，在這裡可以利用 kleene star 來完成。

但是對於輸出順序，使用 `input -> input startsymbol` 跟 `input -> startsymbol input` 輸出順序是不同的，如果要順著輸出，請選擇前者。
...