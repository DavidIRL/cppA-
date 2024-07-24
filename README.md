By targetting a **file** containing a matrix of int s,
A* can find **(if possible)** the shortest path from ~start~ to ~goal~.


Example:

```
0,1,0,0,0,0,
0,1,0,0,0,0,
0,1,0,0,0,0,
0,1,0,0,0,0,
0,0,0,0,1,0,
```

^This^ will output

```
🚦   ⛰️    0    0    0    0   
🚗   ⛰️    0    0    0    0
🚗   ⛰️    0    0    0    0   
🚗   ⛰️    0   🚗   🚗   🚗   
🚗   🚗   🚗   🚗   ⛰️   🏁
```

^This^










