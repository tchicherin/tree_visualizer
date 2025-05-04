#ifndef AVLTREE_IMPL
#define AVLTREE_IMPL

#include "AVLTree.h"
#include <algorithm>
#include <cassert>

template <typename T>
AVLTree<T>::Node* AVLTree<T>::RotateLeft(Node *x) {
  Node *y = x->right_, *beta = y->left_;
  if (x->parent_) {
    if (x->parent_->left_ == x) {
      x->parent_->left_ = y; 
    } else {
      x->parent_->right_ = y;
    }
  }
  y->parent_ = x->parent_;
  y->left_ = x;
  x->parent_ = y;
  x->right_ = beta;
  if (beta) {
    beta->parent_ = x;
  }
  if (root_ == x) {
    root_ = y;
  }
  UpdateHeight(x), UpdateHeight(y);
  return y;
}

template <typename T>
AVLTree<T>::Node* AVLTree<T>::RotateRight(Node *x) {
  Node *y = x->left_, *beta = y->right_;
  if (x->parent_) {
    if (x->parent_->left_ == x) {
      x->parent_->left_ = y;
    } else {
      x->parent_->right_ = y;
    }
  }
  y->parent_ = x->parent_;
  y->right_ = x;
  x->parent_ = y;
  x->left_ = beta;
  if (beta) {
    beta->parent_ = x;
  }
  if (root_ == x) {
    root_ = y;
  }
  UpdateHeight(x), UpdateHeight(y);
  return y;
}

template <typename T>
int AVLTree<T>::GetHeight(Node *x) {
  return x ? x->height_ : 0;
}

template <typename T>
void AVLTree<T>::UpdateHeight(Node *x) {
  assert(x->parent_ != x);
  x->height_ = std::max(GetHeight(x->left_), GetHeight(x->right_)) + 1;
}

template <typename T>
AVLTree<T>::Node* AVLTree<T>::Fix(Node *x) {
  int diff_cur = GetHeight(x->left_) - GetHeight(x->right_);
  if (diff_cur < -1) {
    int diff_down = GetHeight(x->right_->left_) - GetHeight(x->right_->right_);
    if (diff_down > 0) {
      RotateRight(x->right_);
    }
    return RotateLeft(x);
  }
  if (diff_cur > +1) {
    int diff_down = GetHeight(x->left_->left_) - GetHeight(x->left_->right_);
    if (diff_down < 0) {
      RotateLeft(x->left_);
    }
    return RotateRight(x);
  }
  return x;
}

template <typename T>
AVLTree<T>::Node* AVLTree<T>::FindNode(T value) {
  Node *node = root_;
  while (node) {
    if (value < node->value) {
      node = node->left_;
    } else if (value == node->value) {
      return node;
    } else {
      node = node->right_;
    }
  }
  return node;
}

template <typename T>
void AVLTree<T>::Insert(T value) {
  if (root_) {
    Node *node = root_, *parent = nullptr;
    while (node) {
      if (value < node->value) {
        parent = node;
        node = node->left_;
      } else if (value == node->value) {
        return;
      } else {
        parent = node;
        node = node->right_;
      }
    }
    node = parent;
    if (value < node->value) {
      node->left_ = new Node(value);
      node->left_->parent_ = node;
    } else {
      node->right_ = new Node(value);
      node->right_->parent_ = node;
    }
    while (node) {
      UpdateHeight(node);
      node = Fix(node)->parent_;
    }
  } else {
    root_ = new Node(value);
  }
}

template <typename T>
void AVLTree<T>::Erase(Node* node) {
  if (!node->right_) {
    if (node->parent_) {
      Node* par = node->parent_;
      if (node->parent_->left_ == node) {
        node->parent_->left_ = node->left_;
        if (node->left_) {
          node->left_->parent_ = node->parent_;
        }
      } else {
        node->parent_->right_ = node->left_;
        if (node->left_) {
          node->left_->parent_ = node->parent_;
        }
      }
      delete node;
      while (par) {
        UpdateHeight(par);
        par = Fix(par)->parent_;
      }
    } else {
      root_ = node->left_;
      if (root_) {
        root_->parent_ = nullptr;
      }
      delete node;
    }
    return;
  }
  Node* min_node = node->right_;
  while (min_node->left_) {
    min_node = min_node->left_;
  }
  std::swap(min_node->value, node->value);
  node = min_node;
  if (node->parent_) {
    Node* par = node->parent_;
    if (node->parent_->left_ == node) {
      node->parent_->left_ = node->right_;
      if (node->right_) {
        node->right_->parent_ = node->parent_;
      }
    } else {
      node->parent_->right_ = node->right_;
      if (node->right_) {
        node->right_->parent_ = node->parent_;
      }
    }
    delete node;
    while (par) {
      UpdateHeight(par);
      par = Fix(par)->parent_;
    }
  } else {
    root_ = node->right_;
    if (root_) {
      root_->parent_ = nullptr;
    }
    delete node;
  }
}

template <typename T>
bool AVLTree<T>::InvariantCheck() {
  auto DFS = [&](auto&& self, Node *node) -> bool {
    if (node == nullptr) {
      return true;
    }
    if (std::abs(GetHeight(node->left_) - GetHeight(node->right_)) > 1) {
      return false;
    }
    return self(self, node->left_) && self(self, node->right_);
  };
  return DFS(DFS, root_);
}

template <typename T>
void AVLTree<T>::Erase(T value) {
  Node *node = FindNode(value);
  if (node != nullptr) {
    Erase(node);
  }
}

template <typename T>
bool AVLTree<T>::Find(T value) {
  selected_ = FindNode(value);
  return selected_ != nullptr;
}

template <typename T>
VisualizationData* AVLTree<T>::GetVisualizationData() {
  auto DFS = [&](auto&& self, Node *node) -> VisualizationData* {
    if (node == nullptr) {
      return nullptr;
    }
    VisualizationData *data = new VisualizationData();
    data->keys.push_back(std::to_string(node->value));
    if (node == selected_) {
      data->colors.push_back({QColor(Qt::green), QColor(Qt::white)});
    } else {
      data->colors.push_back({QColor("#CDCDCE"), QColor(Qt::black)});
    }
    data->children = {self(self, node->left_), self(self, node->right_)};
    return data;
  };
  VisualizationData* data = DFS(DFS, root_);
  selected_ = nullptr;
  return data;
}

#endif // AVLTREE_IMPL
