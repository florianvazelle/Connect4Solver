#include "Board.hpp"
#include "Info.hpp"

class Node {
public:
  Node();
  Node(Board &, int &);
  Node(const Node &);

  int initChilds();
  void update(int);

  Board getBoard() const { return b; }
  Info getInfo() const { return i; }
  auto getChild(int i) const { return childs[i]; }

  bool operator==(const Node &n) const {
    return (b.getBitboard(0) == n.getBoard().getBitboard(0) &&
            b.getBitboard(1) == n.getBoard().getBitboard(1));
  }

private:
  Board b;
  Info i;
  Node *childs[W] = {nullptr};
};

class Hash {
public:
  uint64_t operator()(const Node &n) const {
    Board b = n.getBoard();
    return
        // position
        b.getBitboard(b.getCounter() & 1) +
        // mask
        (b.getBitboard(0) | b.getBitboard(1));
  }
};

typedef std::unordered_set<Node, Hash> NodeHistory;
