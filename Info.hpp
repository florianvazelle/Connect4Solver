class Info {
public:
  Info() {
    isTerminal_ = false;
    heuristic_ = 0;
  }

  bool getIsTerminal() const { return isTerminal_; }
  void setIsTerminal(bool i) { isTerminal_ = i; }

  int getHeuristic() const { return heuristic_; }
  void setHeuristic(int h) { heuristic_ = h; }

private:
  bool isTerminal_;
  int heuristic_;
};
