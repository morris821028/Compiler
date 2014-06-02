## Problem ##

Loglan is a synthetic speakable language designed to test some of the fundamental problems of linguistics, such as the Sapir Whorf hypothesis. It is syntactically unambiguous, culturally neutral and metaphysically parsimonious. What follows is a gross over-simplification of an already very small grammar of some 200 rules.

Loglan sentences consist of a series of words and names, separated by spaces, and are terminated by a period (.). Loglan words all end with a vowel; names, which are derived extra-linguistically, end with a consonant. Loglan words are divided into two classes--little words which specify the structure of a sentence, and predicates which have the form CCVCV or CVCCV where C represents a consonant and V represents a vowel (see examples later).

The subset of Loglan that we are considering uses the following grammar:

```
A             →  a | e | i | o | u
MOD           →  ga | ge | gi | go | gu
BA            →  ba | be | bi | bo | bu
DA            →  da | de | di | do | du
LA            →  la | le | li | lo | lu
NAM           →  {all names}
PREDA         →  {all predicates}
<sentence>    →  <statement> | <predclaim>
<predclaim>   →  <predname> BA <preds> | DA <preds>
<preds>       →  <predstring> | <preds> A <predstring>
<predname>    →  LA <predstring> | NAM
<predstring>  →  PREDA | <predstring> PREDA
<statement>   →  <predname> <verbpred> <predname> | <predname> <verbpred>
<verbpred>    →  MOD <predstring>
```
Write a program that will read a succession of strings and determine whether or not they are correctly formed Loglan sentences.

## Input and Output ##

Each Loglan sentence will start on a new line and will be terminated by a period (.). The sentence may occupy more than one line and words may be separated by more than one whitespace character. The input will be terminated by a line containing a single `#'. You can assume that all words will be correctly formed.

Output will consist of one line for each sentence containing either `Good' or `Bad!'.

## Sample input ##
```
la mutce bunbo mrenu bi ditca.
la fumna bi le mrenu.
djan ga vedma le negro ketpi.
#
```

## Sample output ##
```
Good
Bad!
Good
```
## Solution ##

其實這一題有比較好的解法，但是在正在學編譯器，因此把 LL(1) parser 運用在此題
，所以代碼會稍微長一點。

```
<sentence>    ->  <common_pre>
<sentence>    ->  DA <preds>
<common_pre>  ->  <predname> <suffix>
<suffix>      ->  <predclaim>
<suffix>      ->  <statement>
<predclaim>   ->  BA <preds>
<preds>       ->  <preds_head> <preds_tail>
<preds_head>  ->  <predstring>
<preds_tail>  ->  A <predstring> <preds_tail>
<preds_tail>  ->  l
<predname>    ->  LA <predstring>
<predname>    ->  NAM
<predstring>  ->  PREDA <predstr_tail>
<predstr_tail> -> PREDA <predstr_tail>
<predstr_tail> -> l
<statement>   ->  <verbpred> <statement_s>
<statement_s> ->  <predname>
<statement_s> ->  l
<verbpred>    ->  MOD <predstring>
```

為了要符合 LL(1) 的形式，要想辦法將 Grammar 符合規格，也就是每一條 production rule 的 predict set 不會有模稜兩個的情況，也就是說當我看到某個 non-terminal 在 stack 上，在去看下一個 token，然後可以對於 (non-terminal, token) 只找到一條規則去匹配。

predict set 是根據每一條 production rule 所產生的，也就是說這個推論可以產生的第一個 token 是什麼，假使有相同的前綴時，則必須拉出做調整。// 因為這將會造成模稜兩可的情況。

遇到
```
A -> BCx
A -> BCy
```
應調整成
```
A -> BCW
W -> x
W -> y
```

在 parsing 問題上，應避免替換的無限迴圈。
遇到
```
A -> AC
A -> X
A -> Y
```
應調整成
```
A -> WD
D -> CD
D -> lambda
W -> X
W -> Y
```

發現對照課本的 parsing function 撰寫起來才發現有些不完善的地方，對於 `lldriver()` 會沒有辦法應對輸入結束，要吐出 lambda 的問題。
