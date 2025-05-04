#ifndef AVLTREE_H
#define AVLTREE_H

#include "Visualization.h"

template <typename T>
class AVLTree : public VisualizableTree<T> {
 public:
  class Node {
    friend class AVLTree;
   public:
    T value;

    Node() = default;
    
    Node(T value_) : value(value_) {}
    
   private:
    Node *left_ = nullptr, *right_ = nullptr;
    Node *parent_ = nullptr;
    int height_ = 1;
  };
 
  AVLTree() = default;

  void Insert(T value) override;

  void Erase(Node* node);
  void Erase(T value) override;

  Node* FindNode(T value);
  bool Find(T value) override;
 
  VisualizationData* GetVisualizationData() override;

  bool InvariantCheck();

 private:
  Node *root_ = nullptr, *selected_ = nullptr;

  int GetHeight(Node* node);
  void UpdateHeight(Node* node);

  Node* RotateLeft(Node *node);
  Node* RotateRight(Node *node);

  Node* Fix(Node *node);
};

#endif // AVLTREE_H
