#ifndef TREAP_IMPL
#define TREAP_IMPL

#include "Treap.h"

template <typename T>
std::pair<typename Treap<T>::Node*, typename Treap<T>::Node*> Treap<T>::Split(Node* node, T key) {
  if (node == nullptr) {
    return {nullptr, nullptr};
  }
  if (node->value >= key) {
    auto [L, R] = Split(node->left_, key);
    node->left_ = R;
    return {L, node};
  } else {
    auto [L, R] = Split(node->right_, key);
    node->right_ = L;
    return {node, R};
  }
}

template <typename T>
Treap<T>::Node* Treap<T>::Merge(Node *a, Node *b) {
  if (a == nullptr) {
    return b;
  }
  if (b == nullptr) {
    return a;
  }
  if (a->priority_ > b->priority_) {
    a->right_ = Merge(a->right_, b);
    return a;
  } else {
    b->left_ = Merge(a, b->left_);
    return b;
  }
}

template <typename T>
void Treap<T>::Insert(T key) {
  auto [L, R] = Split(root_, key);
  root_ = Merge(L, Merge(new Node(key), R));
}

template <typename T>
void Treap<T>::Erase(T key) {
  auto [L1, R1] = Split(root_, key);
  auto [L2, R2] = Split(R1, key + 1);
  delete L2;
  root_ = Merge(L1, R2); 
}

template <typename T>
Treap<T>::Node* Treap<T>::FindNode(T key) {
  Treap<T>::Node* current = root_;
  while (current != nullptr) {
    if (key < current->value) {
      current = current->left_;
    } else if (key == current->value) {
      selected_ = current;
      return current;
    } else {
      current = current->right_;
    }
  }
  return current;
}

template <typename T>
bool Treap<T>::Find(T key) {
  selected_ = FindNode(key);
  return selected_ != nullptr;
}

template <typename T>
VisualizationData* Treap<T>::GetVisualizationData() {
  auto DFS = [&](auto&& self, Node *node) -> VisualizationData* {
    if (node == nullptr) {
      return nullptr;
    }
    VisualizationData *data = new VisualizationData();
    data->keys.push_back(std::to_string(node->value));
    data->colors.push_back({QColor("#CDCDCE"), QColor(Qt::black)});
    data->children = {self(self, node->left_), self(self, node->right_)};
    return data;
  };
  VisualizationData* data = DFS(DFS, root_);
  selected_ = nullptr;
  return data;
}

#endif // TREAP_IMPL
