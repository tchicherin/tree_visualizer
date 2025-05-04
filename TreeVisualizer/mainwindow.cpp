#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "impl/AVLTree.cpp"
#include "impl/RBTree.cpp"
#include "impl/SplayTree.cpp"
#include "impl/BTree.cpp"
#include "impl/Treap.cpp"
#include "impl/Visualization.cpp"
#include <iostream>
#include <QShortcut>
#include <QGraphicsRectItem>
#include <QKeySequence>
#include <QCursor>
#include <string>
#include <limits>

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
  ui->setupUi(this);
  QGraphicsScene *scene = new QGraphicsScene(this);
  ui->gView->setScene(scene);
  ui->gView->setInteractive(true);
  ui->gView->setDragMode(QGraphicsView::ScrollHandDrag);

  ui->treeComboBox->insertItem(0, QString(""));
  ui->treeComboBox->insertItem(1, QString("AVL Tree"));
  ui->treeComboBox->insertItem(2, QString("RB Tree"));
  ui->treeComboBox->insertItem(3, QString("Splay Tree"));
  ui->treeComboBox->insertItem(4, QString("B-Tree"));
  ui->treeComboBox->insertItem(5, QString("Treap"));

  QShortcut *zoomInShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Equal), this);
  QObject::connect(zoomInShortcut, &QShortcut::activated, this, &Widget::ZoomIn);
  
  QShortcut *zoomOutShortcut = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_Minus), this);
  QObject::connect(zoomOutShortcut, &QShortcut::activated, this, &Widget::ZoomOut);
}

int Widget::GetNodeInput() {
  char* end;
  long res = strtol(ui->valueEdit->text().toStdString().c_str(), &end, 10);
  if (*end || res > INT_MAX || res < INT_MIN) {
    return -1;
  }
  return res;
}

void Widget::on_insertButton_clicked() {
  if (int inp = GetNodeInput(); inp != -1) {
    if (tree != nullptr) {
      tree->Insert(inp);
      Visualize(tree, this, ui->gView->scene(), tree->GetVisualizationData());
    }
  }
}

void Widget::on_eraseButton_clicked() {
  if (int inp = GetNodeInput(); inp != -1) {
    if (tree != nullptr) {
      tree->Erase(inp);
      Visualize(tree, this, ui->gView->scene(), tree->GetVisualizationData());
    }
  }
}

void Widget::on_findButton_clicked() {
  if (int inp = GetNodeInput(); inp != -1) {
    if (tree != nullptr) {
      tree->Find(inp);
      Visualize(tree, this, ui->gView->scene(), tree->GetVisualizationData());
    }
  }
}

void Widget::on_treeComboBox_currentIndexChanged(int index) {
  if (ui->gView->scene()) {
    ui->gView->scene()->clear();
  }
  ui->gView->resetTransform();
  ui->gView->centerOn(0, 0);
  delete ui->gView->scene();
  ui->gView->setScene(new QGraphicsScene(ui->gView));
  ui->gView->setViewportUpdateMode(QGraphicsView::MinimalViewportUpdate);
  delete tree;
  if (index == 1) {
    tree = new AVLTree<int>();
  } else if (index == 2) {
    tree = new RBTree<int>();
  } else if (index == 3) {
    tree = new SplayTree<int>(); 
  } else if (index == 4) {
    tree = new BTree<int>();
  } else if (index == 5) {
    tree = new Treap<int>();
  } else {
    tree = nullptr;
  }
}

void Widget::ZoomView(qreal factor) {
  QPoint point = QCursor::pos();
  QPointF scene_point = ui->gView->mapToScene(point);
  ui->gView->scale(factor, factor);
}

void Widget::ZoomIn() {
  ZoomView(1.25);
}

void Widget::ZoomOut() {
  ZoomView(1 / 1.25);
}

Widget::~Widget() {
  delete ui;
}
