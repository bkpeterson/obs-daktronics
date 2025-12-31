#include "dak-widget.hpp"

DAKDock::DAKDock(QWidget *parent) : QDockWidget(parent), ui(new Ui::DAKDock)
{
	ui->setupUi(this);

	// 4. Connect Signals and Slots
	connect(ui->refreshButton, &QPushButton::clicked, this, &DAKDock::refreshList);
	connect(ui->selectButton, &QPushButton::clicked, this, &DAKDock::selectItem);
	connect(&DAKLogger::instance(), &DAKLogger::logMessage, this, &DAKDock::appendLogMessage);
}

DAKDock::~DAKDock()
{
	delete ui;
}

// --- Slot Implementations ---

void DAKDock::refreshList()
{
	// **Plugin Logic:** Clear and add new items (e.g., from a file or network)
	ui->dropDownList->clear();

	std::vector<std::string> ports = SerialPort::listPorts();

	// 2. Add available ports
	for (const auto &port : ports) {
		ui->dropDownList->addItem(port.c_str());
	}

	// 3. If no ports are found, add a placeholder
	if (ports.empty()) {
		ui->dropDownList->addItem("No Serial Ports Found");
	}
}

void DAKDock::selectItem()
{
	QString selected = ui->dropDownList->currentText();

	// **Plugin Logic:** Use the selected item (e.g., set a scene, start a stream)
	// Note: To interact with OBS, you would use 'obs_module_get_context()' or other API calls here.

	// Example: Show a small message
	ui->selectButton->setText("Selected: " + selected);

	DAKDataUtils::startSerial(selected.toStdString());
}

void DAKDock::appendLogMessage(const QString &message)
{
	ui->plainTextEdit->appendPlainText(message);
}
