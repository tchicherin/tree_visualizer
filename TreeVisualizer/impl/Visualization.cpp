#ifndef VISUALIZATION_IMPL
#define VISUALIZATION_IMPL

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsRectItem>
#include <QFontMetrics>
#include <QApplication>
#include <QColor>
#include <QWidget>
#include <QPen>
#include <vector>
#include <string>
#include <tuple>
#include <utility>
#include <iostream>

#include "Visualization.h"

static void Visualize(VisualizableTree<int> *tree, QWidget *widget,
    QGraphicsScene *scene, VisualizationData *data) {
  scene->clear();
  constexpr qreal kHeightMargin = 30, kPadding = 10, kWidthMargin = 50;
  constexpr qreal kOneChildHack = kWidthMargin;
  auto GetWidth = [&](VisualizationData *data) -> qreal {
    qreal width = 0;
    for (auto [item, text_item] : data->items) {
      if (item != nullptr) {
        width += item->rect().width();
      }
    }
    return width;
  };
  auto MakeItem = [&](int& x, int& y, VisualizationData *cur, int index) {
    std::string value = cur->keys[index];
    auto [back_color, fore_color] = cur->colors[index];
    QFontMetrics fm(QApplication::font());
    QRectF textRect = fm.boundingRect(QString::fromStdString(value));
    textRect.setX(x), textRect.setY(y);
    cur->items.push_back(std::make_pair(new NodeItem(), new QGraphicsTextItem()));
    cur->items.back().first->setBrush(back_color);
    cur->items.back().second->setPlainText(QString::fromStdString(value));
    cur->items.back().second->setX(x + kPadding);
    cur->items.back().second->setY(y + kPadding);
    cur->items.back().second->setDefaultTextColor(fore_color);
    textRect.setWidth(cur->items.back().second->boundingRect().width() + 2 * kPadding);
    textRect.setHeight(cur->items.back().second->boundingRect().height() + 2 * kPadding);
    cur->items.back().first->setRect(textRect);
    cur->items.back().first->value = std::stoi(value);
    cur->items.back().first->ds = tree;
    cur->items.back().first->widget = widget;
    cur->items.back().first->scene = scene;
    widget->connect(cur->items.back().first, &NodeItem::clicked, [](NodeItem *item) {
      item->ds->Erase(item->value);
      Visualize(item->ds, item->widget, item->scene, item->ds->GetVisualizationData());
    });
    x += textRect.width();
  };
  auto PreinitItems = [&](auto&& self, VisualizationData *cur) -> void {
    if (cur == nullptr) {
      return;
    }
    int x = 0, y = 0;
    for (int i = 0; i < int(cur->keys.size()); i++) {
      MakeItem(x, y, cur, i);
    }
    bool left_hack = false, right_hack = false;
    if (cur->children.size() == 2) {
      if (cur->children[0] != nullptr && cur->children[1] == nullptr) {
        left_hack = true; 
      }
      if (cur->children[0] == nullptr && cur->children[1] != nullptr) {
        right_hack = true;
      }
    }
    qreal children_width = 0;
    for (auto child : cur->children) {
      self(self, child);
      if (child != nullptr) {
        children_width += child->total_width + kWidthMargin;
      }
    }
    if (children_width > 0) {
      children_width -= kWidthMargin;
    }
    cur->total_width = std::max(GetWidth(cur), children_width);
    if (left_hack || right_hack) {
      cur->total_width += kOneChildHack; 
    }
  };
  auto ShowItems = [&](auto&& self, VisualizationData *cur, qreal offset_x, qreal offset_y) -> qreal {
    if (cur == nullptr) {
      return 0;
    }
    qreal width = GetWidth(cur);
    bool left_hack = false, right_hack = false;
    if (cur->children.size() == 2) {
      if (cur->children[0] != nullptr && cur->children[1] == nullptr) {
        left_hack = true; 
      }
      if (cur->children[0] == nullptr && cur->children[1] != nullptr) {
        right_hack = true;
      }
    }
    qreal pure_width = cur->total_width;
    if (left_hack || right_hack) {
      pure_width -= kOneChildHack;
    }
    qreal start_x = offset_x + (pure_width - width) / 2;
    qreal center = pure_width / 2;
    if (left_hack) {
      start_x += kOneChildHack;
      center += kOneChildHack;
    }
    for (auto [item, text_item] : cur->items) {
      item->setX(start_x + item->x());
      item->setY(offset_y + item->y());
      text_item->setX(start_x + text_item->x());
      text_item->setY(offset_y + text_item->y());
      scene->addItem(item);
      scene->addItem(text_item);
    }
    if (right_hack) {
      offset_x += kOneChildHack;
    }
    std::vector<qreal> pref(cur->items.size() + 1);
    pref[0] = start_x;
    for (int i = 0; i < int(cur->items.size()); i++) {
      pref[i + 1] = pref[i] + cur->items[i].first->rect().width();
    }
    qreal offset_y0 = offset_y;
    qreal h = cur->items[0].first->rect().height();
    offset_y += h + kHeightMargin;
    for (int i = 0; i < int(cur->children.size()); i++) {
      auto child = cur->children[i];
      if (child != nullptr) {
        qreal child_center = self(self, child, offset_x, offset_y);
        QGraphicsLineItem *line = new QGraphicsLineItem(pref[i], offset_y0 + h, offset_x + child_center, offset_y);
        QPen pen;
        pen.setWidth(1);
        line->setPen(pen);
        scene->addItem(line);
        offset_x += child->total_width + kWidthMargin;
      }
    }
    return center;
  };
  PreinitItems(PreinitItems, data);
  ShowItems(ShowItems, data, 0, 0);
  scene->setSceneRect(scene->itemsBoundingRect());
}

#endif // VISUALIZATION_H
