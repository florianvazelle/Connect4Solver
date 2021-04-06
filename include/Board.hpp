#pragma once

#include <bitset>
#include <cassert>
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

#define ALIGN_BOARD alignof((64 * 2) + W + (4 * W * H + 4))

class alignas(ALIGN_BOARD) Board {
 public:
  Board() : counter(0) {
    for (int i = 0; i < W; i++) {
      height[i] = i * H;
    }
    bitboard[0] = 0;
    bitboard[1] = 0;
  }

  Board(const Board& b) : counter(b.getCounter()) {
    for (int i = 0; i < W; i++) {
      height[i] = b.height[i];
    }
    bitboard[0] = b.bitboard[0];
    bitboard[1] = b.bitboard[1];
  }

  Board(const Board& b, int col) : Board(b) { makeMove(col); }

  void makeMove(int col) {
    assert(col < W);
    assert(((int)height[col]) < ((H * (col + 1))) && "Hauteur max atteinte pour cette colonne!");
    const uint64_t& move = 1 << height[col]++;
    bitboard[counter & 1] ^= move;
    moves[counter++] = col;
  }

  /**
   * @brief Test si c'est un plateau gagnant
   */
  bool isWin(const uint64_t& newboard) const {
    static const int directions[4] = {1, H, H + 1, H + 2};
    for (const int& direction : directions) {
      const uint64_t& bb = (newboard & (newboard >> direction));
      if ((bb & (bb >> (2 * direction))) != 0) return true;
    }
    return false;
  }

  /**
   * @brief Test si un coup est possible
   */
  inline bool isLegal(int col) const {
    const uint64_t& TOP = ((1 << H) << col * H);
    return ((TOP & (1 << height[col])) == 0);
  }

  /**
   * @brief Test si c'est un coup possible et si c'est un coup gagnant
   */
  inline bool isLegalIsWin(const uint64_t& newboard, int col) const {
    return (isLegal(col) && isWin(newboard));
  }

  inline bool isWin(int i) const { return isWin(bitboard[i]); }

  inline const uint64_t& getBitboard(int i) const { return bitboard[i]; };
  inline const char& getHeight(int col) const { return height[col]; };
  inline const int& getCounter() const { return counter; };

 private:
  uint64_t bitboard[2];       // 64 * 2
  char height[W];             // 1 * W
  int moves[W * H], counter;  // 4 * W * H + 4
};
