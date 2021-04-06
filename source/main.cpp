#include <future>

#include "Common.hpp"
#include "Node.hpp"
#include "ThreadPool.hpp"

NodeHistory list;
minipool<Node> pool(6000);

static int columnOrder[W];

#define FIRST_VISIT                                                                \
  /* Si pas terminal, et premier enfant à null alors c'est sa première visite */ \
  if (n->getChild(0) == nullptr) {                                                 \
    n->initChilds();                                                               \
  }

/* Define prototype methods */
int alphabeta(Node* const n, int depth, int alpha, int beta, bool maximizingPlayer);
int maxValue(Node* const n, int depth, int alpha, int beta);
int minValue(Node* const n, int depth, int alpha, int beta);

int maxValue(Node* const n, int depth, int alpha, int beta) {
  FIRST_VISIT

  Node* bestchild = nullptr;
  int value = LESS;
  for (unsigned int m = 0; m < W; m++) {
    Node* child = n->getChild(columnOrder[m]);
    if (child) {
      int maxvalue = alphabeta(child, depth - 1, alpha, beta, true);
      if (maxvalue > value) {
        value = maxvalue;
      }
      if (value > alpha) {
        alpha = value;
        bestchild = child;
      }
      if (alpha >= beta) break;
    }
  }
  if (bestchild && bestchild->getInfo().isTerminal) {
    // Si le meilleur enfant de l'exploration est terminal, alors le noeud courant devient terminal
    // aussi pour éviter de la reparcourir
    // ! On update pas tout les noeuds car on pourrait setter la profondeur max, pour éviter de tout
    // parcourir et le noeud ne serait pas réellement terminal
    n->update(value);
  }
  return value;
}

int minValue(Node* const n, int depth, int alpha, int beta) {
  FIRST_VISIT

  Node* bestchild = nullptr;
  int value = MORE;
  for (unsigned int m = 0; m < W; m++) {
    Node* child = n->getChild(columnOrder[m]);
    if (child) {
      int minvalue = alphabeta(child, depth - 1, alpha, beta, true);
      if (minvalue < value) {
        value = minvalue;
      }
      if (value < beta) {
        beta = value;
        bestchild = child;
      }
      if (alpha >= beta) break;
    }
  }
  if (bestchild && bestchild->getInfo().isTerminal) {
    // cf. maxValue comments
    n->update(value);
  }
  return value;
}

int alphabeta(Node* const n, int depth, int alpha, int beta, bool maximizingPlayer) {
  if (depth == 0 || n->getInfo().isTerminal) {
    return n->getInfo().heuristic;
  }

  if (maximizingPlayer) {
    return maxValue(n, depth, alpha, beta);
  } else {
    return minValue(n, depth, alpha, beta);
  }
}

int solve() {
  Node* n = pool.alloc();

  ThreadPool tp(1);  // std::thread::hardware_concurrency());
  std::vector<std::future<int>> futures(W);

  int depth = W * H;

  int alpha = LESS;
  // int beta = MORE;

  /* Ordering column to start exploration by the middle */
  for (int i = 0; i < W; i++) columnOrder[i] = W / 2 + (1 - 2 * (i % 2)) * (i + 1) / 2;

  /* For the iterative deepening, we start with an alpha to multi-thread the first childs */

  // FIRST_VISIT

  // for (int d = 1; d < depth; d++) {
  //   for (int m = 0; m < W; m++) {
  //     Node* child = n->getChild(columnOrder[m]);
  //     if (child) {
  //       futures[m] = tp.enqueue(alphabeta, child, d, LESS, MORE, false);
  //     }
  //   }

  //   int value = LESS;
  //   for (auto&& future : futures) {
  //     value = std::max(value, future.get());
  //     alpha = std::max(alpha, value);
  //     if (alpha >= beta) break;
  //   }
  // }

  alpha = alphabeta(n, depth, LESS, MORE, true);

  std::cout << "list.size=" << list.size() << "\n";

  return alpha;
}

int main() {
  int res = TIE;
  if (!(W < 4 && H < 4)) {
    res = solve();
  }
  std::cout << "<-=+>?"[res] << "\n";
  return 0;
}
