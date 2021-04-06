# Connect4Solver

It's a parallelism-oriented project to solve Connect4 for any size.

## Features

- [Alpha-beta pruning](https://en.wikipedia.org/wiki/Alpha%E2%80%93beta_pruning)
- [Bitboard](https://en.wikipedia.org/wiki/Bitboard) implementation
- [Transposition Table](https://www.chessprogramming.org/Transposition_Table)
- [Iterative Deepening](https://www.chessprogramming.org/Iterative_Deepening) Alpha Beta Search 
<!-- - Anticipation of losing moves -->

## Build

Use the following command to build and run the executable target.

```
cmake -Bbuild
cmake --build build
./build/solver
```

To interpret the results :
```
+ : victory of the player 1
= : tie
- : victory of the player 2    
```

## References

- [John's Connect Four Playground](https://tromp.github.io/c4/c4.html)
- [Solving Connect 4: how to build a perfect AI](https://blog.gamesolver.org/)
- [Iterative Deepening Alpha Beta Search](https://gist.github.com/kartikkukreja/e58a77d6380f1af9b1f3)
- [Bitboards and Connect Four](https://github.com/denkspuren/BitboardC4/blob/master/BitboardDesign.md)