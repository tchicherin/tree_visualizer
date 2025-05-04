#ifndef BTREE_IMPL
#define BTREE_IMPL

#include "BTree.h"
#include <algorithm>
#include <cassert>

template <typename T>
bool BTree<T>::Node::IsLeaf() {
  return children[0] == nullptr;
}

template <typename T>
bool BTree<T>::Follow(Node *&node, T key) {
  auto iter = std::lower_bound(node->keys.begin(), node->keys.end(), key);
  if (iter != node->keys.end() && *iter == key) {
    return false;
  }
  node = node->children[iter - node->keys.begin()];
  return true;
}

template <typename T>
void BTree<T>::Insert(T value) {
  if (root_ == nullptr) {
    root_ = new Node();
    root_->keys.push_back(value);
    root_->children = {nullptr, nullptr};
  }
  Node *cur = root_, *tmp = nullptr;
  while (cur != nullptr) {
    cur = FixOversaturation(cur, tmp);
    tmp = cur;
    if (!Follow(cur, value)) {
      return;
    }
  }
  cur = tmp;
  InsertInner(cur, value);
}

template <typename T>
void BTree<T>::Erase(T value) {
  if (root_ == nullptr) {
    return;
  }
  Node *cur = root_, *tmp = nullptr;
  while (cur != nullptr) {
    cur = FixUndersaturation(cur, tmp);
    tmp = cur;
    if (cur->IsLeaf()) {
      EraseInner(cur, value);
      return;
    }
    if (!Follow(cur, value)) {
      // Find the needed iterator and the right child
      auto iter = std::lower_bound(cur->keys.begin(), cur->keys.end(), value);
      int pos = std::distance(cur->keys.begin(), iter);
      Node *right_ch = cur->children[pos + 1];
      assert(right_ch != nullptr);

      // Swap with the minimum from the right child
      Node *min_node = right_ch;
      while (!min_node->IsLeaf()) {
        min_node = min_node->children.front();
      }
      T last_min = min_node->keys.front();
      std::swap(cur->keys[pos], min_node->keys.front());
      
      // Go to the minimum fixing undersaturation
      while (!right_ch->IsLeaf()) {
        Node *where = right_ch;
        Follow(where, last_min);
        right_ch = FixUndersaturation(right_ch, tmp);
        tmp = right_ch;
        right_ch = where;
      }
      right_ch = FixUndersaturation(right_ch, tmp);
      EraseInner(right_ch, value);
      return;
    }
  }
}

template <typename T>
bool BTree<T>::Find(T value) {
  if (root_ == nullptr) {
    return false;
  }
  Node *cur = root_;
  while (cur != nullptr) {
    Node *par = cur;
    if (!Follow(cur, value)) {
      selected_ = cur;
      return true;
    }
  }
  return false;
}

template <typename T>
void BTree<T>::InsertInner(Node *node, T value) {
  auto iter = std::lower_bound(node->keys.begin(), node->keys.end(), value);
  int pos = std::distance(node->keys.begin(), iter);
  node->keys.insert(iter, value);
  node->children.insert(node->children.begin() + pos, nullptr);
}

template <typename T>
void BTree<T>::EraseInner(Node *node, T value) {
  auto iter = std::find(node->keys.begin(), node->keys.end(), value);
  if (iter == node->keys.end() || *iter != value) {
    return;
  }
  int pos = std::distance(node->keys.begin(), iter);
  node->keys.erase(iter);
  node->children.erase(node->children.begin() + pos);
  if (node->keys.size() == 0) {
    delete node;
    root_ = nullptr;
  }
}

template <typename T>
BTree<T>::Node* BTree<T>::FixOversaturation(Node *node, Node *par) {
  if (int(node->keys.size()) < 2 * factor - 1) {
    return node;
  }
  T med = node->keys[factor - 1];
  Node *brother = new Node();
  brother->children = std::vector<Node*>(node->children.begin() + factor, node->children.end());
  node->children.resize(factor);
  brother->keys = std::vector<T>(node->keys.begin() + factor, node->keys.end());
  node->keys.resize(factor - 1);
  if (par == nullptr) {
    root_ = new Node();
    root_->keys = {med};
    root_->children = {node, brother};
    return root_;
  } else {
    auto iter = std::find(par->children.begin(), par->children.end(), node);
    int pos = std::distance(par->children.begin(), iter);
    par->keys.insert(par->keys.begin() + pos, med);
    *iter = brother;
    par->children.insert(iter, node);
    return par;
  }
}

template <typename T>
BTree<T>::Node* BTree<T>::FixUndersaturation(Node *node, Node *par) {
  if (int(node->keys.size()) > factor - 1 || par == nullptr) {
    return node;
  }
  auto iter = std::find(par->children.begin(), par->children.end(), node);
  int pos = std::distance(par->children.begin(), iter);
  if (pos + 1 < int(par->children.size())) {
    if (int(par->children[pos + 1]->keys.size()) >= factor) {
      int x = par->keys[pos];
      par->children[pos]->keys.push_back(x);
      par->children[pos]->children.push_back(par->children[pos + 1]->children.front());
      par->keys[pos] = par->children[pos + 1]->keys.front();
      par->children[pos + 1]->keys.erase(par->children[pos + 1]->keys.begin());
      par->children[pos + 1]->children.erase(par->children[pos + 1]->children.begin());
      return node;
    }
  }
  if (pos - 1 >= 0) {
    if (int(par->children[pos - 1]->keys.size()) >= factor) {
      int x = par->keys[pos - 1];
      par->children[pos]->keys.insert(par->children[pos]->keys.begin(), x);
      par->children[pos]->children.insert(par->children[pos]->children.begin(),
                                          par->children[pos - 1]->children.back());
      par->keys[pos - 1] = par->children[pos - 1]->keys.back();
      par->children[pos - 1]->keys.pop_back();
      par->children[pos - 1]->children.pop_back();
      return node;
    }
  }
  if (pos + 1 == int(par->children.size())) {
    --pos;
  }
  node = par->children[pos];
  Node *nxt = par->children[pos + 1];
  node->children.insert(node->children.end(), nxt->children.begin(), nxt->children.end());
  node->keys.push_back(par->keys[pos]);
  node->keys.insert(node->keys.end(), nxt->keys.begin(), nxt->keys.end());
  par->keys.erase(par->keys.begin() + pos);
  par->children.erase(par->children.begin() + pos + 1);
  par->children[pos] = node;
  delete nxt;
  if (par->keys.empty()) {
    assert(par == root_);
    delete par;
    root_ = node;
    return node;
  } else {
    return node;
  }
}

template <typename T>
VisualizationData* BTree<T>::GetVisualizationData() {
  auto DFS = [&](auto&& self, Node *node) -> VisualizationData* {
    if (node == nullptr) {
      return nullptr;
    }
    VisualizationData *data = new VisualizationData();
    for (auto key : node->keys) {
      data->keys.push_back(std::to_string(key));
      if (node == selected_) {
        data->colors.push_back({QColor(Qt::green), QColor(Qt::white)});
      } else {
        data->colors.push_back({QColor("#CDCDCE"), QColor(Qt::black)});
      }
    }
    for (auto child : node->children) {
      data->children.push_back(self(self, child));
    }
    return data;
  };
  VisualizationData* data = DFS(DFS, root_);
  selected_ = nullptr;
  return data;
}

#endif // BTREE_IMPL
