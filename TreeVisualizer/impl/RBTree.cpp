#ifndef RBTREE_IMPL
#define RBTREE_IMPL

#include "RBTree.h"
#include <algorithm>

template <typename T>
void RBTree<T>::CutParent(Node* node) {
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
void RBTree<T>::LinkLeft(Node *node, Node *parent) {
  if (parent) {
    parent->left_ = node;
  }
  if (node) {
    node->parent_ = parent;
  }
}

template <typename T>
void RBTree<T>::LinkRight(Node *node, Node *parent) {
  if (parent) {
    parent->right_ = node;
  }
  if (node) {
    node->parent_ = parent;
  }
}

template <typename T>
bool RBTree<T>::IsLeft(Node *x) {
  return x && x->parent_ && x->parent_->left_ == x;
}

template <typename T>
bool RBTree<T>::IsRight(Node *x) {
  return x && x->parent_ && x->parent_->right_ == x;
}

template <typename T>
RBTree<T>::Node* RBTree<T>::RotateLeft(Node *x) {
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
RBTree<T>::Node* RBTree<T>::RotateRight(Node *x) {
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
RBTree<T>::Node* RBTree<T>::GetBrother(Node *x) {
  return IsLeft(x) ? x->parent_->right_ : x->parent_->left_;
}

template <typename T>
RBTree<T>::Node::Color RBTree<T>::GetColor(Node *x) {
  return x ? x->color_ : Node::kBlack;
}

template <typename T>
RBTree<T>::Node* RBTree<T>::FindNode(T value) {
  Node *current = root_;
  while (current) {
    if (value < current->value) {
      current = current->left_;
    } else if (value == current->value) {
      return current;
    } else {
      current = current->right_;
    }
  }
  return current;
}

template <typename T>
void RBTree<T>::Insert(T value) {
  Node *current = root_, *parent = nullptr;
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
  current = new Node(value);
  if (is_left) {
    LinkLeft(current, parent);
  } else {
    LinkRight(current, parent);
  }
  if (root_ == nullptr) {
    root_ = current;
  }
  RebalanceInsert(current);
}

template <typename T>
void RBTree<T>::Erase(Node *node) {
  if (node->left_) {
    Node* max_node = node->left_;
    while (max_node->right_) {
      max_node = max_node->right_;
    }
    std::swap(node->value, max_node->value);
    RebalanceErase(max_node);
    CutParent(max_node);
    delete max_node;
  } else if (node->right_) {
    Node* min_node = node->right_;
    while (min_node->left_) {
      min_node = min_node->left_;
    }
    std::swap(node->value, min_node->value);
    RebalanceErase(min_node);
    CutParent(min_node);
    delete min_node;
  } else {
    RebalanceErase(node);
    CutParent(node);
    if (node == root_) {
      root_ = nullptr;
    }
    delete node;
  }
}

template <typename T>
bool RBTree<T>::CheckInvariant() {
  auto DFS = [&](auto&& self, Node *node) -> int {
    if (node == nullptr) {
      return 0;
    } else if (node->parent_ && node->color_ == Node::kRed &&
               node->parent_->color_ == Node::kRed) {
      return -1;
    } else {
      int left_val = self(self, node->left_);
      int right_val = self(self, node->right_);
      if (node->left_ && node->left_->value >= node->value) {
        return -1;
      }
      if (node->right_ && node->right_->value <= node->value) {
        return -1;
      }
      if (left_val == -1 || right_val == -1) {
        return -1;
      }
      return left_val + (node->color_ == Node::kBlack);
    }
  };
  return DFS(DFS, root_) != -1;
}

template <typename T>
void RBTree<T>::RebalanceInsert(Node *node) {
  if (node == root_) {
    node->color_ = Node::kBlack;
  } else if (node->parent_->color_ == Node::kBlack) {
    return;
  } else {
    Node *p = node->parent_, *gp = p->parent_;
    Node *uncle = GetBrother(p);
    if (GetColor(uncle) == Node::kRed) {
      gp->color_ = Node::kRed;
      uncle->color_ = p->color_ = Node::kBlack;
      RebalanceInsert(gp);
    } else {
      if (IsLeft(p) == IsLeft(node)) {
        if (IsLeft(p)) {
          RotateRight(gp);
        } else {
          RotateLeft(gp);
        }
        p->color_ = Node::kBlack;
        gp->color_ = Node::kRed;
      } else {
        if (IsLeft(p)) {
          RotateLeft(p);
          RotateRight(gp);
        } else {
          RotateRight(p);
          RotateLeft(gp);
        }
        gp->color_ = Node::kRed;
        node->color_ = Node::kBlack;
      }
    }
  }
}

template <typename T>
void RBTree<T>::RebalanceErase(Node *node) {
  if (node->color_ != Node::kBlack || node->left_ || node->right_) {
    bool is_left = IsLeft(node);
    Node *child = node->left_ ? node->left_ : node->right_;
    if (child) {
      CutParent(child);
      if (is_left) {
        LinkLeft(child, node->parent_);
      } else {
        LinkRight(child, node->parent_);
      }
      child->color_ = Node::kBlack;
    }
  } else {
    while (node != root_) {
      Node *p = node->parent_;
      Node *b = GetBrother(node);
      Node *sl = b ? b->left_ : nullptr, *sr = b ? b->right_ : nullptr;
      auto SolveRed = [&](Node* cur) {
        p = cur->parent_, b = GetBrother(cur);
        sl = b ? b->left_ : nullptr, sr = b ? b->right_ : nullptr;
        if (GetColor(sl) == Node::kRed) {
          if (IsLeft(sl) == IsLeft(b)) {
            RotateRight(p);
            b->color_ = Node::kRed;
            sl->color_ = p->color_ = cur->color_ = Node::kBlack;
          } else { 
            RotateRight(b);
            RotateLeft(p);
            sl->color_ = Node::kRed;
            b->color_ = p->color_ = cur->color_ = Node::kBlack;
          }
        } else if (GetColor(sr) == Node::kRed) {
          if (IsLeft(sr) == IsLeft(b)) {
            RotateLeft(p);
            b->color_ = Node::kRed;
            sr->color_ = p->color_ = cur->color_ = Node::kBlack;
          } else {
            RotateLeft(b);
            RotateRight(p);
            sr->color_ = Node::kRed;
            b->color_ = p->color_ = cur->color_ = Node::kBlack;
          }
        } else {
          p->color_ = Node::kBlack;
          cur->color_ = Node::kRed;
          if (b) {
            b->color_ = Node::kRed;
          }
        }
      };
      if (GetColor(p) == Node::kRed) {
        SolveRed(node);
        break;
      } else {
        if (GetColor(b) == Node::kRed) {
          if (IsRight(node)) {
            RotateRight(p);
          } else {
            RotateLeft(p);
          }
          p->color_ = Node::kRed;
          b->color_ = node->color_ = Node::kBlack;
          SolveRed(node);
          break;
        } else if (GetColor(sl) == Node::kRed && IsLeft(sl) == IsLeft(b)) {
          RotateRight(p);
          sl->color_ = Node::kBlack;
          break;
        } else if (GetColor(sr) == Node::kRed && IsLeft(sr) == IsLeft(b)) {
          RotateLeft(p);
          sr->color_ = Node::kBlack;
          break;
        } else if (GetColor(sl) == Node::kRed && IsLeft(sl) != IsLeft(b)) {
          RotateRight(b);
          RotateLeft(p);
          sl->color_ = Node::kBlack;
          b->color_ = Node::kRed;
          break;
        } else if (GetColor(sr) == Node::kRed && IsLeft(sr) != IsLeft(b)) {
          RotateLeft(b);
          RotateRight(p);
          sr->color_ = Node::kBlack;
          b->color_ = Node::kRed; 
          break;
        } else {
          node = node->parent_;
        }
      }
    }
  }
}

template <typename T>
bool RBTree<T>::Find(T value) {
  selected_ = FindNode(value);
  return selected_ != nullptr;
}

template <typename T>
void RBTree<T>::Erase(T value) {
  Node *node = FindNode(value);
  if (node != nullptr) {
    Erase(node);
  }
}

template <typename T>
VisualizationData* RBTree<T>::GetVisualizationData() {
  auto DFS = [&](auto&& self, Node *node) -> VisualizationData* {
    if (node == nullptr) {
      return nullptr;
    }
    VisualizationData *data = new VisualizationData();
    data->keys.push_back(std::to_string(node->value));
    if (node == selected_) {
      data->colors.push_back({QColor(Qt::green), QColor(Qt::white)});
    } else if (node->color_ == Node::kRed) {
      data->colors.push_back({QColor(Qt::red), QColor(Qt::white)});
    } else {
      data->colors.push_back({QColor(Qt::black), QColor(Qt::white)});
    }
    data->children = {self(self, node->left_), self(self, node->right_)};
    return data; 
  };
  VisualizationData* data = DFS(DFS, root_);
  selected_ = nullptr;
  return data;
}

#endif // RBTREE_IMPL
