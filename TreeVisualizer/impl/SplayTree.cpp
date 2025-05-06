#ifndef SPLAYTREE_IMPL
#define SPLAYTREE_IMPL

#include "SplayTree.h"

template <typename T>
void SplayTree<T>::CutParent(Node* node) {
  if (node && node->parent_) {
    if (node->parent_->left_ == node) {
      node->parent_->left_ = nullptr;
    } else if (node->parent_->right_ == node) {
      node->parent_->right_ = nullptr;
    }
    node->parent_ = nullptr;
  }
}

template <typename T>
void SplayTree<T>::LinkLeft(Node *node, Node *parent) {
  if (parent) {
    parent->left_ = node;
  }
  if (node) {
    node->parent_ = parent;
  }
}

template <typename T>
void SplayTree<T>::LinkRight(Node *node, Node *parent) {
  if (parent) {
    parent->right_ = node;
  }
  if (node) {
    node->parent_ = parent;
  }
}

template <typename T>
bool SplayTree<T>::IsLeft(Node *x) {
  return x && x->parent_ && x->parent_->left_ == x;
}

template <typename T>
bool SplayTree<T>::IsRight(Node *x) {
  return x && x->parent_ && x->parent_->right_ == x;
}

template <typename T>
SplayTree<T>::Node* SplayTree<T>::RotateLeft(Node *x) {
  Node *y = x->right_, *beta = y->left_, *parent = x->parent_;
  bool is_left = IsLeft(x);
  CutParent(x), CutParent(y), CutParent(beta);
  if (is_left) {
    LinkLeft(y, parent);
  } else {
    LinkRight(y, parent);
  }
  LinkLeft(x, y), LinkRight(beta, x);
  if (root_ == x) {
    root_ = y;
  }
  return y;
}

template <typename T>
SplayTree<T>::Node* SplayTree<T>::RotateRight(Node *x) {
  Node *y = x->left_, *beta = y->right_, *parent = x->parent_;
  bool is_left = IsLeft(x);
  CutParent(x), CutParent(y), CutParent(beta);
  if (is_left) {
    LinkLeft(y, parent);
  } else {
    LinkRight(y, parent);
  }
  LinkRight(x, y), LinkLeft(beta, x);
  if (root_ == x) {
    root_ = y;
  }
  return y;
}

template <typename T>
void SplayTree<T>::Splay(Node *node) {
  while (node->parent_) {
    if (!node->parent_->parent_) {
      if (IsLeft(node)) {
        RotateRight(node->parent_);
      } else {
        RotateLeft(node->parent_);
      }
    } else {
      if (IsLeft(node)) {
        if (IsLeft(node->parent_)) {
          RotateRight(node->parent_->parent_);
          RotateRight(node->parent_);
        } else {
          RotateRight(node->parent_);
          RotateLeft(node->parent_);
        }
      } else {
        if (IsLeft(node->parent_)) {
          RotateLeft(node->parent_);
          RotateRight(node->parent_); 
        } else {
          RotateLeft(node->parent_->parent_);
          RotateLeft(node->parent_);
        }
      }
    }
  }
}

template <typename T>
void SplayTree<T>::Insert(T value) {
  Node *current = root_;
  Node *parent = nullptr;
  bool is_left = false;
  while (current) {
    if (value < current->value) {
      parent = current;
      current = current->left_;
      is_left = true;
    } else if (value == current->value) {
      return;
    } else {
      parent = current;
      current = current->right_;
      is_left = false;
    }
  }
  if (parent == nullptr) {
    root_ = new Node(value);
  } else {
    current = new Node(value);
    if (is_left) {
      LinkLeft(current, parent);
    } else {
      LinkRight(current, parent);
    }
    Splay(current);
  }
}

template <typename T>
SplayTree<T>::Node* SplayTree<T>::FindNode(T value) {
  Node *current = root_;
  while (current) {
    if (value < current->value) {
      current = current->left_;
    } else if (value == current->value) {
      Splay(current);
      return root_;
    } else {
      current = current->right_;
    }
  }
  return current;
}

template <typename T>
void SplayTree<T>::Erase(Node *node) {
  Splay(node);
  Node *left = node->left_, *right = node->right_;
  CutParent(node->left_);
  CutParent(node->right_);
  delete node;
  root_ = Merge(left, right);
}

template <typename T>
SplayTree<T>::Node* SplayTree<T>::Merge(Node *a, Node *b) {
  if (a == nullptr) {
    return b;
  }
  if (b == nullptr) {
    return a;
  }
  Node *max_node_a = a;
  while (max_node_a->right_) {
    max_node_a = max_node_a->right_;
  }
  Splay(max_node_a);
  LinkRight(b, max_node_a);
  return max_node_a;
}

template <typename T>
bool SplayTree<T>::Find(T value) {
  selected_ = FindNode(value);
  return selected_ != nullptr;
}

template <typename T>
void SplayTree<T>::Erase(T value) {
  Node *node = FindNode(value);
  if (node != nullptr) {
    Erase(node);
  }
}

template <typename T>
VisualizationData* SplayTree<T>::GetVisualizationData() {
  auto DFS = [&](auto&& self, Node *node) -> VisualizationData* {
    if (node == nullptr) {
      return nullptr;
    }
    VisualizationData *data = new VisualizationData();
    data->keys.push_back(std::to_string(node->value));
    if (node != selected_) {
      data->colors.push_back({QColor("#CDCDCE"), QColor(Qt::black)});
    } else {
      data->colors.push_back({QColor(Qt::green), QColor(Qt::white)});
    }
    data->children = {self(self, node->left_), self(self, node->right_)};
    return data;
  };
  VisualizationData* data = DFS(DFS, root_);
  selected_ = nullptr;
  return data;
}

#endif // SPLAYTREE_IMPL
