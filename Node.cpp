#include "Node.hpp"

enum result { LESS = 0, LOSS = 1, TIE = 2, WIN = 3, MORE = 4, CONTINUE = 5 };

static NodeHistory list;

Node::Node() {}

Node::Node(Board &oldb, int &m) {
  b = oldb;
  b.makeMove(m);

  int side = b.getCounter() & 1;
  bool cp = b.isWin(b.getBitboard(side));
  bool op = b.isWin(b.getBitboard(side ^ 1));

  i.setIsTerminal(cp || op);
  i.setHeuristic((cp ? WIN : (op ? LOSS : TIE)));
}

Node::Node(const Node &n) {
  i = n.getInfo();
  b = n.getBoard();
  for (int i = 0; i < W; i++) {
    Node *child = n.getChild(i);
    if (child) {
      childs[i] = child;
    }
  }
}

bool Node::isLegalIsWin(uint64_t board, int i) {
  return b.isLegalIsWin(board | (1 << b.getHeight(i)), i);
}

int Node::initChilds() {
  /* Initialisation du tableau des coups &&
   * Prediction des mouvements perdants */
  int moves[W];

  int side = b.getCounter() & 1;
  int otherside = side ^ 1;

  uint64_t current, other;
  current = b.getBitboard(side);
  other = b.getBitboard(otherside);

  int size_move = 0;
  for (int i = 0; i < W; i++) {
    if (!b.isLegal(1 << b.getHeight(i), i))
      continue;
    if (b.isWin(current | (1 << b.getHeight(i))))
      return WIN;
    bool other_winontop = b.isLegalIsWin(other | (2 << b.getHeight(i)), i);
    if (isLegalIsWin(other, i)) {
      if (other_winontop) {
        return LOSS;
      }
      size_move = 0;
      moves[size_move++] = i;
      while (++i < W) {
        if (isLegalIsWin(other, i)) {
          return LOSS;
        }
      }
      break;
    }
    if (!other_winontop) {
      moves[size_move++] = i;
    }
  }

  if (size_move == 0) {
    return LOSS;
  }

  /* Initialisation des fils en fonction des mouvements et
   * de l'historique des coups */
  for (int i = 0; i < size_move; i++) {
    Node *child = new Node(b, moves[i]);
    NodeHistory::const_iterator got = list.find(*child);
    if (got == list.end()) {
      childs[i] = child;
    } else {
      childs[i] = new Node(*got);
    }
  }
  return CONTINUE;
}

void Node::update(int value) {
  i.setHeuristic(value);
  i.setIsTerminal(true);
  list.insert(*this);
}
