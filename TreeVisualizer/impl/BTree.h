#ifndef BTREE_H
#define BTREE_H

#include "Visualization.h"
#include <vector>

template <typename T>
class BTree : public VisualizableTree<T> {
 public:
  int factor;

  BTree() : factor(2) {}

  BTree(int factor_) : factor(factor_) {}

  void Insert(T value) override;

  void Erase(T value) override;

  bool Find(T value) override;

  VisualizationData* GetVisualizationData() override;

 private: 
  struct Node {
    std::vector<T> keys;
    std::vector<Node*> children;

    bool IsLeaf();
  
    friend bool BTree<T>::Follow(Node *&node, T key);
  };

  Node *root_ = nullptr, *selected_ = nullptr;

  bool Follow(Node *&node, T key);

  void InsertInner(Node *node, T key);

  void EraseInner(Node *node, T key);

  Node* FixOversaturation(Node *node, Node *par);

  Node* FixUndersaturation(Node *node, Node *par);
};

#endif // BTREE_H
