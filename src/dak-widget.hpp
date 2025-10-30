#pragma once

#include <QDockWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

/**
 * @brief Custom Qt Dock Widget for the OBS Plugin.
 */
class DAKDock : public QDockWidget {
    Q_OBJECT

public:
    DAKDock(QWidget* parent = nullptr);
    ~DAKDock();

private slots:
    void refreshList();
    void selectItem();

private:
    QVBoxLayout* mainLayout;
    QComboBox* dropDownList;
    QPushButton* refreshButton;
    QPushButton* selectButton;
};