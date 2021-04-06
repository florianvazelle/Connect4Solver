#pragma once

#include <cstdlib>
#include <mutex>

#include "Board.hpp"
#include "Info.hpp"

enum result { LESS, J2_WIN, TIE, J1_WIN, MORE, CONTINUE };

#define ALIGN_NODE alignof(ALIGN_BOARD + 4 + (8 * W))

class alignas(ALIGN_NODE) Node {
 public:
  Node() : b(), i() {}

  Node(const Board& oldb, int col) : b(oldb, col), i() {
    bool j1 = b.isWin(0);
    bool j2 = b.isWin(1);
    bool tie = (b.getCounter() == (W * H));

    assert((b.getBitboard(0) & b.getBitboard(1)) == 0 && "On joue les un sur les autres ?!");

    i.isTerminal = (j1 || j2 || tie);

    int reward = (j1 ? J1_WIN : j2 ? J2_WIN : TIE);
    i.heuristic = reward;
  }

  Node(const Node& n) : b(n.getBoard()), i(n.getInfo()) {
    for (int i = 0; i < W; i++) {
      Node* child = n.getChild(i);
      if (child) {
        childs[i] = child;
      }
    }
  }

  Node operator=(const Node&) = delete;

  int initChilds();
  void update(int);

  const Board& getBoard() const { return b; }
  const Info& getInfo() const { return i; }
  Node* getChild(int i) const { return childs[i]; }

 private:
  Board b;
  Info i;
  Node* childs[W] = {nullptr};

  static std::mutex insert_mutex;
};

// #define RANDOM_64 ((static_cast<uint64_t>(std::rand()) << (sizeof(uint32_t) * 8)) | std::rand())

class Hash {
 public:
  // https://lemire.me/blog/2018/08/15/fast-strongly-universal-64-bit-hashing-everywhere/
  // const uint64_t a = RANDOM_64, b = RANDOM_64, c = RANDOM_64;  // randomly assigned 64-bit values

  // uint32_t hash32(uint64_t x) const {
  //   uint32_t low = (uint32_t)x;
  //   uint32_t high = (uint32_t)(x >> 32);
  //   return (uint32_t)((a * low + b * high + c) >> 32);
  // }

  uint64_t operator()(const Node* const n) const {
    const Board& b = n->getBoard();
    // return ((uint64_t)hash32(b.getBitboard(0))) | ((uint64_t)hash32(b.getBitboard(1)) << 32);
    return
        // position
        b.getBitboard(b.getCounter() & 1) +
        // mask
        (b.getBitboard(0) | b.getBitboard(1));
  }
};

struct Pred {
  bool operator()(const Node* const lhs, const Node* const rhs) const {
    return (lhs->getBoard().getBitboard(0) == rhs->getBoard().getBitboard(0) &&
            lhs->getBoard().getBitboard(1) == rhs->getBoard().getBitboard(1));
  }
};
