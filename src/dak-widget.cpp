#include "dak-widget.hpp"

DAKDock::DAKDock(QWidget *parent) : QFrame(parent)
{
	QVBoxLayout *verticalLayout = new QVBoxLayout();

	QHBoxLayout *horizontalLayout = new QHBoxLayout();

	radioButton = new QRadioButton(this);
	radioButton->setCheckable(false);

	horizontalLayout->addWidget(radioButton);

	lineEdit = new QLineEdit(this);
	lineEdit->setReadOnly(true);

	horizontalLayout->addWidget(lineEdit);

	verticalLayout->addLayout(horizontalLayout);

	QSpacerItem *horizontalSpacer =
		new QSpacerItem(40, 10, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

	verticalLayout->addItem(horizontalSpacer);

	dropDownList = new QComboBox(this);

	verticalLayout->addWidget(dropDownList);

	QHBoxLayout *horizontalLayout_2 = new QHBoxLayout();
	refreshButton = new QPushButton(this);

	horizontalLayout_2->addWidget(refreshButton);

	selectButton = new QPushButton(this);

	horizontalLayout_2->addWidget(selectButton);

	verticalLayout->addLayout(horizontalLayout_2);

	QSpacerItem *horizontalSpacer_2 =
		new QSpacerItem(40, 10, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

	verticalLayout->addItem(horizontalSpacer_2);

	plainTextEdit = new QPlainTextEdit(this);
	plainTextEdit->setReadOnly(true);

	verticalLayout->addWidget(plainTextEdit);

	radioButton->setText(QCoreApplication::translate("DockWidget", "Connected", nullptr));
	refreshButton->setText(QCoreApplication::translate("DockWidget", "Refresh Ports", nullptr));
	selectButton->setText(QCoreApplication::translate("DockWidget", "Select Port", nullptr));

	setLayout(verticalLayout);

	// 4. Connect Signals and Slots
	connect(refreshButton, &QPushButton::clicked, this, &DAKDock::refreshList);
	connect(selectButton, &QPushButton::clicked, this, &DAKDock::selectItem);
	connect(&DAKLogger::instance(), &DAKLogger::logMessage, this, &DAKDock::appendLogMessage);
	connect(DAKDataUtils::serial.get(), &SerialPort::setConnected, this, &DAKDock::setConnected);
}

DAKDock::~DAKDock() {}

// --- Slot Implementations ---

void DAKDock::refreshList()
{
	// **Plugin Logic:** Clear and add new items (e.g., from a file or network)
	dropDownList->clear();

	std::vector<std::string> ports = SerialPort::listPorts();

	// 2. Add available ports
	for (const auto &port : ports) {
		dropDownList->addItem(port.c_str());
	}

	// 3. If no ports are found, add a placeholder
	if (ports.empty()) {
		dropDownList->addItem("No Serial Ports Found");
	}
}

void DAKDock::selectItem()
{
	QString selected = dropDownList->currentText();
	DAKDataUtils::startSerial(selected.toStdString());
}

void DAKDock::appendLogMessage(const QString &message)
{
	plainTextEdit->appendPlainText(message);
}

void DAKDock::setConnected(const bool isConnected)
{
	radioButton->setChecked(isConnected);
	lineEdit->setText(DAKDataUtils::getSerialPort().c_str());
}
