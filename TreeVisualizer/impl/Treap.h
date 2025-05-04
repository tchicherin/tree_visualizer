#ifndef TREAP_H
#define TREAP_H

#include <random>
#include <chrono>
#include <tuple>
#include "Visualization.h"

template <typename T>
class Treap : public VisualizableTree<T> {
 public:
  class Node {
    friend class Treap;
   public:
    T value;

    Node() = default;

    Node(T value_) : value(value_) {
      static std::mt19937_64 rng(std::chrono::steady_clock::now().time_since_epoch().count());
      priority_ = rng();
    }

   private:
    Node *left_ = nullptr, *right_ = nullptr;
    uint64_t priority_;
  };

  Treap() = default;

  std::pair<Node*, Node*> Split(Node *node, T key);

  Node* Merge(Node *a, Node *b);

  void Insert(T key) override;

  Node* FindNode(T key);

  bool Find(T key) override;

  void Erase(T key) override;

  VisualizationData* GetVisualizationData() override;

 private:
  Node *root_ = nullptr, *selected_ = nullptr;
};

#endif // TREAP_H
