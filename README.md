## 編譯器、計算理論 題組系列 ##

涉及 regex, DFA, NFA, CFG, LL(1), SLR, turing machine。同構判定，模擬操作，無法簡單地使用遞迴分析字串的情況下 ...

* UVa 1671 - History of Languages
* UVa 1672 - Disjoint Regular Expressions
* UVa 10597 - Right Words
* UVa 134 - Loglan-A Logical Language
* UVa 171 - Car Trialling
* UVa 11711 - Turing

### UVa 12415 - Digit Patterns ###
讀入一個 regex，一個主字串 s，請問有不同的 i 滿足 s 的子字串 s[j...i] 符合 regex，套用 NFA 轉換成 DFA，可惜的是壓縮後狀態數還是太多，即使通過劉汝佳給的 small gift testdata，而且裡面倒數第二筆測資輸出有誤。當初學的時候就有這個疑問，果然狀態總數的增長非常大。

所謂的動態 NFA 轉換 DFA 指得到底是什麼？又被劉汝佳坑了一題 QQQQQQQQ