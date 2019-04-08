#include "Node.hpp"

enum result { LESS, J2_WIN, TIE, J1_WIN, MORE, CONTINUE };

static NodeHistory list;

Node::Node() {}

Node::Node(Board &oldb, int &m) {
  b = oldb;
  b.makeMove(m);

  bool j1 = b.isWin(b.getBitboard(0));
  bool j2 = b.isWin(b.getBitboard(1));

  i.setIsTerminal(j1 || j2);
  i.setHeuristic((j1 ? J1_WIN : (j2 ? J2_WIN : TIE)));
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

int Node::initChilds() {
  /* Initialisation du tableau des coups &&
   * Prediction des mouvements perdants */
  int moves[W];

  int side = b.getCounter() & 1;
  int otherside = side ^ 1;
  uint64_t current = b.getBitboard(side), other = b.getBitboard(otherside),
           newboard;

  /* Valeur a retourner en cas de coup perdant pour le joueur courant */
  int res = (side == 0) ? J2_WIN : J1_WIN;

  int size_move = 0;
  /* On parcours les colonnes du puissance 4 */
  for (int i = 0; i < W; i++) {
    newboard = current | (1 << b.getHeight(i));
    if (!b.isLegal(newboard, i))
      continue;
    /* On joue un coup dans le tableau de l'adversaire */
    bool other_win = b.isLegalIsWin(other | (1 << b.getHeight(i)), i);
    /* On joue un coup dans le tableau de l'adversaire, mais cette fois ci a
     * la hauteur superieur */
    bool other_winontop = b.isLegalIsWin(other | (2 << b.getHeight(i)), i);
    /* Si le premier mouvement est gagnant (pour l'adversaire) */
    if (other_win) {
      /* Si le deuxieme mouvement est gagnant (toujours pour l'adversaire) */
      if (other_winontop) {
        /* Si le joueur actuelle joue donc dans cette colonne il fera
         * automatiquement gagner l'adversaire */
        return res;
      }
      /* Toujours si le mouvement, dans cette colonne, est gagant pour
       * l'adversaire
       * Nous allons forcer le joueur a jouer a cette colonne */
      size_move = 0;
      /* On stocke la colonne */
      moves[size_move++] = i;
      /* Pour toute les colonnes suivantes on regarde si l'adversaire peut
       * gagner sur une autre colonne */
      while (++i < W)
        if (b.isLegalIsWin(other | (1 << b.getHeight(i)), i)) {
          /* Si il peut alors dans tout les cas c'est perdu */
          return res;
        }
      /* Sinon on a trouve le coup qu'il faut jouer donc on peut sortir de la
       * boucle */
      break;
    }
    if (!other_winontop) {
      /* On stocke le coup */
      moves[size_move++] = i;
    }
  }

  /* Si la liste est vide */
  if (size_move == 0) {
    /* C'est a dire, aucun coup ne peut etre jouer, c'est perdu */
    return res;
  }

  /* Initialisation des fils en fonction des mouvements et
   * de l'historique des coups */
  for (int i = 0; i < size_move; i++) {
    Node *child = new Node(b, moves[i]);
    NodeHistory::const_iterator got = list.find(*child);
    if (got == list.end()) {
      list.insert(*child);
      childs[i] = child;
    } else {
      childs[i] = new Node(*got);
      list.erase(*child);
      list.insert(*childs[i]);
    }
  }
  return CONTINUE;
}

void Node::update(int value) {
  i.setHeuristic(value);
  i.setIsTerminal(true);
  list.erase(*this);
  list.insert(*this);
}
