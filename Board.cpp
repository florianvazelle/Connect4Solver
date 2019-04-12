#include "Board.hpp"

Board::Board() {
  counter = 0;
  for (int i = 0; i < W; i++)
    height[i] = i * (H + 1);
  bitboard[0] = 0;
  bitboard[1] = 0;
}

void Board::makeMove(int col) { bitboard[counter++ & 1] ^= 1 << height[col]++; }

bool Board::isWin(uint64_t newboard) {
  int directions[4] = {1, H + 1, H, H + 2};
  uint64_t bb;
  for (int direction : directions) {
    bb = newboard & (newboard >> direction);
    if ((bb & (bb >> (2 * direction))) != 0)
      return true;
  }
  return false;
}

bool Board::isLegal(uint64_t newboard, int col) {
  return !(newboard & getTop(col));
}

bool Board::isLegalIsWin(uint64_t newboard, int col) {
  return (isLegal(newboard, col) && isWin(newboard));
}

uint64_t Board::getTop(int col) { return (1 << H) << col * (H + 1); }
