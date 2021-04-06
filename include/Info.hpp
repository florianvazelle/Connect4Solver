#pragma once

struct alignas(4) Info {
  Info() {
    isTerminal = false;
    heuristic = 0;
  }

  Info(const Info& i) {
    isTerminal = i.isTerminal;
    heuristic = i.heuristic;
  }

  int heuristic;
  bool isTerminal;
};
