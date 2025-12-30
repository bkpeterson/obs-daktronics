#pragma once

#include <QDockWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QString>
#include <string>
#include <sstream>

#include <obs-module.h>
#include <obs-frontend-api.h>

#include "dak-serial.hpp"
#include "dak-data-utils.hpp"
#include "dak-logger.hpp"

/**
 * @brief Custom Qt Dock Widget for the OBS Plugin.
 */
class DAKDock : public QDockWidget {
	Q_OBJECT

public:
	//DAKDock(QWidget *parent = nullptr);
	DAKDock();
	~DAKDock();

private slots:
	void refreshList();
	void selectItem();
	void appendLogMessage(const QString &message);

private:
	QVBoxLayout *mainLayout;
	QComboBox *dropDownList;
	QPushButton *refreshButton;
	QPushButton *selectButton;
	QPlainTextEdit *logBox;
};
