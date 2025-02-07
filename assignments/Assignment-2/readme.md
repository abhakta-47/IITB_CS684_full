## Q1. 
Complete the following table giving values of expressions in column 1 for the first 6 cycles. The values of integer flows for the first 5 cycles are given in rows, labelled P and Q.
| Expression\Cycle | 0  | 1  | 2  | 3  | 4  | 5  |
|------------------|----|----|----|----|----|----|
| P                | 13 | 9  | 10 | 5  | 15 | 3  |
| Q                | 0  | 12 | 34 | 7  | 28 | 1  |
| pre(Q)           |    |    |    |    |    |    |
| P + 1            |    |    |    |    |    |    |
| P -> Q           |    |    |    |    |    |    |
| P -> (pre(Q)+1)  |    |    |    |    |    |    |
| Q fby P          |    |    |    |    |    |    |
### Answer
| Expression\Cycle | 0  | 1  | 2  | 3  | 4  | 5  |
|------------------|----|----|----|----|----|----|
| P                | 13 | 9  | 10 | 5  | 15 | 3  |
| Q                | 0  | 12 | 34 | 7  | 28 | 1  |
| pre(Q)           | 0  | 0  | 12 | 34 |  7 | 28 |
| P + 1            | 14 | 10 | 11 | 6  | 16 |  4 |
| P -> Q           | 13 | 12 | 34 | 7  | 28 |  1 |
| P -> (pre(Q)+1)  | 13 | 1  | 13 | 35 |  8 | 29 |
| Q fby P          |  0 | 13 | 9  | 10 | 5  | 15 |

## Q2.
Consider the following Lustre nodes:
```lustre
node foo(P: int) returns (Q, R: int)
var W, Y, Z: int;
let
    Q  =  W* Z;
    W = 0 -> (pre(Z) +1);
    Z = W + Y;
    Y =  sqr(W);
    R = Q + W;
tel
```
```lustre
node sqr(A :int) returns (B: int)
let
    B = A*A;
tel
```
Is this program causally correct? What is the order in which values of variables Q,R,W,Y,Z are calculated in each cycle?
### Answer
Casual corrent: Yes
Order:
W, Y, Z, Q, R
```
W = 0 -> (pre(Z) +1);
Y =  sqr(W);
Z = W + Y;
Q  =  W* Z;
R = Q + W;
```
