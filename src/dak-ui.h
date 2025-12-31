/********************************************************************************
** Form generated from reading UI file 'dak-control.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DAK_2D_UI_H
#define DAK_2D_UI_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DAKDock
{
      public:
    QWidget *dockWidgetContents;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton;
    QLineEdit *lineEdit;
    QComboBox *dropDownList;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *refreshButton;
    QPushButton *selectButton;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDockWidget *DockWidget)
    {
        if (DockWidget->objectName().isEmpty())
            DockWidget->setObjectName("DockWidget");
        DockWidget->resize(410, 303);
        DockWidget->setDockLocation(Qt::DockWidgetArea::NoDockWidgetArea);
        dockWidgetContents = new QWidget();
        dockWidgetContents->setObjectName("dockWidgetContents");
        gridLayoutWidget = new QWidget(dockWidgetContents);
        gridLayoutWidget->setObjectName("gridLayoutWidget");
        gridLayoutWidget->setGeometry(QRect(0, 0, 411, 281));
        gridLayout = new QGridLayout(gridLayoutWidget);
        gridLayout->setObjectName("gridLayout");
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        radioButton = new QRadioButton(gridLayoutWidget);
        radioButton->setObjectName("radioButton");
        radioButton->setCheckable(false);

        horizontalLayout->addWidget(radioButton);

        lineEdit = new QLineEdit(gridLayoutWidget);
        lineEdit->setObjectName("lineEdit");

        horizontalLayout->addWidget(lineEdit);

        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        dropDownList = new QComboBox(gridLayoutWidget);
        dropDownList->setObjectName("comboBox");

        gridLayout->addWidget(dropDownList, 3, 0, 1, 1);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        refreshButton = new QPushButton(gridLayoutWidget);
        refreshButton->setObjectName("pushButton_2");

        horizontalLayout_2->addWidget(refreshButton);

        selectButton = new QPushButton(gridLayoutWidget);
        selectButton->setObjectName("pushButton");

        horizontalLayout_2->addWidget(selectButton);

        gridLayout->addLayout(horizontalLayout_2, 4, 0, 1, 1);

        plainTextEdit = new QPlainTextEdit(gridLayoutWidget);
        plainTextEdit->setObjectName("plainTextEdit");

        gridLayout->addWidget(plainTextEdit, 5, 0, 1, 1);

        DockWidget->setWidget(dockWidgetContents);

        retranslateUi(DockWidget);

        QMetaObject::connectSlotsByName(DockWidget);
    }  // setupUi

    void retranslateUi(QDockWidget *DockWidget)
    {
        DockWidget->setWindowTitle(QCoreApplication::translate("DockWidget", "Daktronics Serial Data", nullptr));
        radioButton->setText(QCoreApplication::translate("DockWidget", "Connected", nullptr));
        refreshButton->setText(QCoreApplication::translate("DockWidget", "Refresh Ports", nullptr));
        selectButton->setText(QCoreApplication::translate("DockWidget", "Select Port", nullptr));
    }  // retranslateUi
};

namespace Ui {
    class DAKDock : public Ui_DAKDock
    {};
}  // namespace Ui

QT_END_NAMESPACE

#endif  // DAK_2D_UI_H
