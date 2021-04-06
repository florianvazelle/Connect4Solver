#pragma once

#include "Node.hpp"
#include "Pool.hpp"

typedef std::unordered_set<Node*, Hash, Pred> NodeHistory;

extern NodeHistory list;
extern minipool<Node> pool;