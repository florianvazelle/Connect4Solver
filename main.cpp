#include <future>

#include "Node.hpp"

enum result { LESS, J2_WIN, TIE, J1_WIN, MORE, CONTINUE };
static int columnOrder[W];

int alphabeta_thread(Node *, int, int, int, bool);

int max_ab_thread(Node *n, int depth, int alpha, int beta) {
  int value = LESS;
  std::vector<std::future<int>> futures;
  for (int m = 0; m < W; m++) {
    Node *child = n->getChild(columnOrder[m]);
    if (child) {
      futures.push_back(
          std::async(alphabeta_thread, child, depth - 1, alpha, beta, false));
    }
  }

  for (auto &&fut : futures) {
    value = std::max(value, fut.get());
    alpha = std::max(alpha, value);
    if (alpha >= beta) {
      break;
    }
  }
  n->update(value);
  return value;
}

int min_ab_thread(Node *n, int depth, int alpha, int beta) {
  int value = MORE;
  std::vector<std::future<int>> futures;
  for (int m = 0; m < W; m++) {
    Node *child = n->getChild(columnOrder[m]);
    if (child) {
      futures.push_back(
          std::async(alphabeta_thread, child, depth - 1, alpha, beta, true));
    }
  }

  for (auto &&fut : futures) {
    value = std::min(value, fut.get());
    alpha = std::min(alpha, value);
    if (alpha >= beta) {
      break;
    }
  }
  n->update(value);
  return value;
}

int alphabeta(Node *n, int depth, int alpha, int beta, bool maximizingPlayer) {
  if (depth == 0 || n->getInfo().getIsTerminal()) {
    return n->getInfo().getHeuristic();
  }

  int res = n->initChilds();
  if (res != CONTINUE) {
    n->update(res);
    return res;
  }

  if (maximizingPlayer) {
    int value = LESS;
    for (int m = 0; m < W; m++) {
      Node *child = n->getChild(columnOrder[m]);
      if (child) {
        value =
            std::max(value, alphabeta(child, depth - 1, alpha, beta, false));
        alpha = std::max(alpha, value);
        if (alpha >= beta)
          break;
      }
    }
    n->update(value);
    return value;
  } else {
    int value = MORE;
    for (int m = 0; m < W; m++) {
      Node *child = n->getChild(columnOrder[m]);
      if (child) {
        value = std::min(value, alphabeta(child, depth - 1, alpha, beta, true));
        beta = std::min(beta, value);
        if (alpha >= beta)
          break;
      }
    }
    n->update(value);
    return value;
  }
}

int alphabeta_thread(Node *n, int depth, int alpha, int beta,
                     bool maximizingPlayer) {
  if (depth <= (W * H) - 1) {
    return alphabeta(n, depth, alpha, beta, maximizingPlayer);
  } else {
    if (depth == 0 || n->getInfo().getIsTerminal()) {
      return n->getInfo().getHeuristic();
    }

    int res = n->initChilds();
    if (res != CONTINUE) {
      n->update(res);
      return res;
    }

    if (maximizingPlayer) {
      return max_ab_thread(n, depth, alpha, beta);
    } else {
      return min_ab_thread(n, depth, alpha, beta);
    }
  }
}

int solve(bool thread) {
  Node *origin = new Node();
  int depth = W * H;
  int alpha = LESS;
  int beta = MORE;

  for (int i = 0; i < W; i++)
    columnOrder[i] = W / 2 + (1 - 2 * (i % 2)) * (i + 1) / 2;

  if (thread) {
    return alphabeta_thread(origin, depth, alpha, beta, true);
  } else {
    return alphabeta(origin, depth, alpha, beta, true);
  }
}

int main() {
  int res = TIE;
  if (!(W < 4 && H < 4)) {
    res = solve(true);
  }
  std::cout << "<-=+>"[res] << "\n";
  return 0;
}
