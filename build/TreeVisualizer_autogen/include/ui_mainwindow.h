/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.9.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Widget
{
public:
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QComboBox *treeComboBox;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *valueEdit;
    QPushButton *insertButton;
    QPushButton *eraseButton;
    QPushButton *findButton;
    QGraphicsView *gView;

    void setupUi(QWidget *Widget)
    {
        if (Widget->objectName().isEmpty())
            Widget->setObjectName("Widget");
        Widget->resize(862, 698);
        layoutWidget = new QWidget(Widget);
        layoutWidget->setObjectName("layoutWidget");
        layoutWidget->setGeometry(QRect(10, 0, 181, 38));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget);
        label->setObjectName("label");
        QFont font;
        font.setPointSize(16);
        label->setFont(font);

        horizontalLayout->addWidget(label);

        treeComboBox = new QComboBox(layoutWidget);
        treeComboBox->setObjectName("treeComboBox");

        horizontalLayout->addWidget(treeComboBox);

        layoutWidget1 = new QWidget(Widget);
        layoutWidget1->setObjectName("layoutWidget1");
        layoutWidget1->setGeometry(QRect(10, 40, 439, 33));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setSpacing(5);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget1);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        horizontalLayout_2->addWidget(label_2);

        valueEdit = new QLineEdit(layoutWidget1);
        valueEdit->setObjectName("valueEdit");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(valueEdit->sizePolicy().hasHeightForWidth());
        valueEdit->setSizePolicy(sizePolicy);

        horizontalLayout_2->addWidget(valueEdit);

        insertButton = new QPushButton(layoutWidget1);
        insertButton->setObjectName("insertButton");
        sizePolicy.setHeightForWidth(insertButton->sizePolicy().hasHeightForWidth());
        insertButton->setSizePolicy(sizePolicy);
        insertButton->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_2->addWidget(insertButton);

        eraseButton = new QPushButton(layoutWidget1);
        eraseButton->setObjectName("eraseButton");

        horizontalLayout_2->addWidget(eraseButton);

        findButton = new QPushButton(layoutWidget1);
        findButton->setObjectName("findButton");

        horizontalLayout_2->addWidget(findButton);

        gView = new QGraphicsView(Widget);
        gView->setObjectName("gView");
        gView->setGeometry(QRect(15, 81, 831, 601));

        retranslateUi(Widget);

        QMetaObject::connectSlotsByName(Widget);
    } // setupUi

    void retranslateUi(QWidget *Widget)
    {
        Widget->setWindowTitle(QCoreApplication::translate("Widget", "Tree Visualizer", nullptr));
        label->setText(QCoreApplication::translate("Widget", "Select tree:", nullptr));
        label_2->setText(QCoreApplication::translate("Widget", "Operation:", nullptr));
        insertButton->setText(QCoreApplication::translate("Widget", "Insert", nullptr));
        eraseButton->setText(QCoreApplication::translate("Widget", "Erase", nullptr));
        findButton->setText(QCoreApplication::translate("Widget", "Find", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Widget: public Ui_Widget {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
