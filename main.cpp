#include "Node.hpp"

enum result { LESS = 0, LOSS = 1, TIE = 2, WIN = 3, MORE = 4, CONTINUE = 5 };
static int columnOrder[W];

int alphabeta(Node *n, int depth, int alpha, int beta) {
  if (depth == 0 || n->getInfo().getIsTerminal()) {
    return n->getInfo().getHeuristic();
  }

  int res = n->initChilds();
  if (res != CONTINUE) {
    n->update(res);
    return res;
  }

  int value = LESS;
  for (int m = 0; m < W; m++) {
    Node *child = n->getChild(columnOrder[m]);
    if (child) {
      value = std::max(
          value, MORE - alphabeta(child, depth - 1, MORE - beta, MORE - alpha));
      alpha = std::max(alpha, value);
      if (alpha >= beta)
        break;
    }
  }
  n->update(value);
  return value;
}

int solve() {
  Node *origin = new Node();
  int depth = W * H;
  int alpha = LESS;
  int beta = MORE;

  for (int i = 0; i < W; i++)
    columnOrder[i] = W / 2 + (1 - 2 * (i % 2)) * (i + 1) / 2;

  return alphabeta(origin, depth, alpha, beta);
}

int main() {
  int res = TIE;
  if (!(W < 4 && H < 4)) {
    res = solve();
  }
  std::cout << "<-=+>"[res] << "\n";
  return 0;
}
