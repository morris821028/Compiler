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

<pre>```
Entry -> B1
B1 -> B2
B2 -> B3
B3 -> B4
B4 -> Exit
```
```
B1
+ a b c
B2
- b a d
B3
- c b c
B4
- d a d
```</pre>

### Sample Output 1 ###

```
=== Basic Block Information ===
        --- Entry ---
        --- Exit ---
        --- B1 ---
                [0] a = + b c
        --- B2 ---
                [1] b = - a d
        --- B3 ---
                [2] c = - b c
        --- B4 ---
                [3] d = - a d
===

=== Basic Block Available Expressions ===
        --- Entry ---
                GEN :
                KILL:
                IN  :
                OUT :
        --- Exit ---
                GEN :
                KILL:
                IN  :
                        [2] c = - b c
                        [3] d = - a d
                OUT :
                        [2] c = - b c
                        [3] d = - a d
        --- B1 ---
                GEN :
                        [0] a = + b c
                KILL:
                        [1] b = - a d
                        [3] d = - a d
                IN  :
                OUT :
                        [0] a = + b c
        --- B2 ---
                GEN :
                        [1] b = - a d
                KILL:
                        [0] a = + b c
                        [2] c = - b c
                IN  :
                        [0] a = + b c
                OUT :
                        [1] b = - a d
        --- B3 ---
                GEN :
                        [2] c = - b c
                KILL:
                        [0] a = + b c
                IN  :
                        [1] b = - a d
                OUT :
                        [1] b = - a d
                        [2] c = - b c
        --- B4 ---
                GEN :
                        [3] d = - a d
                KILL:
                        [1] b = - a d
                IN  :
                        [1] b = - a d
                        [2] c = - b c
                OUT :
                        [2] c = - b c
                        [3] d = - a d
===
```

### Sample Input 2 ###

<pre>```
Entry -> B:1
B1 -> B2
B1 -> B3
B2 -> B4
B3 -> B4
B4 -> Exit
```
```
B1
+ a b c
+ b a d
B2
+ c e f
B3
+ c e f
B4
+ g x y
```
</pre>

### Sample Output 2 ###

```
=== Basic Block Information ===
        --- Entry ---
        --- Exit ---
        --- B1 ---
                [0] a = + b c
                [1] b = + a d
        --- B2 ---
                [2] c = + e f
        --- B3 ---
                [3] c = + e f
        --- B4 ---
                [4] g = + x y
===

=== Basic Block Available Expressions ===
        --- Entry ---
                GEN :
                KILL:
                IN  :
                OUT :
        --- Exit ---
                GEN :
                KILL:
                IN  :
                        [4] g = + x y
                OUT :
                        [4] g = + x y
        --- B1 ---
                GEN :
                        [0] a = + b c
                KILL:
                        [1] b = + a d
                IN  :
                OUT :
                        [0] a = + b c
        --- B2 ---
                GEN :
                        [2] c = + e f
                KILL:
                        [0] a = + b c
                IN  :
                        [0] a = + b c
                OUT :
                        [2] c = + e f
        --- B3 ---
                GEN :
                        [3] c = + e f
                KILL:
                        [0] a = + b c
                IN  :
                        [0] a = + b c
                OUT :
                        [3] c = + e f
        --- B4 ---
                GEN :
                        [4] g = + x y
                KILL:
                IN  :
                OUT :
                        [4] g = + x y
===
```

### Reference ###

NTU Department of Computer Science and Information Engineering - Advanced Compiler
