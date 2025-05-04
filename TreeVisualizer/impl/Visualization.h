#ifndef VISUALIZATION_H
#define VISUALIZATION_H

#include <QWidget>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <iostream>

struct VisualizationData;

template <typename T>
struct VisualizableTree {
  virtual void Insert(T value) = 0;
  virtual void Erase(T value) = 0;
  virtual bool Find(T value) = 0;

  virtual VisualizationData* GetVisualizationData() = 0;

  virtual ~VisualizableTree() = default;
};

class NodeItem : public QObject, public QGraphicsRectItem {
  Q_OBJECT
 public:
  explicit NodeItem(QObject *parent = nullptr) : QObject(parent) {}
 
  VisualizableTree<int> *ds;
  QWidget *widget;
  QGraphicsScene *scene;

  int value;

 signals:
  void clicked(NodeItem *item);

 protected:
  void mousePressEvent(QGraphicsSceneMouseEvent* event) override {
    emit clicked(this);
  }
};

struct VisualizationData { 
  std::vector<std::pair<QColor, QColor>> colors;
  std::vector<VisualizationData*> children;
  std::vector<std::string> keys;

  std::vector<std::pair<NodeItem*, QGraphicsTextItem*>> items;
  qreal total_width;
};

#endif // VISUALIZATION_H
