#include <cstdint>
#include <iostream>
#include <memory>
#include <unordered_set>
#include <vector>

#ifndef W
#define W 4
#endif
#ifndef H
#define H 4
#endif

class Board {
public:
  Board();

  void makeMove(int col);
  bool isWin(uint64_t);
  bool isLegal(uint64_t, int);
  bool isLegalIsWin(uint64_t, int);
  uint64_t getTop(int);

  uint64_t getBitboard(int i) const { return bitboard[i]; };
  char getHeight(int col) const { return height[col]; };
  int getCounter() const { return counter; };

private:
  uint64_t bitboard[2];
  char height[W];
  int moves[W * H], counter;
};
