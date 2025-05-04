#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QWheelEvent>
#include "impl/Visualization.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget {
  Q_OBJECT

 public:
  explicit Widget(QWidget *parent = nullptr);
  ~Widget();

  VisualizableTree<int> *tree = nullptr;

 private slots:
  void on_insertButton_clicked();
  void on_eraseButton_clicked();
  void on_findButton_clicked();
  void on_treeComboBox_currentIndexChanged(int index);

 private:
  Ui::Widget *ui;

  int GetNodeInput();

  void ZoomIn();

  void ZoomOut();

  void ZoomView(qreal factor);
};

#endif // MAINWINDOW_H
