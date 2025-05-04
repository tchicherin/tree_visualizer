#ifndef SPLAYTREE_H
#define SPLAYTREE_H

#include <tuple>
#include "Visualization.h"

template <typename T>
class SplayTree : public VisualizableTree<T> {
 public:
  class Node {
    friend class SplayTree;
   public:
    T value;

    Node() = default;

    Node(T value_) : value(value_) {}

   private:
    Node *left_ = nullptr, *right_ = nullptr;
    Node *parent_ = nullptr;
  };

  Node* Merge(Node *a, Node *b);

  void Insert(T value) override;

  Node* FindNode(T value);
  bool Find(T value) override;

  void Erase(Node *node);
  void Erase(T value) override;

  VisualizationData* GetVisualizationData() override;

 private:
  Node *root_ = nullptr, *selected_ = nullptr;

  void CutParent(Node *node);

  void LinkLeft(Node *node, Node *parent);

  void LinkRight(Node *node, Node *parent);

  SplayTree<T>::Node* RotateLeft(Node *node);

  SplayTree<T>::Node* RotateRight(Node *node);

  void Splay(Node *node);

  bool IsLeft(Node *node);

  bool IsRight(Node *node);
};

#endif // SPLAYTREE_H
