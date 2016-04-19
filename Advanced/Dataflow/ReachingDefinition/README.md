## Reaching Definition ##

### Algorithm ###

```
// initialization
OUT[entry] = 0
foreach B other than entry
	OUT[B] = 0

// iterating until reaching fixed point
While some OUT[B] changes
	OUT[B] = GEN[B] union (IN[B] - KILL[B])
	IN[B] = UNION OUT[P] where P in Pred(B)
```

### Example ###

### Sample Input ###

<pre>```
Entry -> B1
B1 -> B2
B2 -> B3
B2 -> B4
B3 -> B4
B4 -> B2
B4 -> Exit
```
```
B1
d1 i m
d2 j n
d3 a u1
B2
d4 i i
d5 j j
B3
d6 a u2
B4
d7 i u3
```</pre>

### Sample Output ###

### Reference ###

NTU Department of Computer Science and Information Engineering - Advanced Compiler