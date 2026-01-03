#include "dak-widget.hpp"

DAKDock::DAKDock(QWidget *parent) : QFrame(parent)
{
	QVBoxLayout *verticalLayout = new QVBoxLayout();

	QHBoxLayout *outputLayout = new QHBoxLayout();

	screenList = new QComboBox(this);

	QList<QScreen *> screens = QGuiApplication::screens();
	for (int i = 0; i < screens.size(); ++i) {
		QScreen *screen = screens[i];
		screenList->addItem((screen->name().toStdString() + " [" + std::to_string(screen->geometry().width()) +
				     "x" + std::to_string(screen->geometry().height()) + "]")
					    .c_str(),
				    (uint)i);
	}

	outputLayout->addWidget(screenList);

	outputButton = new QPushButton(this);
	outputLayout->addWidget(outputButton);

	verticalLayout->addLayout(outputLayout);

	QSpacerItem *horizontalSpacer0 =
		new QSpacerItem(40, 10, QSizePolicy::Policy::Expanding, QSizePolicy::Policy::Minimum);

	verticalLayout->addItem(horizontalSpacer0);

	QHBoxLayout *horizontalLayout = new QHBoxLayout();

	connectedBox = new QCheckBox(this);
	connectedBox->setCheckable(false);
	connectedBox->setStyleSheet(
		"QLabel {"
		"color: red;"
		"}"
	);

	horizontalLayout->addWidget(connectedBox);

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

	outputButton->setText("Show Display");
	connectedBox->setText(" Disconnected");
	refreshButton->setText("Refresh Ports");
	selectButton->setText("Select Port");

	setLayout(verticalLayout);

	// 4. Connect Signals and Slots
	connect(outputButton, &QPushButton::clicked, this, &DAKDock::startOutput);
	connect(refreshButton, &QPushButton::clicked, this, &DAKDock::refreshList);
	connect(selectButton, &QPushButton::clicked, this, &DAKDock::selectItem);
	connect(&DAKLogger::instance(), &DAKLogger::logMessage, this, &DAKDock::appendLogMessage);
	connect(DAKDataUtils::serial.get(), &SerialPort::setConnected, this, &DAKDock::setConnected);

	refreshList();

	char *config_path = obs_module_config_path("dak-settings.json");
	std::string path = config_path ? config_path : "";
	bfree(config_path);
	
	size_t pos = path.find_last_of("/\\");
	if (pos != std::string::npos) {
		std::string dir = path.substr(0, pos);
		os_mkdirs(dir.c_str());
	}

	obs_data_t *settings = obs_data_create_from_json_file(path.c_str());

	const char *lastPort = obs_data_get_string(settings, "serial_port");
	appendLogMessage("Loaded serial port: " + QString::fromStdString(lastPort ? lastPort : "None"));
	if (lastPort && std::string(lastPort).length() > 0) {
		dropDownList->setCurrentText(lastPort);
		selectItem();
	}

	const char *lastScreen = obs_data_get_string(settings, "screen");
	appendLogMessage("Loaded screen: " + QString::fromStdString(lastScreen ? lastScreen : "None"));
	if (lastScreen && std::string(lastScreen).length() > 0) {
		screenList->setCurrentText(lastScreen);
	}

	obs_data_release(settings);
}

DAKDock::~DAKDock() {}

// --- Slot Implementations ---

void DAKDock::startOutput() {
	char *config_path = obs_module_config_path("dak-settings.json");
	std::string path = config_path ? config_path : "";
	bfree(config_path);
	
	size_t pos = path.find_last_of("/\\");
	if (pos != std::string::npos) {
		std::string dir = path.substr(0, pos);
		os_mkdirs(dir.c_str());
	}

	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "serial_port", dropDownList->currentText().toStdString().c_str());
	obs_data_set_string(settings, "screen", screenList->currentText().toStdString().c_str());
	if(obs_data_save_json(settings, path.c_str()))
		appendLogMessage("Settings saved successfully.");
	else
		appendLogMessage("Failed to save settings.");
	obs_data_release(settings);

	obs_frontend_open_projector("Preview", screenList->currentIndex(), nullptr, "Daktronics Scoreboard Output");
}

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

	char *config_path = obs_module_config_path("dak-settings.json");
	std::string path = config_path ? config_path : "";
	bfree(config_path);
	
	size_t pos = path.find_last_of("/\\");
	if (pos != std::string::npos) {
		std::string dir = path.substr(0, pos);
		os_mkdirs(dir.c_str());
	}

	obs_data_t *settings = obs_data_create();
	obs_data_set_string(settings, "serial_port", selected.toStdString().c_str());
	obs_data_set_string(settings, "screen", screenList->currentText().toStdString().c_str());
	if(obs_data_save_json(settings, path.c_str()))
		appendLogMessage("Settings saved successfully.");
	else
		appendLogMessage("Failed to save settings.");
	obs_data_release(settings);
}

void DAKDock::appendLogMessage(const QString &message)
{
	QString curText = plainTextEdit->toPlainText();
	curText.append(message);

	if (curText.length() > 10000) {
		curText = curText.right(8000);
		plainTextEdit->setPlainText(curText);
	} else {
		plainTextEdit->appendPlainText(message);
	}
}

void DAKDock::setConnected(const bool isConnected)
{
	connectedBox->setChecked(isConnected);
	
	if (isConnected) {
		connectedBox->setText(" Connected");
		connectedBox->setStyleSheet(
			"QLabel {"
			"color: green;"
			"}"
		);
	} else {
		connectedBox->setText(" Disconnected");
		connectedBox->setStyleSheet(
			"QLabel {"
			"color: red;"
			"}"
		);
	}

	lineEdit->setText(DAKDataUtils::getSerialPort().c_str());
}
