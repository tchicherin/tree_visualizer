#ifndef RBTREE_H
#define RBTREE_H

#include "Visualization.h"

template <typename T>
class RBTree : public VisualizableTree<T> {
 public:
  class Node {
    friend class RBTree;
   public:
    enum Color {
      kRed,
      kBlack
    };

    T value;

    Node() = default;

    Node(T value_) : value(value_) {}

   private:
    Node *left_ = nullptr, *right_ = nullptr;
    Node *parent_ = nullptr;
    Color color_ = kRed;
  };

  RBTree() = default;

  VisualizationData* GetVisualizationData() override;

  void Insert(T value) override;
  
  Node* FindNode(T value);

  bool Find(T value) override;

  void Erase(Node *node);

  void Erase(T value) override;

  bool CheckInvariant();

 private:
  Node *root_ = nullptr, *selected_ = nullptr;

  void CutParent(Node *node);

  void LinkLeft(Node *node, Node *parent);

  void LinkRight(Node *node, Node *parent);

  bool IsLeft(Node *node);

  bool IsRight(Node *node);

  Node* RotateLeft(Node *node);

  Node* RotateRight(Node *node);

  Node* GetBrother(Node* node);

  Node::Color GetColor(Node *node);

  void RebalanceInsert(Node *node);

  void RebalanceErase(Node *node);
};

#endif // RBTREE_H
