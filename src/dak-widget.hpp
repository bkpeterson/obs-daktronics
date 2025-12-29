#pragma once

#include <QDockWidget>
#include <QVBoxLayout>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <string>
#include <sstream>

#include <obs-module.h>
#include <obs-frontend-api.h>

#include "dak-serial.hpp"
#include "dak-data-utils.hpp"

/**
 * @brief Custom Qt Dock Widget for the OBS Plugin.
 */
class DAKDock : public QDockWidget {
	Q_OBJECT

public:
	DAKDock();
	~DAKDock();

	void updateLog(uint32_t code, std::string text);
	void updateFilterLog(uint32_t code, const char *source, std::string text);

private slots:
	void refreshList();
	void selectItem();

private:
	static DAKDock *_self;
	QVBoxLayout *mainLayout;
	QComboBox *dropDownList;
	QPushButton *refreshButton;
	QPushButton *selectButton;
	QPlainTextEdit *logBox;
};
