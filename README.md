# Connect4Solver
A parallelisme oriented project with :
- Alpha-beta pruning
- Bitboard implementation
- Transposition Table
- Anticipation of losing moves

## Use
    $> make
    g++ -o exe -W -Wall -O3 -std=c++14 -pthread Board.cpp Node.cpp main.cpp -DW=4 -DH=5
    $> make run
    =
    $> make clean
## Results

    + : victory of the player 1
    = : tie
    - : victory of the player 2    
