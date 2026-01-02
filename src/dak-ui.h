/********************************************************************************
** Form generated from reading UI file 'dak-panel.ui'
**
** Created by: Qt User Interface Compiler version 6.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef DAK_2D_PANEL_H
#define DAK_2D_PANEL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DAKDock
{
      public:
    //QWidget *dockWidgetContents;
    //QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QRadioButton *radioButton;
    QLineEdit *lineEdit;
    QSpacerItem *horizontalSpacer;
    QComboBox *dropDownList;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *refreshButton;
    QPushButton *selectButton;
    QSpacerItem *horizontalSpacer_2;
    QPlainTextEdit *plainTextEdit;

    void setupUi(QDockWidget *DockWidget)
    {
        if (DockWidget->objectName().isEmpty())
            DockWidget->setObjectName("DockWidget");
        DockWidget->resize(357, 312);
        DockWidget->setFeatures(QDockWidget::DockWidgetFloatable | QDockWidget::DockWidgetMovable);
        //dockWidgetContents = new QWidget();
        //dockWidgetContents->setObjectName("dockWidgetContents");
        //verticalLayoutWidget = new QWidget(dockWidgetContents);
        //verticalLayoutWidget->setObjectName("verticalLayoutWidget");
        //verticalLayoutWidget->setGeometry(QRect(0, 0, 351, 281));
        //verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        //radioButton = new QRadioButton(verticalLayoutWidget);
        radioButton = new QRadioButton();
        radioButton->setObjectName("radioButton");
        radioButton->setCheckable(false);

        horizontalLayout->addWidget(radioButton);

        //lineEdit = new QLineEdit(verticalLayoutWidget);
        lineEdit = new QLineEdit();
        lineEdit->setObjectName("lineEdit");
        lineEdit->setReadOnly(true);

        horizontalLayout->addWidget(lineEdit);

        verticalLayout->addLayout(horizontalLayout);

        horizontalSpacer = new QSpacerItem(40, 10, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        verticalLayout->addItem(horizontalSpacer);

        //dropDownList = new QComboBox(verticalLayoutWidget);
        dropDownList = new QComboBox();
        dropDownList->setObjectName("dropDownList");

        verticalLayout->addWidget(dropDownList);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        //refreshButton = new QPushButton(verticalLayoutWidget);
        refreshButton = new QPushButton();
        refreshButton->setObjectName("refreshButton");

        horizontalLayout_2->addWidget(refreshButton);

        //selectButton = new QPushButton(verticalLayoutWidget);
        selectButton = new QPushButton();
        selectButton->setObjectName("selectButton");

        horizontalLayout_2->addWidget(selectButton);

        verticalLayout->addLayout(horizontalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 10, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

        verticalLayout->addItem(horizontalSpacer_2);

        //plainTextEdit = new QPlainTextEdit(verticalLayoutWidget);
        plainTextEdit = new QPlainTextEdit();
        plainTextEdit->setObjectName("plainTextEdit");
        plainTextEdit->setReadOnly(true);

        verticalLayout->addWidget(plainTextEdit);

        //DockWidget->setWidget(dockWidgetContents);
        DockWidget->setLayout(verticalLayout);

        retranslateUi(DockWidget);

        QMetaObject::connectSlotsByName(DockWidget);
    }  // setupUi

    void retranslateUi(QDockWidget *DockWidget)
    {
        DockWidget->setWindowTitle(QCoreApplication::translate("DockWidget", "Daktronics Serial Reader", nullptr));
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

#endif  // DAK_2D_PANEL_H
