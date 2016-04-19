## Liveness ##

A variable is live out if used by a successor.

### Algorithm ###

```
// initialization
IN[exit] = 0
foreach B other than entry
	IN[B] = 0

// iterating until reaching fixed point
While some OUT[B] changes
	IN[B] = USE[B] union (OUT[B] - DEF[B])
	OUT[B] = UNION IN[S] where S in Successor(B)
```

### Example ###

```
+------------+
|            |
| a = b + c  | B1
|            |
+------------+
    |
    |
+---v--------+
|            |
| b = a - d  | B2
|            |
+------------+
    |
    |
+---v--------+
|            |
| c = b - c  | B3
|            |
+------------+
    |
    |
+---v--------+
|            |
| d = a - d  | B4
|            |
+------------+
```

### Sample Input 1 ###

*** in sample.in ***

### Sample Output 1 ###

*** in sample.out ***

### Example 2 ###

```
            +-------------+
            |             |
            |  a = b + c  | B1
            |  b = c + d  |
       +----+             +----+
       |    +-------------+    |
       |                       |
       |                       |
+------v------+         +------v------+
|             |         |             |
|  c = e + f  | B2      |  c = e + f  | B3
|             |         |             |
+------+------+         +------+------+
       |                       |
       |                       |
       |    +-------------+    |
       +---->             <----+
            |  g = x + y  |
            |             | B4
            +-------------+
```

### Sample Input 2 ###

*** in sample2.in ***

### Sample Output 2 ###

*** in sample2.out ***

### Reference ###

NTU Department of Computer Science and Information Engineering - Advanced Compiler
