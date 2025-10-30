#include "dak-widget.hpp"

DAKDock::DAKDock() : QDockWidget("Daktronics Serial Reader", nullptr) {
    // 1. Create the main container widget and layout
    QWidget* contentWidget = new QWidget(this);
    mainLayout = new QVBoxLayout(contentWidget);

    // 2. Create UI Elements
    dropDownList = new QComboBox(this);
    refreshButton = new QPushButton("Refresh Ports", this);
    selectButton = new QPushButton("Select Port", this);

    // Initialize the dropdown with placeholder items
    refreshList();

    // 3. Add elements to the layout
    mainLayout->addWidget(dropDownList);
    mainLayout->addWidget(refreshButton);
    mainLayout->addWidget(selectButton);

    // Set the content widget to the dock
    contentWidget->setLayout(mainLayout);
    setWidget(contentWidget);

    // 4. Connect Signals and Slots
    connect(refreshButton, &QPushButton::clicked, this, &MyDock::refreshList);
    connect(selectButton, &QPushButton::clicked, this, &MyDock::selectItem);
}

DAKDock::~DAKDock() {}

// --- Slot Implementations ---

void DAKDock::refreshList() {
    // **Plugin Logic:** Clear and add new items (e.g., from a file or network)
    dropDownList->clear();

	std::vector<std::string> ports = SerialPort::listPorts();

    // 2. Add available ports
	for (const auto &port : ports) {
       dropDownList->addItem(port.c_str());
	}

	// 3. If no ports are found, add a placeholder
	if (ports.empty()) {
		dropDownList->addItem()"No Serial Ports Found");
	}
}

void DAKDock::selectItem() {
    QString selected = dropDownList->currentText();

    // **Plugin Logic:** Use the selected item (e.g., set a scene, start a stream)
    // Note: To interact with OBS, you would use 'obs_module_get_context()' or other API calls here.
    
    // Example: Show a small message
    selectButton->setText("Selected: " + selected);

    DAKDataUtils::startSerial(elected.toStdString());
}
