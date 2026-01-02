#pragma once

#include <QFrame>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QRadioButton>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QPlainTextEdit>
#include <QString>
#include <QWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QSpacerItem>
#include <string>
#include <sstream>

#include <obs-module.h>
#include <obs-frontend-api.h>

#include "dak-serial.hpp"
#include "dak-data-utils.hpp"
#include "dak-logger.hpp"

class DAKDock : public QFrame {
	Q_OBJECT

public:
	explicit DAKDock(QWidget *parent = nullptr);
	~DAKDock();

private slots:
	void startOutput();
	void refreshList();
	void selectItem();
	void appendLogMessage(const QString &message);
	void setConnected(const bool isConnected);

private:
	QRadioButton *radioButton;
	QLineEdit *lineEdit;
	QComboBox *dropDownList;
	QPushButton *refreshButton;
	QPushButton *selectButton;
	QPlainTextEdit *plainTextEdit;
	QComboBox *screenList;
	QPushButton *outputButton;
};
