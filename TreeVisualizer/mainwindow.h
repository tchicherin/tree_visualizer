#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QWheelEvent>
#include <QLineEdit>
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
  int index = 0, factor = 2;

 private slots:
  void on_submitButton_clicked();
  void on_insertButton_clicked();
  void on_eraseButton_clicked();
  void on_findButton_clicked();
  void on_treeComboBox_currentIndexChanged(int index);
  void on_randomButton_clicked();

 private:
  Ui::Widget *ui;

  int GetNodeInput(QLineEdit *edit);

  void ZoomIn();

  void ZoomOut();

  void ZoomView(qreal factor);

  void MakeTree();
};

#endif // MAINWINDOW_H
