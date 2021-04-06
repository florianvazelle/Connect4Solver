#include "Node.hpp"

#include "Common.hpp"

std::mutex Node::insert_mutex;

/**
 * @brief Initialisation du tableau des coups && Prediction des mouvements perdants
 */
int Node::initChilds() {
  static int moves[W];

  // int side = b.getCounter() & 1;
  // int otherside = side ^ 1;
  // const uint64_t current = b.getBitboard(side);  //, other = b.getBitboard(otherside);

  /* Valeur a retourner en cas de coup perdant pour le joueur courant */
  // int res = (side == 0) ? J2_WIN : J1_WIN;

  int size_move = 0;
  /* On parcours les colonnes du puissance 4 */
  for (int i = 0; i < W; i++) {
    // const uint64_t newboard = current | (1 << b.getHeight(i));
    if (!b.isLegal(i)) continue;
    //   /* On joue un coup dans le tableau de l'adversaire */
    //   bool other_win = b.isLegalIsWin(other | (1 << b.getHeight(i)), i);
    //   /* On joue un coup dans le tableau de l'adversaire, mais cette fois ci a
    //    * la hauteur superieur */
    //   bool other_winontop = b.isLegalIsWin(other | (2 << b.getHeight(i)), i);
    //   /* Si le premier mouvement est gagnant (pour l'adversaire) */
    //   if (other_win) {
    //     /* Si le deuxieme mouvement est gagnant (toujours pour l'adversaire) */
    //     if (other_winontop) {
    //       /* Si le joueur actuelle joue donc dans cette colonne il fera
    //        * automatiquement gagner l'adversaire */
    //       return res;
    //     }
    //     /* Toujours si le mouvement, dans cette colonne, est gagant pour
    //      * l'adversaire
    //      * Nous allons forcer le joueur a jouer a cette colonne */
    //     size_move = 0;
    //     /* On stocke la colonne */
    //     moves[size_move++] = i;
    //     /* Pour toute les colonnes suivantes on regarde si l'adversaire peut
    //      * gagner sur une autre colonne */
    //     while (++i < W)
    //       if (b.isLegalIsWin(other | (1 << b.getHeight(i)), i)) {
    //         /* Si il peut alors dans tout les cas c'est perdu */
    //         return res;
    //       }
    //     /* Sinon on a trouve le coup qu'il faut jouer donc on peut sortir de la
    //      * boucle */
    //     break;
    //   }
    //   if (!other_winontop) {
    //     /* On stocke le coup */
    moves[size_move++] = i;
    //   }
  }

  assert(size_move != 0 && "Aucun coup possible! Le Node aurait du être considéré comme terminal.");

  {
    std::unique_lock<std::mutex> lock(insert_mutex);

    /* Initialisation des fils en fonction des mouvements et
     * de l'historique des coups */
    for (int i = 0; i < size_move; i++) {
      Node* child = pool.alloc(b, moves[i]);
      /* On regarde si le Node n'a pas déjà été créé par une autre exploration de l'arbre */
      NodeHistory::const_iterator got = list.find(child);
      if (got == list.end()) {
        list.insert(child);
        childs[i] = child;
        // std::cout << "list.size=" << list.size() << "\n";
      } else {
        childs[i] = *got;
        pool.free(child);
      }
    }
  }
  return CONTINUE;
}

void Node::update(int value) {
  i.heuristic = value;
  i.isTerminal = true;
}
