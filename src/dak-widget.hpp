#pragma once

#include <QDockWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>

#include "dak-serial.hpp"

/**
 * @brief Custom Qt Dock Widget for the OBS Plugin.
 */
class DAKDock : public QDockWidget {
    Q_OBJECT

public:
    DAKDock();
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
